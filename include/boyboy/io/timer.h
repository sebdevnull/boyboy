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

    void start();
    void stop(); // Used by STOP instruction
    [[nodiscard]] bool is_stopped() const { return stopped_; }

private:
    struct Flags {
        static constexpr uint8_t TimerEnable = 0x04;  // Bit 2 - Timer Enable
        static constexpr uint8_t ClockSelect0 = 0x01; // Bit 0 - Input Clock Select bit 0
        static constexpr uint8_t ClockSelect1 = 0x02; // Bit 1 - Input Clock Select bit 1
        static constexpr uint8_t ClockSelectMask = ClockSelect0 | ClockSelect1;

        static constexpr uint8_t Clock256M = 0x00; // 4096 Hz
        static constexpr uint8_t Clock4M = 0x01;   // 262144 Hz
        static constexpr uint8_t Clock16M = 0x02;  // 65536 Hz
        static constexpr uint8_t Clock64M = 0x03;  // 16384 Hz

        // Frequencies in number of cycles per increment
        static constexpr std::array<uint16_t, 4> ClockFrequencies = {1024, 16, 64, 256};
    };

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