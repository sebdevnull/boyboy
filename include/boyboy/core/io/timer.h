/**
 * @file timer.h
 * @brief Timer operations for BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <array>
#include <cstdint>

#include "boyboy/io/iocomponent.h"

namespace boyboy::io {

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
    [[nodiscard]] uint16_t get_frequency() const
    {
        return Frequency::ClockFrequencies.at(tac_ & Flags::ClockSelectMask);
    }
    [[nodiscard]] static uint16_t get_frequency(uint8_t tac)
    {
        return Frequency::ClockFrequencies.at(tac & Flags::ClockSelectMask);
    }

    struct Flags {
        static constexpr uint8_t ClockSelect0 = 0b001; // Bit 0 - Input Clock Select bit 0
        static constexpr uint8_t ClockSelect1 = 0b010; // Bit 1 - Input Clock Select bit 1
        static constexpr uint8_t TimerEnable = 0b100;  // Bit 2 - Timer Enable
        static constexpr uint8_t ClockSelectMask = ClockSelect0 | ClockSelect1;

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

private:
    cpu::InterruptRequestCallback request_interrupt_;

    // Timer registers
    uint8_t div_{0};  // Divider Register
    uint8_t tima_{0}; // Timer Counter
    uint8_t tma_{0};  // Timer Modulo
    uint8_t tac_{0};  // Timer Control

    // Internal counters
    uint16_t div_counter_{0};
    uint16_t tima_counter_{0};

    // Timer stopped flag
    bool stopped_{false};
};

} // namespace boyboy::io