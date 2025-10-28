/**
 * @file emulator.cpp
 * @brief Main emulator class for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/core/emulator/emulator.h"

#include <chrono>
#include <memory>
#include <thread>

#include "boyboy/common/config/config.h"
#include "boyboy/common/log/logging.h"
#include "boyboy/common/save/save_manager.h"
#include "boyboy/core/cartridge/cartridge.h"
#include "boyboy/core/cartridge/cartridge_loader.h"
#include "boyboy/core/cpu/cpu.h"
#include "boyboy/core/display/display.h"
#include "boyboy/core/io/buttons.h"
#include "boyboy/core/io/io.h"
#include "boyboy/core/io/joypad.h"
#include "boyboy/core/io/serial.h"
#include "boyboy/core/io/timer.h"
#include "boyboy/core/mmu/mmu.h"
#include "boyboy/core/ppu/ppu.h"
#include "boyboy/core/profiling/profiler_utils.h"

namespace boyboy::core::emulator {

using namespace boyboy::common;

Emulator::Emulator()
    : io_(std::make_shared<io::Io>()),
      mmu_(std::make_shared<mmu::Mmu>(io_)),
      cpu_(std::make_shared<cpu::Cpu>(mmu_)),
      ppu_(std::make_shared<ppu::Ppu>(mmu_.get())),
      timer_(std::make_shared<io::Timer>()),
      joypad_(std::make_shared<io::Joypad>()),
      serial_(std::make_shared<io::Serial>()),
      display_(std::make_shared<display::Display>()),
      cartridge_(std::make_unique<cartridge::Cartridge>())
{
}

Emulator::~Emulator() = default;

void Emulator::init()
{
    log::info("Initializing emulator...");
    io_->register_component(ppu_);
    io_->register_component(timer_);
    io_->register_component(joypad_);
    io_->register_component(serial_);

    mmu_->init();
    io_->init();
    cpu_->init();
}

void Emulator::reset()
{
    log::info("Resetting emulator...");
    cpu_->reset();
    mmu_->reset();
    io_->reset();
}

void Emulator::load(const std::string& path)
{
    log::info("Loading ROM from {}", path);

    cartridge_ = cartridge::CartridgeLoader::load(path);
    mmu_->map_rom(*cartridge_);
}

void Emulator::start()
{
    if (started_) {
        log::warn("Emulator already started");
        return;
    }

    log::info("Starting emulator...");

    // Hook system callbacks
    display_->set_button_cb([this](io::Button b, bool p) { on_button_event(b, p); });
    cartridge_->set_ram_load_cb([this]() {
        auto res = save::SaveManager::instance().load_sram(cartridge_->get_header().title);
        return (res.has_value()) ? res.value() : std::vector<uint8_t>{};
    });
    cartridge_->set_ram_save_cb([this](auto data) {
        auto res = save::SaveManager::instance().save_sram(cartridge_->get_header().title, data);
        return res.has_value();
    });

    cartridge_->load_ram();
    display_->init();

    // Enable LCD: it seems that some games expect it to be on at start, probably because the boot
    // ROM does it so we will enable it until we implement the boot ROM (if we ever do)
    ppu_->enable_lcd(true);

    started_ = true;
}

void Emulator::stop()
{
    if (!started_) {
        log::warn("Emulator not started");
        return;
    }

    cartridge_->save_ram();

    log::info("Stopping emulator...");

    BB_PROFILE_REPORT();
    BB_HOT_PROFILE_REPORT();
    BB_FRAME_PROFILE_REPORT();

    display_->shutdown();
    started_ = false;
}

void Emulator::run()
{
    start();

    using clock = std::chrono::steady_clock;
    auto next_frame_time = clock::now();
    constexpr auto FrameDuration = std::chrono::duration_cast<clock::duration>(
        std::chrono::duration<double>(ppu::FrameDuration)
    );

    running_ = true;
    while (running_) {
        display_->poll_events(running_);
        emulate_frame();
        render_frame();

        // Frame limiting (if needed) at 59.73Hz * speed_
        if (frame_rate_limited_) {
            next_frame_time += FrameDuration / speed_;
            auto now = clock::now();
            if (now < next_frame_time) {
                std::this_thread::sleep_until(next_frame_time);
            }
            else {
                next_frame_time = now;
            }
        }
    }

    stop();
}

void Emulator::apply_config(const common::config::Config& config)
{
    log::info("Applying configuration...");

    // Emulator settings
    speed_ = config.emulator.speed;
    frame_rate_limited_ = speed_ != 0;

    // Video settings
    display_->set_scale(config.video.scale);
    display_->set_vsync(config.video.vsync);

    // Battery save settings
    cartridge_->enable_autosave(config.saves.autosave);
    cartridge_->set_save_interval_ms(config.saves.save_interval);

    // Logging settings
    log::set_level(config.debug.log_level);

    log::info("Configuration applied");
}

void Emulator::on_button_event(io::Button button, bool pressed)
{
    if (pressed) {
        joypad_->press(button);
    }
    else {
        joypad_->release(button);
    }
}

void Emulator::emulate_frame()
{
    while (!ppu_->frame_ready()) {
        auto cycles = cpu_->tick();
        instruction_count_++;
        cycle_count_ += cycles;

        mmu_->tick_dma(cycles);
        io_->tick(cycles);
        cartridge_->tick();
    }

    // Check if there is any drift in the cycle count
    constexpr int CycleDriftTolerance = 8;
    int64_t cycle_diff = static_cast<int64_t>(cycle_count_) -
                         static_cast<int64_t>(ppu::CyclesPerFrame);
    if (std::abs(cycle_diff) > CycleDriftTolerance) {
        log::warn("Frame cycle count drift detected: {} cycles", cycle_diff);
    }
}

void Emulator::render_frame()
{
    display_->render_frame(ppu_->framebuffer());
    ppu_->consume_frame();

    // Update and log frame statistics
    BB_PROFILE_FRAME(instruction_count_, cycle_count_);
    instruction_count_ = 0;
    cycle_count_ = 0;
}

} // namespace boyboy::core::emulator