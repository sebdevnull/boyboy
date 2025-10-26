/**
 * @file io.cpp
 * @brief Input/Output operations for BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/core/io/io.h"

#include "boyboy/core/io/joypad.h"
#include "boyboy/core/io/serial.h"
#include "boyboy/core/io/timer.h"
#include "boyboy/core/ppu/ppu.h"

namespace boyboy::core::io {

void Io::init()
{
    registers_.fill(0);
    for (auto& component : components_) {
        component->init();
    }
}

void Io::reset()
{
    registers_.fill(0);
    for (auto& component : components_) {
        component->reset();
    }
}

void Io::tick(uint16_t cycles)
{
    for (auto& component : components_) {
        component->tick(cycles);
    }
}

[[nodiscard]] uint8_t Io::read(uint16_t addr) const
{
    if (IoReg::Ppu::contains(addr)) {
        return ppu_->read(addr);
    }
    if (IoReg::Timer::contains(addr)) {
        return timer_->read(addr);
    }
    if (IoReg::Joypad::contains(addr)) {
        return joypad_->read(addr);
    }
    if (IoReg::Serial::contains(addr)) {
        return serial_->read(addr);
    }

    // Default behavior: return the value in the register
    return registers_.at(io_addr(addr));
}

void Io::write(uint16_t addr, uint8_t value)
{
    if (IoReg::Ppu::contains(addr)) {
        ppu_->write(addr, value);
        return;
    }
    if (IoReg::Timer::contains(addr)) {
        timer_->write(addr, value);
        return;
    }
    if (IoReg::Joypad::contains(addr)) {
        joypad_->write(addr, value);
        return;
    }
    if (IoReg::Serial::contains(addr)) {
        serial_->write(addr, value);
        return;
    }

    // Default behavior: write the value to the register
    registers_.at(io_addr(addr)) = value;
}

[[nodiscard]] const std::shared_ptr<ppu::Ppu>& Io::ppu() const
{
    return ppu_;
}
[[nodiscard]] std::shared_ptr<ppu::Ppu>& Io::ppu()
{
    return ppu_;
}
[[nodiscard]] const std::shared_ptr<Timer>& Io::timer() const
{
    return timer_;
}
[[nodiscard]] std::shared_ptr<Timer> Io::timer()
{
    return timer_;
}
[[nodiscard]] const std::shared_ptr<Joypad>& Io::joypad() const
{
    return joypad_;
}
[[nodiscard]] std::shared_ptr<Joypad> Io::joypad()
{
    return joypad_;
}
[[nodiscard]] const std::shared_ptr<Serial>& Io::serial() const
{
    return serial_;
}
[[nodiscard]] std::shared_ptr<Serial> Io::serial()
{
    return serial_;
}

} // namespace boyboy::core::io