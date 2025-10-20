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

#include "boyboy/core/io/io.h"
#include "boyboy/core/mmu/constants.h"

// Forward declaration
namespace boyboy::core::cartridge {
class Cartridge;
}

namespace boyboy::core::mmu {

class Mmu {
public:
    using IoWriteCallback = std::function<void(uint16_t, uint8_t)>;
    using IoReadCallback = std::function<void(uint16_t, uint8_t)>;

    Mmu() { reset(); }

    // Reset MMU state
    void reset();

    // Maps ROM memory into own memory map
    void map_rom(cartridge::Cartridge& cart);

    // Memory access
    [[nodiscard]] uint8_t read_byte(uint16_t addr) const;
    [[nodiscard]] uint16_t read_word(uint16_t addr) const;
    void write_byte(uint16_t addr, uint8_t value);
    void write_word(uint16_t addr, uint16_t value);
    void copy(uint16_t dst_addr, std::span<uint8_t> src);

    // DMA transfer
    void start_dma(uint8_t value);
    void tick_dma(uint16_t cycles);

    // Access to I/O handler
    [[nodiscard]] io::Io& io() { return io_; }
    [[nodiscard]] const io::Io& io() const { return io_; }

    // IO read/write callbacks
    void set_io_write_callback(IoWriteCallback callback);
    void set_io_read_callback(IoReadCallback callback);

    // For debugging: dump memory region to file
    void dump(uint16_t start_addr, uint16_t end_addr, const std::string& filename = "") const;

private:
    enum class MemoryRegionID : uint8_t {
        ROMBank0 = 0,
        ROMBank1,
        VRAM,
        SRAM,
        WRAM0,
        WRAM1,
        ECHO,
        OAM,
        NotUsable,
        IO,
        HRAM,
        IEReg,
        OpenBus, // Invalid region
        Count,
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
        std::function<uint8_t(uint16_t)> read_handler = nullptr;
        std::function<void(uint16_t, uint8_t)> write_handler = nullptr;

        // TODO: use helpers
        // Helpers
        [[nodiscard]] size_t size() const { return end - start + 1; }
        [[nodiscard]] bool contains(uint16_t addr) const { return addr >= start && addr <= end; }
    };

    struct Dma {
        bool active = false;
        uint16_t src = 0;
        uint16_t dst = OAMStart;
        int bytes_remaining = 0;
        uint16_t tick_counter = 0;
        uint16_t cks = 0; // debug variable to track transfer content

        void start(uint8_t value);
        void tick(uint16_t cycles, Mmu& mmu);
        void reset();
    };

    // DMA state
    Dma dma_;

    // I/O handler
    io::Io io_;

    // Optional I/O read/write callbacks
    IoWriteCallback io_write_callback_ = nullptr;
    IoReadCallback io_read_callback_ = nullptr;

    // ROM load status flag
    bool rom_loaded_ = false;

    // Memory banks
    std::array<uint8_t, VRAMSize> vram_{}; // video ram
    std::array<uint8_t, WRAMSize> wram_{}; // work ram
    std::array<uint8_t, OAMSize> oam_{};   // sprite attribute table
    std::array<uint8_t, HRAMSize> hram_{}; // high ram
    uint8_t ier_{};                        // interrupt enable register

    // Memory map table for region mapping
    std::array<MemoryRegion, static_cast<size_t>(MemoryRegionID::Count)> memory_map_{};

    // Lookup table for fast region finding
    std::array<MemoryRegion*, MemoryMapSize> region_lut_{};

    // Memory mapping helpers
    MemoryRegion& map(MemoryRegionID idx) { return memory_map_[static_cast<size_t>(idx)]; }
    [[nodiscard]] const MemoryRegion& map(MemoryRegionID idx) const
    {
        return memory_map_[static_cast<size_t>(idx)];
    }

    // Initialize memory map and lookup table
    void init_memory_map();
    void init_region_lut();

    // Memory region lookup
    [[nodiscard]] MemoryRegion& region_lookup(uint16_t addr) { return *region_lut_[addr]; }
    [[nodiscard]] const MemoryRegion& region_lookup(uint16_t addr) const
    {
        return *region_lut_[addr];
    }

    // I/O read/write handlers
    void io_write(uint16_t addr, uint8_t value);
    [[nodiscard]] uint8_t io_read(uint16_t addr) const;
};

} // namespace boyboy::core::mmu