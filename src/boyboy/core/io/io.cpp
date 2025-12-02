/**
 * @file io.cpp
 * @brief Input/Output operations for BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/core/io/io.h"

#include "boyboy/common/log/logging.h"
#include "boyboy/common/utils.h"
#include "boyboy/core/io/apu.h"
#include "boyboy/core/io/constants.h"
#include "boyboy/core/io/iocomponent.h"
#include "boyboy/core/io/joypad.h"
#include "boyboy/core/io/registers.h"
#include "boyboy/core/io/serial.h"
#include "boyboy/core/io/timer.h"
#include "boyboy/core/ppu/ppu.h"

namespace boyboy::core::io {

void Io::init()
{
    // Initialize registers (assume DMG0)
    registers_.fill(0xFF);
    registers_.at(io_addr(IoReg::Interrupts::IF)) = RegInitValues::Dmg0::Interrupts::IF;

    // Initialize components
    for (auto& component : components_) {
        component->init();
    }
}

void Io::reset()
{
    // Reset registers (assume DMG0)
    registers_.fill(0xFF);
    registers_.at(io_addr(IoReg::Interrupts::IF)) = RegInitValues::Dmg0::Interrupts::IF;

    // Reset components
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
        return component_read(ppu_.get(), addr);
    }
    if (IoReg::Timer::contains(addr)) {
        return component_read(timer_.get(), addr);
    }
    if (IoReg::Joypad::contains(addr)) {
        return component_read(joypad_.get(), addr);
    }
    if (IoReg::Serial::contains(addr)) {
        return component_read(serial_.get(), addr);
    }
    if (IoReg::Apu::contains(addr)) {
        return component_read(apu_.get(), addr);
    }

    // Default behavior: return the value in the register
    return registers_.at(io_addr(addr));
}

void Io::write(uint16_t addr, uint8_t value)
{
    if (IoReg::Ppu::contains(addr)) {
        component_write(ppu_.get(), addr, value);
        return;
    }
    if (IoReg::Timer::contains(addr)) {
        component_write(timer_.get(), addr, value);
        return;
    }
    if (IoReg::Joypad::contains(addr)) {
        component_write(joypad_.get(), addr, value);
        return;
    }
    if (IoReg::Serial::contains(addr)) {
        component_write(serial_.get(), addr, value);
        return;
    }
    if (IoReg::Apu::contains(addr)) {
        component_write(apu_.get(), addr, value);
        return;
    }

    if (IoReg::Interrupts::contains(addr)) {
        common::log::trace(
            "[IO] Write to register {}: {}",
            (addr == IoReg::Interrupts::IE) ? "IE" : "IF",
            common::utils::PrettyHex(value).to_string()
        );
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
[[nodiscard]] const std::shared_ptr<Apu>& Io::apu() const
{
    return apu_;
}
[[nodiscard]] std::shared_ptr<Apu> Io::apu()
{
    return apu_;
}

[[nodiscard]] inline uint8_t Io::component_read(const IoComponent* comp, uint16_t addr) const
{
    if (comp == nullptr) {
        common::log::warn("Attempting to read from IO component with null pointer");
        return registers_.at(io_addr(addr));
    }

    return comp->read(addr);
}

inline void Io::component_write(IoComponent* comp, uint16_t addr, uint8_t value)
{
    if (comp == nullptr) {
        common::log::warn("Attempting to write to IO component with null pointer");
        registers_.at(io_addr(addr)) = value;
        return;
    }

    comp->write(addr, value);
}

} // namespace boyboy::core::io