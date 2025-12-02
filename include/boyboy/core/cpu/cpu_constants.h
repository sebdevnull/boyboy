/**
 * @file cpu_constants.h
 * @brief CPU constants for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstdint>

namespace boyboy::core::cpu {

// Initial values for registers after boot ROM
struct RegInitValues {
    struct Dmg0 {
        static constexpr uint16_t AF = 0x0100;
        static constexpr uint16_t BC = 0xFF13;
        static constexpr uint16_t DE = 0x00C1;
        static constexpr uint16_t HL = 0x8403;
        static constexpr uint16_t PC = 0x0100;
        static constexpr uint16_t SP = 0xFFFE;
    };
    struct Dmg {
        static constexpr uint16_t AF = 0x01B0; // assumes header cks != 0, otherwise 0x0180
        static constexpr uint16_t BC = 0x0013;
        static constexpr uint16_t DE = 0x00D8;
        static constexpr uint16_t HL = 0x014D;
        static constexpr uint16_t PC = 0x0100;
        static constexpr uint16_t SP = 0xFFFE;
    };
};

constexpr uint8_t CBInstructionPrefix = 0xCB;

constexpr uint16_t HighRAMOffset = 0xFF00;

constexpr uint64_t MasterClockFrequencyHz = 4'194'304;                  // 4.194304 MHz
constexpr uint64_t SystemClockFrequencyHz = MasterClockFrequencyHz / 4; // 1.048576 MHz

} // namespace boyboy::core::cpu