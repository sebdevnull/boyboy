/**
 * @file timer.cpp
 * @brief Timer operations for BoyBoy emulator.
 *
 * On how the timer circuit works:
 * Basically, TIMA is incremented with the falling edge of a logic AND between TAC enable and
 * certain bits of the internal system counter based on TAC selected frequency.
 * This means two things:
 *     1) Any change in DIV, TAC.enable or TAC.freq that triggers the falling edge detector,
 *        will increase TIMA. i.e. TAC.enable && DIV(TAC.freq) 1 -> 0; TIMA++
 *     2) If resetting DIV before the tested bit is set, TIMA will never be incremented no matter
 *        how many ticks.
 *
 * For more information check Pan Docs and/or The Cycle-Accurate Game Boy Docs.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/core/io/timer.h"

#include <algorithm>
#include <cstdint>

#include "boyboy/core/io/constants.h"
#include "boyboy/core/io/registers.h"

namespace boyboy::core::io {

void Timer::init()
{
    // Assume DMG0
    tima_ = RegInitValues::Dmg0::Timer::TIMA;
    tma_ = RegInitValues::Dmg0::Timer::TMA;
    tac_ = RegInitValues::Dmg0::Timer::TAC;
    // TODO: DIV counter is off by 0x23 cycles on start, investigate
    div_counter_ = RegInitValues::Dmg0::Timer::DIVCounter + 0x23;
    tima_counter_ = 0;
    tima_overflow_scheduler_.reset();
    tima_reload_scheduler_.reset();
    stopped_ = false;
}

void Timer::reset()
{
    init();
}

void Timer::tick(uint16_t cycles)
{
    if (stopped_) {
        return;
    }

    // Increment at a rate of 4 T-cycles (1 M-cycle)
    while (cycles > 0) {

        auto cur_cycles = std::min(cycles, uint16_t{4});

        if (tima_overflow_scheduler_.scheduled) {
            tima_overflow_scheduler_.update(cur_cycles);
        }
        else if (tima_reload_scheduler_.scheduled) {
            tima_reload_scheduler_.update(cur_cycles);
        }

        // Update DIV counter
        increment_div_counter(cur_cycles);

        // Check if TIMA overflow happened and schedule
        if (tima_overflow_) {
            schedule_overflow();
        }

        cycles -= cur_cycles;
    }
}

uint8_t Timer::read(uint16_t addr) const
{
    switch (addr) {
        case IoReg::Timer::DIV:
            return div_counter_ >> 8;
        case IoReg::Timer::TIMA:
            return tima_;
        case IoReg::Timer::TMA:
            return tma_;
        case IoReg::Timer::TAC:
            return tac_;
        default:
            return 0xFF;
    }
}

void Timer::write(uint16_t addr, uint8_t value)
{
    switch (addr) {
        case IoReg::Timer::DIV:
            // writing any value resets the counter
            set_div_counter(0);
            break;
        case IoReg::Timer::TIMA:
            set_tima(value);
            break;
        case IoReg::Timer::TMA:
            set_tma(value);
            break;
        case IoReg::Timer::TAC:
            set_tac(value);
            break;
        default:
            break;
    }
}

void Timer::set_interrupt_cb(cpu::InterruptRequestCallback callback)
{
    request_interrupt_ = std::move(callback);
}

void Timer::start()
{
    stopped_ = false;
}

void Timer::stop()
{
    stopped_ = true;
    div_counter_ = 0;
}

} // namespace boyboy::core::io