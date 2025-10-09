/**
 * @file interrupts.cpp
 * @brief CPU interrupt handling for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/core/cpu/interrupt_handler.h"

#include <cstdint>

#include "boyboy/core/cpu/cpu.h"
#include "boyboy/core/cpu/interrupts.h"

namespace boyboy::core::cpu {

constexpr auto IF = boyboy::core::io::IoReg::Interrupts::IF;
constexpr auto IE = boyboy::core::io::IoReg::Interrupts::IE;

void InterruptHandler::service()
{
    if (!cpu_.get_ime()) {
        return;
    }

    uint8_t ier = get_ie();
    uint8_t ifr = get_if();
    uint8_t pending = ier & ifr;

    if (pending == 0) {
        return;
    }

    // Disable interrupts and wake CPU if halted
    cpu_.set_ime(false);
    cpu_.set_halted(false);

    for (size_t i = 0; i < InterruptVectors::Vectors.size(); ++i) {
        uint8_t mask = (1 << i);
        if ((pending & mask) != 0) {
            // Clear IF flag
            ifr &= ~mask;
            set_if(ifr);
            // Push PC to stack
            cpu_.push_pc();
            // Jump to interrupt vector
            cpu_.set_pc(InterruptVectors::Vectors.at(i));
            // Interrupt handling takes 5 machine cycles
            cpu_.add_cycles(5 * 4);
            break;
        }
    }
}

void InterruptHandler::request(uint8_t interrupt)
{
    uint8_t ifr = get_if();
    ifr |= interrupt;
    set_if(ifr);
}

void InterruptHandler::enable(uint8_t interrupt)
{
    uint8_t ie = get_ie();
    ie |= interrupt;
    set_ie(ie);
}

bool InterruptHandler::is_requested(uint8_t interrupt) const
{
    uint8_t ifr = get_if();
    return (ifr & interrupt) != 0;
}

bool InterruptHandler::is_enabled(uint8_t interrupt) const
{
    uint8_t ie = get_ie();
    return (ie & interrupt) != 0;
}

uint8_t InterruptHandler::pending() const
{
    uint8_t ie = get_ie();
    uint8_t ifr = get_if();
    return ie & ifr;
}

void InterruptHandler::clear_interrupt(uint8_t interrupt)
{
    uint8_t ifr = get_if();
    ifr &= ~interrupt;
    set_if(ifr);
}

uint8_t InterruptHandler::get_ie() const
{
    return cpu_.read_byte(IE);
}

void InterruptHandler::set_ie(uint8_t value)
{
    cpu_.write_byte(IE, value);
}

uint8_t InterruptHandler::get_if() const
{
    return cpu_.read_byte(IF);
}

void InterruptHandler::set_if(uint8_t value)
{
    cpu_.write_byte(IF, value);
}

} // namespace boyboy::core::cpu