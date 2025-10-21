/**
 * @file mmu.cpp
 * @brief Memory Management Unit (MMU) for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

// TODO: handle better mirrored memory sections
//       We actually only have one (ECHO -> WRAM) and it behaves differently depending on the
//       cartridge type and GB HW version (DMG/CGB). Check "The Cycle-Accurate Game Boy Docs"
// TODO: read/write word and copy are only used in tests. Consider removing them.

#include "boyboy/core/mmu/mmu.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <span>
#include <stdexcept>
#include <string>

#include "boyboy/common/log/logging.h"
#include "boyboy/common/utils.h"
#include "boyboy/core/cartridge/cartridge.h"
#include "boyboy/core/mmu/constants.h"
#include "boyboy/core/profiling/profiler_utils.h"

namespace boyboy::core::mmu {

using namespace boyboy::common;

void Mmu::reset()
{
    // Reset memory regions
    vram_.fill(0);
    wram_.fill(0);
    oam_.fill(0);
    hram_.fill(0);
    ier_ = 0;

    // Reset components
    io_.reset();
    dma_.reset();

    // Reset memory locks
    lock_vram_ = false;
    lock_oam_ = false;

    // Reinitialize memory map
    init_memory_map();
}

void Mmu::map_rom(cartridge::Cartridge& cart)
{
    auto& rom_bank0 = map(MemoryRegionID::ROMBank0);
    auto& rom_bank1 = map(MemoryRegionID::ROMBank1);
    auto& sram = map(MemoryRegionID::SRAM);

    auto cart_read = [&cart](uint16_t addr) -> uint8_t {
        return cart.mbc_read(addr);
    };
    auto cart_write = [&cart](uint16_t addr, uint8_t value) {
        cart.mbc_write(addr, value);
    };

    // Map ROMBank0 (0x0000 - 0x3FFF)
    rom_bank0.read_handler = cart_read;
    rom_bank0.write_handler = cart_write;

    // Map ROMBank1 (0x4000 - 0x7FFF)
    rom_bank1.read_handler = cart_read;
    rom_bank1.write_handler = cart_write;

    // Map SRAM (0xA000 - 0xBFFF)
    sram.read_handler = cart_read;
    sram.write_handler = cart_write;

    rom_loaded_ = true;
}

// NOLINTBEGIN(misc-no-recursion)

uint8_t Mmu::read_byte(uint16_t addr, bool unlocked) const
{
    BB_PROFILE_START(profiling::HotSection::MmuRead);

    const auto& region = region_lookup(addr);

    // Check if region is accessible
    if (!unlocked && is_region_locked(region.id)) {
        return OpenBusValue;
    }

    if (region.mirrored) {
        if (!region.mirror) {
            BB_PROFILE_STOP(profiling::HotSection::MmuRead);

            throw std::runtime_error("Mirrored region missing mirror target");
        }
        const auto& mirror = map(*region.mirror);
        uint16_t mirror_addr = mirror.start + (addr - region.start);

        log::debug(
            "Read from mirrored region at {}: [{}] (mirrored to {})",
            common::utils::PrettyHex(addr).to_string(),
            common::utils::PrettyHex(mirror.data[addr - region.start]).to_string(),
            common::utils::PrettyHex(mirror_addr).to_string()
        );

        uint8_t result = read_byte(mirror_addr);
        BB_PROFILE_STOP(profiling::HotSection::MmuRead);

        return result;
    }

    if (region.read_handler) {
        uint8_t result = region.read_handler(addr);
        BB_PROFILE_STOP(profiling::HotSection::MmuRead);

        return result;
    }

    uint8_t result = region.data[addr - region.start];
    BB_PROFILE_STOP(profiling::HotSection::MmuRead);

    return result;
}

uint16_t Mmu::read_word(uint16_t addr) const
{
    const auto& region = region_lookup(addr);

    if (region.mirrored) {
        if (!region.mirror) {
            throw std::runtime_error("Mirrored region missing mirror target");
        }
        const auto& mirror = map(*region.mirror);
        uint16_t mirror_addr = mirror.start + (addr - region.start);

        log::debug(
            "Read from mirrored region at {} (mirrored to {})",
            common::utils::PrettyHex(addr).to_string(),
            common::utils::PrettyHex(mirror_addr).to_string()
        );

        return read_word(mirror_addr);
    }

    if (region.read_handler) {
        return region.read_handler(addr);
    }

    uint16_t local_addr = addr - region.start;

    return common::utils::to_u16(region.data[local_addr + 1], region.data[local_addr]);
}

void Mmu::write_byte(uint16_t addr, uint8_t value, bool unlocked)
{
    BB_PROFILE_START(profiling::HotSection::MmuWrite);

    if (dma_.active && addr >= OAMStart && addr <= OAMEnd) {
        // Ignore writes to OAM during DMA transfer
        log::warn(
            "Attempted write to OAM during DMA transfer at {}",
            common::utils::PrettyHex(addr).to_string()
        );
        BB_PROFILE_STOP(profiling::HotSection::MmuWrite);

        return;
    }

    auto& region = region_lookup(addr);

    // Check if region is accessible
    if (!unlocked && is_region_locked(region.id)) {
        return;
    }

    if (region.read_only) {
        log::warn(
            "Attempted write to read-only memory at {}", common::utils::PrettyHex(addr).to_string()
        );
        BB_PROFILE_STOP(profiling::HotSection::MmuWrite);

        return;
    }

    if (region.mirrored) {
        if (!region.mirror) {
            BB_PROFILE_STOP(profiling::HotSection::MmuWrite);

            throw std::runtime_error("Mirrored region missing mirror target");
        }
        const auto& mirror = map(*region.mirror);
        uint16_t mirror_addr = mirror.start + (addr - region.start);

        log::debug(
            "Write to mirrored region at {}: [{}] (mirrored to {})",
            common::utils::PrettyHex(addr).to_string(),
            common::utils::PrettyHex(value).to_string(),
            common::utils::PrettyHex(mirror_addr).to_string()
        );

        BB_PROFILE_STOP(profiling::HotSection::MmuWrite);
        write_byte(mirror_addr, value);

        return;
    }

    if (region.write_handler) {
        region.write_handler(addr, value);
        BB_PROFILE_STOP(profiling::HotSection::MmuWrite);

        return;
    }

    region.data[addr - region.start] = value;

    BB_PROFILE_STOP(profiling::HotSection::MmuWrite);
}

void Mmu::write_word(uint16_t addr, uint16_t value)
{
    auto& region = region_lookup(addr);

    if (region.read_only) {
        log::warn(
            "Attempted write to read-only memory at {}", common::utils::PrettyHex(addr).to_string()
        );
        return;
    }

    if (region.mirrored) {
        if (!region.mirror) {
            throw std::runtime_error("Mirrored region missing mirror target");
        }
        const auto& mirror = map(*region.mirror);
        uint16_t mirror_addr = mirror.start + (addr - region.start);

        log::debug(
            "Write to mirrored region at {} (mirrored to {})",
            common::utils::PrettyHex(addr).to_string(),
            common::utils::PrettyHex(mirror_addr).to_string()
        );

        write_word(mirror_addr, value);
        return;
    }

    if (region.write_handler) {
        region.write_handler(addr, value);
        return;
    }

    uint16_t local_addr = addr - region.start;
    region.data[local_addr] = common::utils::lsb(value);
    region.data[local_addr + 1] = common::utils::msb(value);
}

void Mmu::copy(uint16_t dst_addr, std::span<uint8_t> src)
{
    auto& region = region_lookup(dst_addr);

    if (region.read_only) {
        log::warn(
            "Attempted copy to read-only memory at {}",
            common::utils::PrettyHex(dst_addr).to_string()
        );
        return;
    }

    if (region.mirrored) {
        if (!region.mirror) {
            throw std::runtime_error("Mirrored region missing mirror target");
        }
        const auto& mirror = map(*region.mirror);
        uint16_t mirror_addr = mirror.start + (dst_addr - region.start);

        log::debug(
            "Copy to mirrored region at {} (mirrored to {})",
            common::utils::PrettyHex(dst_addr).to_string(),
            common::utils::PrettyHex(mirror_addr).to_string()
        );

        copy(mirror_addr, src);
        return;
    }

    for (size_t i = 0; i < src.size(); i++) {
        write_byte(dst_addr + i, src[i]);
    }
}

void Mmu::start_dma(uint8_t value)
{
    dma_.start(value);
}

void Mmu::tick_dma(uint16_t cycles)
{
    dma_.tick(cycles, *this);
}

void Mmu::Dma::start(uint8_t value)
{
    if (active) {
        log::warn("DMA transfer already in progress, new request ignored");
        return;
    }

    src = static_cast<uint16_t>(value) << 8; // value * 0x100
    dst = OAMStart;
    active = true;
    bytes_remaining = DMATransferSize;
    tick_counter = 0;
    cks = 0;

    log::trace("Starting DMA transfer from {}", common::utils::PrettyHex(src).to_string());
}

void Mmu::Dma::tick(uint16_t cycles, Mmu& mmu)
{
    if (!active) {
        return;
    }

    tick_counter += cycles;

    while (tick_counter >= 4 && bytes_remaining > 0) {
        uint8_t data = mmu.read_byte(src);

        // Use direct access to OAM to avoid blocking writes during DMA
        mmu.oam_.at(dst - OAMStart) = data;

        bytes_remaining--;
        tick_counter -= 4;
        src++;
        dst++;

        cks = (cks + data) & 0xFFFF; // simple checksum
    }

    if (bytes_remaining == 0) {
        active = false;
        log::trace(
            "DMA transfer completed, checksum: {}", common::utils::PrettyHex(cks).to_string()
        );
    }
}

void Mmu::Dma::reset()
{
    active = false;
    src = 0;
    dst = OAMStart;
    bytes_remaining = 0;
    tick_counter = 0;
    cks = 0;
}

void Mmu::set_io_write_callback(IoWriteCallback callback)
{
    io_write_callback_ = std::move(callback);
}

void Mmu::set_io_read_callback(IoReadCallback callback)
{
    io_read_callback_ = std::move(callback);
}

void Mmu::dump(uint16_t start_addr, uint16_t end_addr, const std::string& filename) const
{
    if (!filename.empty()) {
        std::ofstream out(filename, std::ios::binary);
        if (!out) {
            log::error("Failed to open file: {}", filename);
            return;
        }
        for (uint16_t addr = start_addr; addr <= end_addr; ++addr) {
            uint8_t byte = read_byte(addr);
            out.put(static_cast<char>(byte));
        }
        log::info("Memory dumped to {}", filename);
    }
}

// NOLINTEND(misc-no-recursion)

void Mmu::init_memory_map()
{
    auto unloaded_rom_read = [](uint16_t addr) -> uint8_t {
        log::warn(
            "Read from ROM before ROM loaded at {}", common::utils::PrettyHex(addr).to_string()
        );
        return OpenBusValue;
    };
    auto unloaded_rom_write = [](uint16_t addr, uint8_t value) {
        log::warn(
            "Write to ROM before ROM loaded at {}: {}",
            common::utils::PrettyHex(addr).to_string(),
            common::utils::PrettyHex(value).to_string()
        );
    };
    auto unloaded_sram_read = [](uint16_t addr) -> uint8_t {
        log::warn(
            "Read from SRAM before ROM loaded at {}", common::utils::PrettyHex(addr).to_string()
        );
        return OpenBusValue;
    };
    auto unloaded_sram_write = [](uint16_t addr, uint8_t value) {
        log::warn(
            "Write to SRAM before ROM loaded at {}: {}",
            common::utils::PrettyHex(addr).to_string(),
            common::utils::PrettyHex(value).to_string()
        );
    };

    map(MemoryRegionID::ROMBank0) = {
        .id = MemoryRegionID::ROMBank0,
        .start = ROMBank0Start,
        .end = ROMBank0End,
        .data = {},
        .read_handler = unloaded_rom_read,
        .write_handler = unloaded_rom_write,
    };
    map(MemoryRegionID::ROMBank1) = {
        .id = MemoryRegionID::ROMBank1,
        .start = ROMBank1Start,
        .end = ROMBank1End,
        .data = {},
        .read_handler = unloaded_rom_read,
        .write_handler = unloaded_rom_write,
    };
    map(MemoryRegionID::VRAM) = {
        .id = MemoryRegionID::VRAM,
        .start = VRAMStart,
        .end = VRAMEnd,
        .data = vram_,
    };
    map(MemoryRegionID::SRAM) = {
        .id = MemoryRegionID::SRAM,
        .start = SRAMStart,
        .end = SRAMEnd,
        .data = {},
        .read_handler = unloaded_sram_read,
        .write_handler = unloaded_sram_write,
    };
    map(MemoryRegionID::WRAM0) = {
        .id = MemoryRegionID::WRAM0,
        .start = WRAM0Start,
        .end = WRAM0End,
        .data = wram_,
    };
    map(MemoryRegionID::WRAM1) = {
        .id = MemoryRegionID::WRAM1,
        .start = WRAM1Start,
        .end = WRAM1End,
        .data = std::span<uint8_t>(wram_).subspan(WRAM0Size),
    };
    map(MemoryRegionID::ECHO) = {
        .id = MemoryRegionID::ECHO,
        .start = ECHOStart,
        .end = ECHOEnd,
        .data = {},
        .mirrored = true,
        .mirror = MemoryRegionID::WRAM0,
    };
    map(MemoryRegionID::OAM) = {
        .id = MemoryRegionID::OAM,
        .start = OAMStart,
        .end = OAMEnd,
        .data = oam_,
    };
    map(MemoryRegionID::NotUsable) = {
        .id = MemoryRegionID::NotUsable,
        .start = NotUsableStart,
        .end = NotUsableEnd,
        .data = {},
        .read_only = true,
        .read_handler = [](uint16_t) { return uint8_t{0x00}; },
        .write_handler = [](uint16_t, uint8_t) {},
    };
    map(MemoryRegionID::IO) = {
        .id = MemoryRegionID::IO,
        .start = IOStart,
        .end = IOEnd,
        .data = {},
        .io_register = true,
        .read_handler = [&](uint16_t addr) -> uint8_t { return io_read(addr); },
        .write_handler = [&](uint16_t addr, uint8_t value) { io_write(addr, value); },
    };
    map(MemoryRegionID::HRAM) = {
        .id = MemoryRegionID::HRAM,
        .start = HRAMStart,
        .end = HRAMEnd,
        .data = hram_,
    };
    map(MemoryRegionID::IEReg) = {
        .id = MemoryRegionID::IEReg,
        .start = IEAddr,
        .end = IEAddr,
        .data = {&ier_, 1},
    };

    // Fallback for unmapped addresses (open bus)
    map(MemoryRegionID::OpenBus) = {
        .id = MemoryRegionID::OpenBus,
        .start = 0x0000,
        .end = 0x0000,
        .data = {},
        .read_only = false,
        .mirrored = false,
        .io_register = false,
        .read_handler = [&](uint16_t addr) -> uint8_t {
#ifdef DEBUG
            // In debug mode, throw for easier debugging
            throw std::out_of_range(std::format(
                "Read from unmapped memory at {}", common::utils::PrettyHex(addr).to_string()
            ));
#else
            // In release mode, log a warning and return open bus value
            log::warn(
                "Read from unmapped memory at {}", common::utils::PrettyHex(addr).to_string()
            );
            return OpenBusValue;
#endif
        },
        .write_handler =
            [&](uint16_t addr, uint8_t value) {
#ifdef DEBUG
                // In debug mode, throw for easier debugging
                throw std::out_of_range(std::format(
                    "Write to unmapped memory at {}: {}",
                    common::utils::PrettyHex(addr).to_string(),
                    common::utils::PrettyHex(value).to_string()
                ));
#else
                // In release mode, log a warning
                log::warn(
                    "Write to unmapped memory at {}: {}",
                    common::utils::PrettyHex(addr).to_string(),
                    common::utils::PrettyHex(value).to_string()
                );
#endif
            },
    };

    init_region_lut();
}

void Mmu::init_region_lut()
{
    // Initialize all entries to point to OpenBus region
    region_lut_.fill(&map(MemoryRegionID::OpenBus));

    // Populate the lookup table with actual memory regions
    for (auto& region : memory_map_) {
        if (region.id == MemoryRegionID::OpenBus) {
            continue;
        }
        std::ranges::fill(
            region_lut_.begin() + region.start, region_lut_.begin() + region.end + 1, &region
        );
    }
}

void Mmu::io_write(uint16_t addr, uint8_t value)
{
    io_.write(addr, value);
    if (io_write_callback_) {
        io_write_callback_(addr, value);
    }
}

[[nodiscard]] uint8_t Mmu::io_read(uint16_t addr) const
{
    uint8_t value = io_.read(addr);
    if (io_read_callback_) {
        io_read_callback_(addr, value);
    }
    return value;
}

} // namespace boyboy::core::mmu