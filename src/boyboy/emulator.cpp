/**
 * @file emulator.cpp
 * @brief Main emulator class for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/emulator.h"

#include "boyboy/cart/cartridge_loader.h"
#include "boyboy/log/logging.h"
#include "boyboy/profiling/profiler_utils.h"

namespace boyboy::emulator {

void Emulator::load(const std::string& path)
{
    log::info("Loading ROM from {}", path);

    cartridge_ = cart::CartridgeLoader::load(path);
    mmu_->map_rom(cartridge_);
}

void Emulator::step()
{
    auto cycles = cpu_.step();
    mmu_->tick_dma(cycles);
    io_.tick(cycles);

    // Update statistics before frame
    instruction_count_++;
    cycle_count_ += cycles;

    if (ppu_.frame_ready()) {
        display_.render_frame(ppu_.framebuffer());
        ppu_.consume_frame();

        // Update and log frame statistics
        BB_PROFILE_FRAME(instruction_count_, cycle_count_);
        instruction_count_ = 0;
        cycle_count_ = 0;
    }
}

void Emulator::run()
{
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

    // TODO: running uncapped for now, add frame limiting when we optimize performance
    running_ = true;
    while (running_) {
        step();
        display_.poll_events(running_);
    }

    BB_PROFILE_REPORT();
    BB_FRAME_PROFILE_REPORT();

    log::info("Stopping emulator...");

    display_.shutdown();
}

void Emulator::reset()
{
    log::info("Resetting emulator...");
    cpu_.reset();
    mmu_.reset();
    io_.reset();
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

} // namespace boyboy::emulator