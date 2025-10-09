/**
 * @file timer.cpp
 * @brief Timer operations for BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/core/io/timer.h"

#include "boyboy/core/io/registers.h"

namespace boyboy::core::io {

void Timer::tick(uint16_t cycles)
{
    // Update DIV register (if not stopped)
    if (!stopped_) {
        div_counter_ += cycles;
        if (div_counter_ >= Frequency::DivIncrement) {
            div_ += div_counter_ / Frequency::DivIncrement;
            div_counter_ %= Frequency::DivIncrement;
        }
    }

    // Check if timer is enabled
    if ((tac_ & Flags::TimerEnable) != 0) {
        // Determine the frequency
        uint16_t freq = get_frequency();

        // Update TIMA register
        tima_counter_ += cycles;
        while (tima_counter_ >= freq) {
            tima_counter_ -= freq;
            if (tima_ == 0xFF) {
                tima_ = tma_; // Reset TIMA to TMA on overflow
                request_interrupt_(cpu::Interrupts::Timer);
            }
            else {
                tima_++;
            }
        }
    }
}

uint8_t Timer::read(uint16_t addr) const
{
    switch (addr) {
        case IoReg::Timer::DIV:
            return div_;
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
            div_ = 0; // writing any value resets DIV
            break;
        case IoReg::Timer::TIMA:
            tima_ = value;
            break;
        case IoReg::Timer::TMA:
            tma_ = value;
            break;
        case IoReg::Timer::TAC:
            tac_ = value & 0x07; // only lower 3 bits are used
            break;
        default:
            break;
    }
}

void Timer::set_interrupt_cb(cpu::InterruptRequestCallback callback)
{
    request_interrupt_ = std::move(callback);
}

void Timer::reset()
{
    div_ = 0;
    tima_ = 0;
    tma_ = 0;
    tac_ = 0;
    div_counter_ = 0;
    tima_counter_ = 0;
    stopped_ = false;
}

void Timer::start()
{
    stopped_ = false;
}

void Timer::stop()
{
    stopped_ = true;
    div_ = 0;
    div_counter_ = 0;
}

} // namespace boyboy::core::io