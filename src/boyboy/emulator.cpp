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
    cpu_.step();
    display_.update();
}

void Emulator::run()
{
    while (true) {
        step();
    }
}

void Emulator::reset()
{
    cpu_.reset();
    mmu_.reset();
}

} // namespace boyboy::emulator