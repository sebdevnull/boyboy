/**
 * @file interrupts.cpp
 * @brief CPU interrupt handling for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/core/cpu/interrupt_handler.h"

#include <cstdint>
#include <utility>

#include "boyboy/core/cpu/cpu.h"
#include "boyboy/core/cpu/cycles.h"
#include "boyboy/core/cpu/interrupts.h"
#include "boyboy/core/io/registers.h"

namespace boyboy::core::cpu {

constexpr auto IF = boyboy::core::io::IoReg::Interrupts::IF;
constexpr auto IE = boyboy::core::io::IoReg::Interrupts::IE;

void InterruptHandler::tick(Cycles cycles)
{
    // Tick until we can service the interrupt.
    // It could be done in multiple cycles stages (push PC, jump), but since the CPU is locked
    // servicing the interrupt it doesn't (or shouldn't) matter.
    if (is_servicing()) {
        cycles_left_ -= to_tcycles(cycles);
        if (cycles_left_ > 0) {
            return;
        }
        service_interrupt(current_interrupt_);
    }

    // Don't do anything if interrupts are disabled
    if (!cpu_.get_ime()) {
        return;
    }

    // If not servicing check if there's any interrupt pending and schedule
    if (auto pend = pending(); pend != 0) {
        for (size_t i = 0; i < InterruptVectors::Vectors.size(); ++i) {
            auto interrupt = 1 << i;
            if ((pend & interrupt) == 0) {
                continue;
            }

            cycles_left_ = service_cycles() - to_tcycles(cycles);
            current_interrupt_ = static_cast<Interrupt>(interrupt);

            // Disable interrupts, clear IF and wake CPU if halted
            cpu_.set_ime(false);
            clear_interrupt(current_interrupt_);
            cpu_.set_halted(false);
        }
    }
}

TCycle InterruptHandler::service()
{
    if (!cpu_.get_ime()) {
        return 0;
    }

    uint8_t ier = get_ie();
    uint8_t ifr = get_if();
    uint8_t pending = ier & ifr;

    if (pending == 0) {
        return 0;
    }

    const auto ServiceCycles = service_cycles();

    // Disable interrupts and wake CPU if halted
    cpu_.set_ime(false);
    cpu_.set_halted(false);

    uint8_t cycles = 0;
    for (size_t i = 0; i < InterruptVectors::Vectors.size(); ++i) {
        uint8_t mask = (1 << i);
        if ((pending & mask) != 0) {
            // Clear IF flag
            ifr &= ~mask;
            set_if(ifr);
            service_interrupt(static_cast<Interrupt>(mask));
            cycles = ServiceCycles;
            break;
        }
    }

    return cycles;
}

inline void InterruptHandler::service_interrupt(Interrupt interrupt)
{
    cpu_.push_pc();
    cpu_.set_pc(std::to_underlying(InterruptVectors::get_vector(interrupt)));
}

void InterruptHandler::request(Interrupt interrupt)
{
    uint8_t ifr = get_if();
    ifr |= std::to_underlying(interrupt);
    set_if(ifr);
}

void InterruptHandler::enable(Interrupt interrupt)
{
    uint8_t ie = get_ie();
    ie |= std::to_underlying(interrupt);
    set_ie(ie);
}

bool InterruptHandler::is_requested(Interrupt interrupt) const
{
    uint8_t ifr = get_if();
    return (ifr & std::to_underlying(interrupt)) != 0;
}

bool InterruptHandler::is_enabled(Interrupt interrupt) const
{
    uint8_t ie = get_ie();
    return (ie & std::to_underlying(interrupt)) != 0;
}

uint8_t InterruptHandler::pending() const
{
    uint8_t ie = get_ie();
    uint8_t ifr = get_if();
    return ie & ifr;
}

bool InterruptHandler::should_service() const
{
    return cpu_.get_ime() && (pending() != 0);
}

inline void InterruptHandler::clear_interrupt(Interrupt interrupt)
{
    uint8_t ifr = get_if();
    ifr &= ~std::to_underlying(interrupt);
    set_if(ifr);
}

uint8_t InterruptHandler::get_ie() const
{
    return cpu_.read_byte(IE);
}

uint8_t InterruptHandler::get_if() const
{
    return cpu_.read_byte(IF);
}

inline void InterruptHandler::set_ie(uint8_t value)
{
    cpu_.write_byte(IE, value);
}

inline void InterruptHandler::set_if(uint8_t value)
{
    cpu_.write_byte(IF, value);
}

/**
 * @brief Calculate the number of cycles needed to service an interrupt.
 *
 * It takes 20 T-cycles to handle an interrupt and another extra 4 T-cycles if the CPU is halted.
 *
 * @return TCycle Number of T-cycles required to service the interrupt.
 */
inline TCycle InterruptHandler::service_cycles() const
{
    auto cycles = InterruptServiceCycles;
    if (cpu_.is_halted()) {
        cycles += 4;
    }

    return cycles;
}

} // namespace boyboy::core::cpu