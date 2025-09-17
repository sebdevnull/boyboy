/**
 * @file io.h
 * @brief Input/Output operations for BoyBoy emulator.
 *
 * For now the only thing the module does is redirecting serial output to a stream.
 * Other writes are ignored and reads return 0xFF.
 * Used for debugging and testing purposes.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <ostream>

#include "boyboy/io/timer.h"
#include "boyboy/mmu_constants.h"

namespace boyboy::io {

// I/O registers
struct IoReg {
    struct Joypad {
        static constexpr uint16_t P1 = 0xFF00; // Joypad

        static bool contains(uint16_t addr) { return addr == P1; }
    };

    struct Serial {
        static constexpr uint16_t SB = 0xFF01; // Serial Data
        static constexpr uint16_t SC = 0xFF02; // Serial Control

        static bool contains(uint16_t addr) { return addr == SB || addr == SC; }
    };

    struct Timer {
        static constexpr uint16_t DIV = 0xFF04;  // Divider Register
        static constexpr uint16_t TIMA = 0xFF05; // Timer Counter
        static constexpr uint16_t TMA = 0xFF06;  // Timer Modulo
        static constexpr uint16_t TAC = 0xFF07;  // Timer Control

        static bool contains(uint16_t addr) { return addr >= DIV && addr <= TAC; }
    };

    struct Sound {
        static constexpr uint16_t NR10 = 0xFF10; // Channel 1 Sweep register
        static constexpr uint16_t NR11 = 0xFF11; // Channel 1 Sound length/Wave pattern duty
        static constexpr uint16_t NR12 = 0xFF12; // Channel 1 Volume Envelope
        static constexpr uint16_t NR13 = 0xFF13; // Channel 1 Frequency lo
        static constexpr uint16_t NR14 = 0xFF14; // Channel 1 Frequency hi
        static constexpr uint16_t NR21 = 0xFF16; // Channel 2 Sound Length/Wave Pattern Duty
        static constexpr uint16_t NR22 = 0xFF17; // Channel 2 Volume Envelope
        static constexpr uint16_t NR23 = 0xFF18; // Channel 2 Frequency lo
        static constexpr uint16_t NR24 = 0xFF19; // Channel 2 Frequency hi
        static constexpr uint16_t NR30 = 0xFF1A; // Channel 3 Sound on/off
        static constexpr uint16_t NR31 = 0xFF1B; // Channel 3 Sound Length
        static constexpr uint16_t NR32 = 0xFF1C; // Channel 3 Select output level
        static constexpr uint16_t NR33 = 0xFF1D; // Channel 3 Frequency's lower data
        static constexpr uint16_t NR34 = 0xFF1E; // Channel 3 Frequency's higher data
        static constexpr uint16_t NR41 = 0xFF20; // Channel 4 Sound Length
        static constexpr uint16_t NR42 = 0xFF21; // Channel 4 Volume Envelope
        static constexpr uint16_t NR43 = 0xFF22; // Channel 4 Polynomial Counter
        static constexpr uint16_t NR44 = 0xFF23; // Channel 4 Counter/Consecutive; Initial
        static constexpr uint16_t NR50 = 0xFF24; // Channel control / ON-OFF / Volume
        static constexpr uint16_t NR51 = 0xFF25; // Selection of Sound output terminal
        static constexpr uint16_t NR52 = 0xFF26; // Sound on/off

        static bool contains(uint16_t addr) { return addr >= NR10 && addr <= NR52; }
    };

    struct Lcd {
        static constexpr uint16_t LCDC = 0xFF40; // LCD Control
        static constexpr uint16_t STAT = 0xFF41; // LCDC Status
        static constexpr uint16_t SCY = 0xFF42;  // Scroll Y
        static constexpr uint16_t SCX = 0xFF43;  // Scroll X
        static constexpr uint16_t LY = 0xFF44;   // LCDC Y-Coordinate
        static constexpr uint16_t LYC = 0xFF45;  // LY Compare
        static constexpr uint16_t DMA = 0xFF46;  // DMA Transfer and Start Address
        static constexpr uint16_t BGP = 0xFF47;  // BG Palette Data
        static constexpr uint16_t OBP0 = 0xFF48; // Object Palette 0 Data
        static constexpr uint16_t OBP1 = 0xFF49; // Object Palette 1 Data
        static constexpr uint16_t WY = 0xFF4A;   // Window Y Position
        static constexpr uint16_t WX = 0xFF4B;   // Window X Position minus 7

        static bool contains(uint16_t addr) { return addr >= LCDC && addr <= WX; }
    };

    struct Interrupts {
        static constexpr uint16_t IF = 0xFF0F; // Interrupt Flag
        static constexpr uint16_t IE = 0xFFFF; // Interrupt Enable

        static bool contains(uint16_t addr) { return addr == IF || addr == IE; }
    };
};

class Io {
public:
    Io(std::ostream& out = std::cout) : serial_out_(out) {}
    ~Io() = default;

    // Delete move and copy
    Io(const Io&) = delete;
    Io& operator=(const Io&) = delete;
    Io(Io&&) = delete;
    Io& operator=(Io&&) = delete;

    void tick(uint16_t cycles);

    // Read/write I/O registers
    [[nodiscard]] uint8_t read(uint16_t addr) const;
    void write(uint16_t addr, uint8_t value);

    // Components accessors
    [[nodiscard]] const Timer& timer() const { return timer_; }
    [[nodiscard]] Timer& timer() { return timer_; }

    [[nodiscard]] const std::ostream& get_serial_stream() const { return serial_out_; }

private:
    Timer timer_;

    std::array<uint8_t, mmu::IOSize> registers_{};
    std::ostream& serial_out_;

    [[nodiscard]] static uint8_t io_addr(uint16_t addr)
    {
        return static_cast<uint8_t>(addr - mmu::IOStart);
    }
};

} // namespace boyboy::io