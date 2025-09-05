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
    static constexpr uint16_t ROMBank0Start = 0x0000;
    static constexpr uint16_t ROMBank0End = 0x3FFF;
    static constexpr uint16_t ROMBank1Start = 0x4000;
    static constexpr uint16_t ROMBank1End = 0x7FFF;
    static constexpr uint16_t ROMStart = ROMBank0Start;
    static constexpr uint16_t ROMEnd = ROMBank1End;

    static constexpr uint16_t VRAMStart = 0x8000;
    static constexpr uint16_t VRAMEnd = 0x9FFF;

    static constexpr uint16_t ERAMStart = 0xA000;
    static constexpr uint16_t ERAMEnd = 0xBFFF;

    static constexpr uint16_t WRAM0Start = 0xC000;
    static constexpr uint16_t WRAM0End = 0xCFFF;
    static constexpr uint16_t WRAM1Start = 0xD000;
    static constexpr uint16_t WRAM1End = 0xDFFF;
    static constexpr uint16_t WRAMStart = WRAM0Start;
    static constexpr uint16_t WRAMEnd = WRAM1End;

    static constexpr uint16_t ECHOStart = 0xE000;
    static constexpr uint16_t ECHOEnd = 0xFDFF;

    static constexpr uint16_t OAMStart = 0xFE00;
    static constexpr uint16_t OAMEnd = 0xFE9F;

    static constexpr uint16_t NotUsableStart = 0xFEA0;
    static constexpr uint16_t NotUsableEnd = 0xFEFF;

    static constexpr uint16_t IOStart = 0xFF00;
    static constexpr uint16_t IOEnd = 0xFF7F;

    static constexpr uint16_t HRAMStart = 0xFF80;
    static constexpr uint16_t HRAMEnd = 0xFFFE;

    static constexpr uint16_t IEAddr = 0xFFFF;

    // --- MMU memory bank sizes ---
    static constexpr size_t ROMBank0Size = ROMBank0End - ROMBank0Start + 1;
    static constexpr size_t ROMBank1Size = ROMBank1End - ROMBank1Start + 1;
    static constexpr size_t ROMBankSize = ROMBank0Size + ROMBank1Size;
    static constexpr size_t VRAMSize = VRAMEnd - VRAMStart + 1;
    static constexpr size_t ERAMSize = ERAMEnd - ERAMStart + 1;
    static constexpr size_t WRAM0Size = WRAM0End - WRAM0Start + 1;
    static constexpr size_t WRAM1Size = WRAM1End - WRAM1Start + 1;
    static constexpr size_t WRAMSize = WRAM0Size + WRAM1Size;
    static constexpr size_t OAMSize = OAMEnd - OAMStart + 1;
    static constexpr size_t IOSize = IOEnd - IOStart + 1;
    static constexpr size_t HRAMSize = HRAMEnd - HRAMStart + 1;

    // Number of memory regions
    static constexpr size_t NumRegions = 10;

    // memory banks
    std::array<uint8_t, ROMBankSize> cart_{}; // cartridge
    std::array<uint8_t, VRAMSize> vram_{};    // video ram
    std::array<uint8_t, ERAMSize> eram_{};    // external ram
    std::array<uint8_t, WRAMSize> wram_{};    // work ram
    std::array<uint8_t, OAMSize> oam_{};      // sprite attribute table
    std::array<uint8_t, IOSize> ior_{};       // i/o registers
    std::array<uint8_t, HRAMSize> hram_{};    // high ram
    uint8_t ier_{};                           // interrupt enable register

    // memory region table for easier address deduction
    std::array<MemoryRegion, NumRegions> region_table_ = {
        // clang-format off
        MemoryRegion{.start = ROMBank0Start, .end = ROMBank0End, .data = cart_},
        MemoryRegion{.start = ROMBank1Start, .end = ROMBank1End, .data = std::span<uint8_t>(cart_).subspan(ROMBank0Size)},
        MemoryRegion{.start = VRAMStart, .end = VRAMEnd, .data = vram_},
        MemoryRegion{.start = ERAMStart, .end = ERAMEnd, .data = eram_},
        MemoryRegion{.start = WRAM0Start, .end = WRAM0End, .data = wram_},
        MemoryRegion{.start = WRAM1Start, .end = WRAM1End, .data = std::span<uint8_t>(wram_).subspan(WRAM0Size)},
        MemoryRegion{.start = OAMStart, .end = OAMEnd, .data = oam_},
        MemoryRegion{.start = IOStart, .end = IOEnd, .data = ior_},
        MemoryRegion{.start = HRAMStart, .end = HRAMEnd, .data = hram_},
        MemoryRegion{.start = IEAddr, .end = IEAddr, .data = {&ier_, 1}}
        // clang-format on
    };

    uint8_t* get_pointer(uint16_t address);
};

} // namespace boyboy::mmu