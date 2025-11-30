/**
 * @file constants.h
 * @brief I/O constants for BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstdint>

namespace boyboy::core::io {

struct RegInitValues {
    struct Dmg0 {
        struct Joypad {
            static constexpr uint8_t P1 = 0xCF;
        };
        struct Serial {
            static constexpr uint8_t SB = 0x00;
            static constexpr uint8_t SC = 0x7E;
        };
        struct Timer {
            // Full timer internal counter, where DIV is the 8 upper bits
            static constexpr uint16_t DIVCounter = 0x180D;
            static constexpr uint8_t TIMA = 0x00;
            static constexpr uint8_t TMA = 0x00;
            static constexpr uint8_t TAC = 0xF8;
        };
        struct Apu {
            static constexpr uint8_t NR10 = 0x80;
            static constexpr uint8_t NR11 = 0xBF;
            static constexpr uint8_t NR12 = 0xF3;
            static constexpr uint8_t NR13 = 0xFF;
            static constexpr uint8_t NR14 = 0xBF;
            static constexpr uint8_t NR21 = 0x3F;
            static constexpr uint8_t NR22 = 0x00;
            static constexpr uint8_t NR23 = 0xFF;
            static constexpr uint8_t NR24 = 0xBF;
            static constexpr uint8_t NR30 = 0x7F;
            static constexpr uint8_t NR31 = 0xFF;
            static constexpr uint8_t NR32 = 0x9F;
            static constexpr uint8_t NR33 = 0xFF;
            static constexpr uint8_t NR34 = 0xBF;
            static constexpr uint8_t NR41 = 0xFF;
            static constexpr uint8_t NR42 = 0x00;
            static constexpr uint8_t NR43 = 0x00;
            static constexpr uint8_t NR44 = 0xBF;
            static constexpr uint8_t NR50 = 0x77;
            static constexpr uint8_t NR51 = 0xF3;
            static constexpr uint8_t NR52 = 0xF1;
        };
        struct Ppu {
            static constexpr uint8_t LCDC = 0x91;
            static constexpr uint8_t STAT = 0x81;
            static constexpr uint8_t SCY = 0x00;
            static constexpr uint8_t SCX = 0x00;
            static constexpr uint8_t LY = 0x91;
            static constexpr uint8_t LYC = 0x00;
            static constexpr uint8_t DMA = 0xFF;
            static constexpr uint8_t BGP = 0xFC;
            static constexpr uint8_t OBP0 = 0x00; // Left uninitialized in hw
            static constexpr uint8_t OBP1 = 0x00; // Left uninitialized in hw
            static constexpr uint8_t WY = 0x00;
            static constexpr uint8_t WX = 0x00;
        };
        struct Interrupts {
            static constexpr uint8_t IF = 0xE1;
            static constexpr uint8_t IE = 0x00;
        };
    };

    struct Dmg {
        struct Joypad {
            static constexpr uint8_t P1 = 0xCF;
        };
        struct Serial {
            static constexpr uint8_t SB = 0x00;
            static constexpr uint8_t SC = 0x7E;
        };
        struct Timer {
            // Full timer internal counter, where DIV is the 8 upper bits
            static constexpr uint16_t DIVCounter = 0xAB34;
            static constexpr uint8_t TIMA = 0x00;
            static constexpr uint8_t TMA = 0x00;
            static constexpr uint8_t TAC = 0xF8;
        };
        struct Apu {
            static constexpr uint8_t NR10 = 0x80;
            static constexpr uint8_t NR11 = 0xBF;
            static constexpr uint8_t NR12 = 0xF3;
            static constexpr uint8_t NR13 = 0xFF;
            static constexpr uint8_t NR14 = 0xBF;
            static constexpr uint8_t NR21 = 0x3F;
            static constexpr uint8_t NR22 = 0x00;
            static constexpr uint8_t NR23 = 0xFF;
            static constexpr uint8_t NR24 = 0xBF;
            static constexpr uint8_t NR30 = 0x7F;
            static constexpr uint8_t NR31 = 0xFF;
            static constexpr uint8_t NR32 = 0x9F;
            static constexpr uint8_t NR33 = 0xFF;
            static constexpr uint8_t NR34 = 0xBF;
            static constexpr uint8_t NR41 = 0xFF;
            static constexpr uint8_t NR42 = 0x00;
            static constexpr uint8_t NR43 = 0x00;
            static constexpr uint8_t NR44 = 0xBF;
            static constexpr uint8_t NR50 = 0x77;
            static constexpr uint8_t NR51 = 0xF3;
            static constexpr uint8_t NR52 = 0xF1;
        };
        struct Ppu {
            static constexpr uint8_t LCDC = 0x91;
            static constexpr uint8_t STAT = 0x85;
            static constexpr uint8_t SCY = 0x00;
            static constexpr uint8_t SCX = 0x00;
            static constexpr uint8_t LY = 0x00;
            static constexpr uint8_t LYC = 0x00;
            static constexpr uint8_t DMA = 0xFF;
            static constexpr uint8_t BGP = 0xFC;
            static constexpr uint8_t OBP0 = 0x00; // Left uninitialized in hw
            static constexpr uint8_t OBP1 = 0x00; // Left uninitialized in hw
            static constexpr uint8_t WY = 0x00;
            static constexpr uint8_t WX = 0x00;
        };
        struct Interrupts {
            static constexpr uint8_t IF = 0xE1;
            static constexpr uint8_t IE = 0x00;
        };
    };
};

} // namespace boyboy::core::io