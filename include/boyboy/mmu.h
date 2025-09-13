/**
 * @file mmu.h
 * @brief Memory Management Unit (MMU) for the BoyBoy emulator.
 *
 * ========================== Memory Map ==========================
 * 0000-3FFF   16KB ROM Bank 00     (in cartridge, fixed at bank 00)
 * 4000-7FFF   16KB ROM Bank 01..NN (in cartridge, switchable bank number)
 * 8000-9FFF   8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode)
 * A000-BFFF   8KB External RAM     (in cartridge, switchable bank, if any)
 * C000-CFFF   4KB Work RAM Bank 0 (WRAM)
 * D000-DFFF   4KB Work RAM Bank 1 (WRAM)  (switchable bank 1-7 in CGB Mode)
 * E000-FDFF   Same as C000-DDFF (ECHO)    (typically not used)
 * FE00-FE9F   Sprite Attribute Table (OAM)
 * FEA0-FEFF   Not Usable
 * FF00-FF7F   I/O Ports
 * FF80-FFFE   High RAM (HRAM)
 * FFFF        Interrupt Enable Register
 * ================================================================
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <optional>
#include <span>

#include "boyboy/cartridge.h"
#include "boyboy/mmu_constants.h"

namespace boyboy::mmu {

class Mmu {
public:
    Mmu() { reset(); }

    // Reset MMU state
    void reset();

    // Maps ROM memory into own memory map
    void map_rom(const cartridge::Cartridge& cart);

    [[nodiscard]] uint8_t read_byte(uint16_t addr) const;
    [[nodiscard]] uint16_t read_word(uint16_t addr) const;
    void write_byte(uint16_t addr, uint8_t value);
    void write_word(uint16_t addr, uint16_t value);
    void copy(uint16_t dst_addr, std::span<uint8_t> src);

private:
    enum class MemoryRegionID : uint8_t {
        ROMBank0 = 0,
        ROMBank1,
        VRAM,
        ERAM,
        WRAM0,
        WRAM1,
        ECHO,
        OAM,
        IO,
        HRAM,
        IEReg,
        NumRegions,
        OpenBus, // Invalid region
    };

    struct MemoryRegion {
        MemoryRegionID id = MemoryRegionID::OpenBus;
        uint16_t start{};
        uint16_t end{};
        std::span<uint8_t> data;

        // Protection flags
        bool read_only = false;
        bool mirrored = false;
        bool io_register = false;

        // Optional mirror
        std::optional<MemoryRegionID> mirror = std::nullopt;

        // Optional I/O callbacks
        std::function<void(uint16_t, uint8_t)> write_handler = nullptr;
        std::function<uint8_t(uint16_t)> read_handler = nullptr;
    };

    // ROM load status flag
    bool rom_loaded_ = false;

    // Memory banks
    std::array<uint8_t, ROMBankSize> cart_{}; // cartridge
    std::array<uint8_t, VRAMSize> vram_{};    // video ram
    std::array<uint8_t, ERAMSize> eram_{};    // external ram
    std::array<uint8_t, WRAMSize> wram_{};    // work ram
    std::array<uint8_t, OAMSize> oam_{};      // sprite attribute table
    std::array<uint8_t, IOSize> ior_{};       // i/o registers
    std::array<uint8_t, HRAMSize> hram_{};    // high ram
    uint8_t ier_{};                           // interrupt enable register

    // Fallback open bus region
    uint8_t open_bus_{0xFF};
    MemoryRegion dummy_open_bus_{
        .id = MemoryRegionID::OpenBus,
        .start = 0x0000,
        .end = 0x0000,
        .data = {&open_bus_, 1},
        .read_only = false,
        .mirrored = false,
        .io_register = false,
        .write_handler = nullptr,
        .read_handler = nullptr,
    };

    // Memory map table for address mapping
    std::array<MemoryRegion, static_cast<size_t>(MemoryRegionID::NumRegions)> memory_map_;

    // Memory mapping helpers
    MemoryRegion& map(MemoryRegionID idx) { return memory_map_.at(static_cast<size_t>(idx)); }
    [[nodiscard]] const MemoryRegion& map(MemoryRegionID idx) const
    {
        return memory_map_.at(static_cast<size_t>(idx));
    }

    // Initialize memory mappings
    void init_memory_map();

    // Memory region finding
    [[nodiscard]] inline size_t find_region_index(uint16_t addr) const;
    [[nodiscard]] inline MemoryRegion& find_region(uint16_t addr);
    [[nodiscard]] inline const MemoryRegion& find_region(uint16_t addr) const;
};

} // namespace boyboy::mmu