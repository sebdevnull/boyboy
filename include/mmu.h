/**
 * @file mmu.h
 * @brief Memory Management Unit (MMU) for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <span>

namespace boyboy::mmu {

/**
 *
  0000-3FFF   16KB ROM Bank 00     (in cartridge, fixed at bank 00)
  4000-7FFF   16KB ROM Bank 01..NN (in cartridge, switchable bank number)
  8000-9FFF   8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode)
  A000-BFFF   8KB External RAM     (in cartridge, switchable bank, if any)
  C000-CFFF   4KB Work RAM Bank 0 (WRAM)
  D000-DFFF   4KB Work RAM Bank 1 (WRAM)  (switchable bank 1-7 in CGB Mode)
  E000-FDFF   Same as C000-DDFF (ECHO)    (typically not used)
  FE00-FE9F   Sprite Attribute Table (OAM)
  FEA0-FEFF   Not Usable
  FF00-FF7F   I/O Ports
  FF80-FFFE   High RAM (HRAM)
  FFFF        Interrupt Enable Register
 */

class Mmu {
public:
    uint8_t read_byte(uint16_t address);
    uint16_t read_word(uint16_t address);
    void write_byte(uint16_t address, uint8_t value);
    void write_word(uint16_t address, uint16_t value);
    void copy(uint16_t dst_address, std::span<uint8_t> src);

private:
    struct MemoryRegion {
        uint16_t start;
        uint16_t end;
        std::span<uint8_t> data;
    };

    // --- MMU memory map constants ---
    static constexpr uint16_t kROMBank0Start = 0x0000;
    static constexpr uint16_t kROMBank0End = 0x3FFF;
    static constexpr uint16_t kROMBank1Start = 0x4000;
    static constexpr uint16_t kROMBank1End = 0x7FFF;
    static constexpr uint16_t kROMStart = kROMBank0Start;
    static constexpr uint16_t kROMEnd = kROMBank1End;

    static constexpr uint16_t kVRAMStart = 0x8000;
    static constexpr uint16_t kVRAMEnd = 0x9FFF;

    static constexpr uint16_t kERAMStart = 0xA000;
    static constexpr uint16_t kERAMEnd = 0xBFFF;

    static constexpr uint16_t kWRAM0Start = 0xC000;
    static constexpr uint16_t kWRAM0End = 0xCFFF;
    static constexpr uint16_t kWRAM1Start = 0xD000;
    static constexpr uint16_t kWRAM1End = 0xDFFF;
    static constexpr uint16_t kWRAMStart = kWRAM0Start;
    static constexpr uint16_t kWRAMEnd = kWRAM1End;

    static constexpr uint16_t kECHOStart = 0xE000;
    static constexpr uint16_t kECHOEnd = 0xFDFF;

    static constexpr uint16_t kOAMStart = 0xFE00;
    static constexpr uint16_t kOAMEnd = 0xFE9F;

    static constexpr uint16_t kNotUsableStart = 0xFEA0;
    static constexpr uint16_t kNotUsableEnd = 0xFEFF;

    static constexpr uint16_t kIOStart = 0xFF00;
    static constexpr uint16_t kIOEnd = 0xFF7F;

    static constexpr uint16_t kHRAMStart = 0xFF80;
    static constexpr uint16_t kHRAMEnd = 0xFFFE;

    static constexpr uint16_t kIEAddr = 0xFFFF;

    // --- MMU memory bank sizes ---
    static constexpr size_t kROMBank0Size = kROMBank0End - kROMBank0Start + 1;
    static constexpr size_t kROMBank1Size = kROMBank1End - kROMBank1Start + 1;
    static constexpr size_t kROMBankSize = kROMBank0Size + kROMBank1Size;
    static constexpr size_t kVRAMSize = kVRAMEnd - kVRAMStart + 1;
    static constexpr size_t kERAMSize = kERAMEnd - kERAMStart + 1;
    static constexpr size_t kWRAM0Size = kWRAM0End - kWRAM0Start + 1;
    static constexpr size_t kWRAM1Size = kWRAM1End - kWRAM1Start + 1;
    static constexpr size_t kWRAMSize = kWRAM0Size + kWRAM1Size;
    static constexpr size_t kOAMSize = kOAMEnd - kOAMStart + 1;
    static constexpr size_t kIOSize = kIOEnd - kIOStart + 1;
    static constexpr size_t kHRAMSize = kHRAMEnd - kHRAMStart + 1;

    // Number of memory regions
    static constexpr size_t kNumRegions = 10;

    // memory banks
    std::array<uint8_t, kROMBankSize> cart{}; // cartridge
    std::array<uint8_t, kVRAMSize> vram{};    // video ram
    std::array<uint8_t, kERAMSize> eram{};    // external ram
    std::array<uint8_t, kWRAMSize> wram{};    // work ram
    std::array<uint8_t, kOAMSize> oam{};      // sprite attribute table
    std::array<uint8_t, kIOSize> io{};        // i/o ports
    std::array<uint8_t, kHRAMSize> hram{};    // high ram
    uint8_t ie{};                             // interrupt enable register

    // memory region table for easier address deduction
    std::array<MemoryRegion, kNumRegions> region_table = {
        // clang-format off
        MemoryRegion{.start = kROMBank0Start, .end = kROMBank0End, .data = cart},
        MemoryRegion{.start = kROMBank1Start, .end = kROMBank1End, .data = std::span<uint8_t>(cart).subspan(kROMBank0Size)},
        MemoryRegion{.start = kVRAMStart, .end = kVRAMEnd, .data = vram},
        MemoryRegion{.start = kERAMStart, .end = kERAMEnd, .data = eram},
        MemoryRegion{.start = kWRAM0Start, .end = kWRAM0End, .data = wram},
        MemoryRegion{.start = kWRAM1Start, .end = kWRAM1End, .data = std::span<uint8_t>(wram).subspan(kWRAM0Size)},
        MemoryRegion{.start = kOAMStart, .end = kOAMEnd, .data = oam},
        MemoryRegion{.start = kIOStart, .end = kIOEnd, .data = io},
        MemoryRegion{.start = kHRAMStart, .end = kHRAMEnd, .data = hram},
        MemoryRegion{.start = kIEAddr, .end = kIEAddr, .data = {&ie, 1}}
        // clang-format on
    };

    uint8_t* get_pointer(uint16_t address);
};

} // namespace boyboy::mmu