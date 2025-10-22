/**
 * @file timer.h
 * @brief Timer operations for BoyBoy emulator.
 *
 * For information on how the timer works check the implementation, Pan Docs and/or The
 * Cycle-Accurate Game Boy Docs.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <array>
#include <cstdint>
#include <functional>

#include "boyboy/core/io/iocomponent.h"

namespace boyboy::core::io {

class Timer : public IoComponent {
public:
    void tick(uint16_t cycles) override;
    [[nodiscard]] uint8_t read(uint16_t addr) const override;
    void write(uint16_t addr, uint8_t value) override;
    void set_interrupt_cb(cpu::InterruptRequestCallback callback) override;
    void reset() override;

    // Start/stop the timer
    void start();
    void stop(); // Used by STOP instruction
    [[nodiscard]] bool is_stopped() const { return stopped_; }

    // Get timer frequency in number of cycles per increment
    [[nodiscard]] uint16_t get_frequency() const { return get_frequency(tac_); }
    [[nodiscard]] static constexpr uint16_t get_frequency(uint8_t tac)
    {
        return Frequency::ClockFrequencies.at(tac & Flags::ClockSelectMask);
    }

    // Get the system counter test bit of the falling edge detector
    // For internal use and testing
    [[nodiscard]] uint16_t get_test_bit() const { return get_test_bit(tac_); }
    [[nodiscard]] static constexpr uint16_t get_test_bit(uint8_t tac)
    {
        return DivBitLookup[tac & Flags::ClockSelectMask];
    }

    // Check if timer is enabled
    [[nodiscard]] bool is_enabled() const { return is_enabled(tac_); }
    [[nodiscard]] static bool is_enabled(uint8_t tac) { return (tac & Flags::TimerEnable) != 0; }

    struct Flags {
        static constexpr uint8_t ClockSelect0 = 0b001; // Bit 0 - Input Clock Select bit 0
        static constexpr uint8_t ClockSelect1 = 0b010; // Bit 1 - Input Clock Select bit 1
        static constexpr uint8_t TimerEnable = 0b100;  // Bit 2 - Timer Enable
        static constexpr uint8_t ClockSelectMask = ClockSelect0 | ClockSelect1;
        static constexpr uint8_t TacMask = ClockSelectMask | TimerEnable;

        static constexpr uint8_t Clock256M = 0b00; // 4096 Hz
        static constexpr uint8_t Clock4M = 0b01;   // 262144 Hz
        static constexpr uint8_t Clock16M = 0b10;  // 65536 Hz
        static constexpr uint8_t Clock64M = 0b11;  // 16384 Hz
    };

    struct Frequency {
        static constexpr uint16_t DivIncrement = 256; // DIV increments every 256 cycles
        static constexpr uint16_t Tima256M = 1024; // TIMA increments every 1024 cycles at 4096 Hz
        static constexpr uint16_t Tima4M = 16;     // TIMA increments every 16 cycles at 262144 Hz
        static constexpr uint16_t Tima16M = 64;    // TIMA increments every 64 cycles at 65536 Hz
        static constexpr uint16_t Tima64M = 256;   // TIMA increments every 256 cycles at 16384 Hz

        // Frequencies in number of cycles per increment
        static constexpr std::array<uint16_t, 4> ClockFrequencies = {1024, 16, 64, 256};
    };

    // Initial DIV values for DMG
    static constexpr uint16_t DivCounterStartValue = 0xABCC;
    static constexpr uint8_t DivStartValue = DivCounterStartValue >> 8;

    // Internal system counter bit lookup table for the falling edge detector
    static constexpr std::array<uint8_t, 4> DivBitLookup{9, 3, 5, 7};

    // Delay in T-cycles for scheduled operations
    static constexpr uint8_t InterruptDelay = 4;
    static constexpr uint8_t TimaReloadDelay = 4;

private:
    cpu::InterruptRequestCallback request_interrupt_;

    // Timer registers
    uint8_t tima_{0}; // Timer Counter
    uint8_t tma_{0};  // Timer Modulo
    uint8_t tac_{0};  // Timer Control

    // Internal counters
    uint16_t div_counter_{DivCounterStartValue}; // Internal system counter
    uint16_t tima_counter_{0};

    // Timer stopped flag
    bool stopped_{false};

    // TIMA overflow status
    bool tima_overflow_{false};

    // Schedulers for interrupt request and TIMA reload
    struct Scheduler {
        bool scheduled;
        uint8_t remaining;
        std::function<void(void)> execute;

        void schedule(uint8_t delay)
        {
            scheduled = true;
            remaining = delay;
        }

        void update(uint8_t cycles)
        {
            remaining -= std::min(remaining, cycles);
            if (remaining == 0 && execute != nullptr) {
                execute();
                scheduled = false;
            }
        }

        void reset()
        {
            scheduled = false;
            remaining = 0;
        }
    };
    Scheduler interrupt_scheduler_{
        .scheduled = false,
        .remaining = 0,
        .execute = [this]() { request_interrupt_(cpu::Interrupts::Timer); },
    };
    Scheduler tima_reload_scheduler_{
        .scheduled = false,
        .remaining = 0,
        .execute = [this]() { tima_ = tma_; },
    };

    void schedule_overflow()
    {
        interrupt_scheduler_.schedule(InterruptDelay);
        tima_reload_scheduler_.schedule(InterruptDelay + TimaReloadDelay);
        tima_overflow_ = false;
    }

    // Helpers for falling edge detection
    [[nodiscard]] static bool is_test_bit_set(uint16_t div_counter, uint8_t tac)
    {
        return (div_counter & (1 << get_test_bit(tac))) != 0;
    }
    [[nodiscard]] static bool test_bit_status(uint16_t div_counter, uint8_t tac)
    {
        return is_test_bit_set(div_counter, tac) && is_enabled(tac);
    }

    // Falling edge handling
    void handle_falling_edge(uint16_t old_div, uint16_t new_div, uint8_t old_tac, uint8_t new_tac)
    {
        bool prev_status = test_bit_status(old_div, old_tac);
        bool new_status = test_bit_status(new_div, new_tac);
        if (prev_status && !new_status) {
            tima_++;
            tima_overflow_ = tima_ == 0;
        }
    }

    // Registers setters to allow falling edge detection
    void increment_div_counter(uint16_t inc) { set_div_counter(div_counter_ + inc); }
    void set_div_counter(uint16_t div_counter)
    {
        handle_falling_edge(div_counter_, div_counter, tac_, tac_);
        div_counter_ = div_counter;
    }
    void set_tac(uint8_t tac)
    {
        tac &= Flags::TacMask;
        handle_falling_edge(div_counter_, div_counter_, tac_, tac);
        tac_ = tac;
    }
    void set_tima(uint8_t tima)
    {
        // If writing to TIMA while reload is scheduled but interrupt is already dispatched, write
        // is ignored
        if (tima_reload_scheduler_.scheduled && !interrupt_scheduler_.scheduled) {
            return;
        }
        // If writing to TIMA while the interrupt is scheduled, it is canceled
        if (interrupt_scheduler_.scheduled) {
            interrupt_scheduler_.scheduled = false;
            tima_reload_scheduler_.scheduled = false;
        }
        tima_ = tima;
    }
    void set_tma(uint8_t tma) { tma_ = tma; }
};

} // namespace boyboy::core::io