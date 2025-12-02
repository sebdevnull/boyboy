/**
 * @file interrupts.h
 * @brief CPU interrupts definitions for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <utility>

namespace boyboy::core::cpu {

/**
 * @brief CPU Interrupts.
 *
 */
enum class Interrupt : uint8_t {
    VBlank = 1 << 0,
    LCDStat = 1 << 1,
    Timer = 1 << 2,
    Serial = 1 << 3,
    Joypad = 1 << 4,
};

/**
 * @brief CPU Interrupt Vectors.
 *
 */
enum class InterruptVector : uint8_t {
    VBlank = 0x40,
    LCDStat = 0x48,
    Timer = 0x50,
    Serial = 0x58,
    Joypad = 0x60,
};

/**
 * @brief Mapping of Interrupts to their Vectors.
 *
 */
struct InterruptVectors {
    static constexpr std::array<InterruptVector, 5> Vectors = {
        InterruptVector::VBlank,
        InterruptVector::LCDStat,
        InterruptVector::Timer,
        InterruptVector::Serial,
        InterruptVector::Joypad,
    };

    static constexpr InterruptVector get_vector(Interrupt interrupt)
    {
        int bit_pos = 0;
        auto interrupt_mask = std::to_underlying(interrupt);
        while ((interrupt_mask & 1) == 0) {
            interrupt_mask >>= 1;
            bit_pos++;
        }

        return Vectors.at(bit_pos);
    }
};

// Interrupt request callback
using InterruptRequestCallback = std::function<void(Interrupt)>;

// String conversions

inline static const char* to_string(Interrupt interrupt)
{
    switch (interrupt) {
        case Interrupt::VBlank:
            return "VBlank";
        case Interrupt::LCDStat:
            return "LCDStat";
        case Interrupt::Timer:
            return "Timer";
        case Interrupt::Serial:
            return "Serial";
        case Interrupt::Joypad:
            return "Joypad";
        default:
            return "Unknown";
    }
}

} // namespace boyboy::core::cpu