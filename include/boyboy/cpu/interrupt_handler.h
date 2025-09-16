/**
 * @file interrupt_handler.h
 * @brief CPU interrupt handling for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <array>
#include <cstdint>

#include "boyboy/cpu/cpu_constants.h"

namespace boyboy::mmu {
class Mmu;
}

namespace boyboy::cpu {

// Forward declarations
class Cpu;
// class Mmu;

class InterruptHandler {
public:
    InterruptHandler(Cpu& cpu, mmu::Mmu& mmu) : cpu_(cpu), mmu_(mmu) {}
    ~InterruptHandler() = default;

    // delete move and copy
    InterruptHandler(const InterruptHandler&) = delete;
    InterruptHandler& operator=(const InterruptHandler&) = delete;
    InterruptHandler(InterruptHandler&&) = delete;
    InterruptHandler& operator=(InterruptHandler&&) = delete;

    void service();
    void request(uint8_t interrupt);
    void enable(uint8_t interrupt);
    [[nodiscard]] bool is_requested(uint8_t interrupt) const;
    [[nodiscard]] bool is_enabled(uint8_t interrupt) const;

private:
    Cpu& cpu_;
    mmu::Mmu& mmu_;

    constexpr static std::array<uint16_t, 5> Vectors = {
        VBlankVector,
        LCDStatVector,
        TimerVector,
        SerialVector,
        JoypadVector,
    };

    void clear_interrupt(uint8_t interrupt);

    [[nodiscard]] uint8_t get_ie() const;
    void set_ie(uint8_t value);
    [[nodiscard]] uint8_t get_if() const;
    void set_if(uint8_t value);
};

} // namespace boyboy::cpu