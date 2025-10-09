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

namespace boyboy::core::cpu {

struct Interrupts {
    static constexpr uint8_t VBlank = (1 << 0);
    static constexpr uint8_t LCDStat = (1 << 1);
    static constexpr uint8_t Timer = (1 << 2);
    static constexpr uint8_t Serial = (1 << 3);
    static constexpr uint8_t Joypad = (1 << 4);
};

struct InterruptVectors {
    static constexpr uint8_t VBlank = 0x40;
    static constexpr uint8_t LCDStat = 0x48;
    static constexpr uint8_t Timer = 0x50;
    static constexpr uint8_t Serial = 0x58;
    static constexpr uint8_t Joypad = 0x60;

    static constexpr std::array<uint16_t, 5> Vectors = {
        VBlank,
        LCDStat,
        Timer,
        Serial,
        Joypad,
    };
};

using InterruptRequestCallback = std::function<void(uint8_t)>;

} // namespace boyboy::core::cpu