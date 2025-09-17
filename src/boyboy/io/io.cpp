/**
 * @file io.cpp
 * @brief Input/Output operations for BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/io/io.h"

namespace boyboy::io {

Io::Io()
{
    // Initialize components' interrupt callbacks
    for (auto* component : components_) {
        component->set_interrupt_cb(
            [this](uint8_t interrupt) { this->write(IoReg::Interrupts::IF, interrupt); });
    }
}

void Io::tick(uint16_t cycles)
{
    for (auto* component : components_) {
        component->tick(cycles);
    }
}

[[nodiscard]] uint8_t Io::read(uint16_t addr) const
{
    if (IoReg::Serial::contains(addr)) {
        return serial_.read(addr);
    }
    if (IoReg::Timer::contains(addr)) {
        return timer_.read(addr);
    }

    // Default behavior: return the value in the register
    return registers_.at(io_addr(addr));
}

void Io::write(uint16_t addr, uint8_t value)
{
    if (IoReg::Serial::contains(addr)) {
        serial_.write(addr, value);
        return;
    }
    if (IoReg::Timer::contains(addr)) {
        timer_.write(addr, value);
        return;
    }

    // Default behavior: write the value to the register
    registers_.at(io_addr(addr)) = value;
}

void Io::reset()
{
    registers_.fill(0);
    for (auto* component : components_) {
        component->reset();
    }
}

} // namespace boyboy::io