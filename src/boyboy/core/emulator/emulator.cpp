/**
 * @file emulator.cpp
 * @brief Main emulator class for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/core/emulator/emulator.h"

#include <chrono>
#include <thread>

#include "boyboy/common/config/config.h"
#include "boyboy/common/log/logging.h"
#include "boyboy/core/cartridge/cartridge_loader.h"
#include "boyboy/core/ppu/ppu.h"
#include "boyboy/core/profiling/profiler_utils.h"

namespace boyboy::core::emulator {

using namespace boyboy::common;

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
    ppu_.set_mem_read_cb([this](uint16_t addr) { return mmu_->read_byte(addr); });
    ppu_.set_mem_write_cb([this](uint16_t addr, uint8_t value) { mmu_->write_byte(addr, value); });
    ppu_.set_dma_start_cb([this](uint8_t value) { mmu_->start_dma(value); });
    display_.set_button_cb([this](io::Button b, bool p) { on_button_event(b, p); });

    display_.init();

    // Enable LCD: it seems that some games expect it to be on at start, probably because the boot
    // ROM does it so we will enable it until we implement the boot ROM (if we ever do)
    ppu_.enable_lcd(true);

    started_ = true;
}

void Emulator::stop()
{
    if (!started_) {
        log::warn("Emulator not started");
        return;
    }

    log::info("Stopping emulator...");

    BB_PROFILE_REPORT();
    BB_HOT_PROFILE_REPORT();
    BB_FRAME_PROFILE_REPORT();

    display_.shutdown();
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

    // TODO: allow configurable frame rate for high refresh rate monitors and "turbo" mode.
    // For now we run at the native frame rate (59.73Hz)
    running_ = true;
    while (running_) {
        display_.poll_events(running_);
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

void Emulator::reset()
{
    log::info("Resetting emulator...");
    cpu_.reset();
    mmu_.reset();
    io_.reset();
}

void Emulator::apply_config(const common::config::Config& config)
{
    log::info("Applying configuration...");

    // Emulator settings
    speed_ = config.emulator.speed;
    frame_rate_limited_ = speed_ != 0;

    // Video settings
    display_.set_scale(config.video.scale);
    display_.set_vsync(config.video.vsync);

    // Logging settings
    log::set_level(config.debug.log_level);

    log::info("Configuration applied");
}

void Emulator::on_button_event(io::Button button, bool pressed)
{
    if (pressed) {
        joypad_.press(button);
    }
    else {
        joypad_.release(button);
    }
}

void Emulator::emulate_frame()
{
    while (!ppu_.frame_ready()) {
        auto cycles = cpu_.step();
        instruction_count_++;
        cycle_count_ += cycles;

        mmu_->tick_dma(cycles);
        io_.tick(cycles);
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
    display_.render_frame(ppu_.framebuffer());
    ppu_.consume_frame();

    // Update and log frame statistics
    BB_PROFILE_FRAME(instruction_count_, cycle_count_);
    instruction_count_ = 0;
    cycle_count_ = 0;
}

} // namespace boyboy::core::emulator