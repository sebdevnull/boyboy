/**
 * @file emulator.cpp
 * @brief Main emulator class for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/emulator.h"

namespace boyboy::emulator {

void Emulator::load_cartridge(const std::string& path)
{
    cartridge_.load_rom(path);
    mmu_->map_rom(cartridge_);
}

void Emulator::step()
{
    auto cycles = cpu_.step();
    io_.tick(cycles);

    if (ppu_.frame_ready()) {
        display_.render_frame(ppu_.framebuffer());
        ppu_.consume_frame();
    }
}

void Emulator::run()
{
    ppu_.set_mem_read_cb([this](uint16_t addr) { return mmu_->read_byte(addr); });

    display_.init();

    running_ = true;
    while (running_) {
        step();
        display_.poll_events(running_);
    }

    display_.shutdown();
}

void Emulator::reset()
{
    cpu_.reset();
    mmu_.reset();
    io_.reset();
}

} // namespace boyboy::emulator