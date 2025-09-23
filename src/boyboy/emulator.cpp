/**
 * @file emulator.cpp
 * @brief Main emulator class for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/emulator.h"

#include "boyboy/log/logging.h"

namespace boyboy::emulator {

void Emulator::load(const std::string& path)
{
    log::info("Loading ROM from {}", path);

    cartridge_.load_rom(path);
    mmu_->map_rom(cartridge_);
}

void Emulator::step()
{
    auto cycles = cpu_.step();
    mmu_->tick_dma(cycles);
    io_.tick(cycles);

    if (ppu_.frame_ready()) {
        display_.render_frame(ppu_.framebuffer());
        ppu_.consume_frame();
        frame_count_++;
    }
    instruction_count_++;
    cycle_count_ += cycles;
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

    auto start = std::chrono::high_resolution_clock::now();

    // TODO: running uncapped for now, add frame limiting when we optimize performance
    running_ = true;
    while (running_) {
        step();
        display_.poll_events(running_);

        // Simple FPS/IPS counter
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
        if (elapsed >= 3) {
            log::info("FPS: {:.2f}, IPS: {:.2f}, CPS: {:.2f}",
                      frame_count_ / static_cast<double>(elapsed),
                      instruction_count_ / static_cast<double>(elapsed),
                      cycle_count_ / static_cast<double>(elapsed));
            // Reset counters
            frame_count_ = 0;
            instruction_count_ = 0;
            cycle_count_ = 0;
            start = now;
        }
    }

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