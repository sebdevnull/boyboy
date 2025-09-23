/**
 * @file mmu.cpp
 * @brief Memory Management Unit (MMU) for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

// TODO: handle better mirrored memory sections
// TODO: map cartridge RAM

#include "boyboy/mmu.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <span>
#include <stdexcept>
#include <string>

#include "boyboy/common/utils.h"
#include "boyboy/log/logging.h"

namespace boyboy::mmu {

void Mmu::reset()
{
    cart_.fill(0);
    vram_.fill(0);
    eram_.fill(0);
    wram_.fill(0);
    oam_.fill(0);
    ior_.fill(0);
    hram_.fill(0);
    ier_ = 0;

    io_.reset();
    dma_.reset();

    init_memory_map();
}

// copy map version
void Mmu::map_rom(const cartridge::Cartridge& cart)
{
    const auto& rom = cart.get_rom(); // std::vector<std::byte>

    // Clear cart memory
    cart_.fill(0);

    // Copy ROM data into MMU cart_ array
    size_t bytes_to_copy = std::min(rom.size(), cart_.size());
    for (size_t i = 0; i < bytes_to_copy; ++i) {
        cart_.at(i) = static_cast<uint8_t>(rom.at(i));
    }

    // Map ROMBank0 (0x0000 - 0x3FFF)
    map(MemoryRegionID::ROMBank0).data = std::span<uint8_t>(cart_).subspan(0, ROMBank0Size);

    // Map ROMBank1 (0x4000 - 0x7FFF)
    map(MemoryRegionID::ROMBank1).data =
        std::span<uint8_t>(cart_).subspan(ROMBank0Size, ROMBank1Size);

    rom_loaded_ = true;
}

// real map version
// void Mmu::map_rom(const cartridge::Cartridge& cart)
// {
//     const auto& rom = cart.get_rom();

//     // NOLINTBEGIN

//     const auto rom_ptr = reinterpret_cast<const uint8_t*>(rom.data());

//     // Map ROMBank0 to the first 16KB of the cartridge ROM
//     size_t bank0_size = std::min(ROMBank0Size, rom.size());
//     map(MemoryMapIndex::ROMBank0).data =
//         std::span<uint8_t>(const_cast<uint8_t*>(rom_ptr), bank0_size);

//     // Map ROMBank1
//     size_t bank1_size =
//         std::min(ROMBank1Size, rom.size() > ROMBank0Size ? rom.size() - ROMBank0Size : 0);
//     map(MemoryMapIndex::ROMBank1).data =
//         std::span<uint8_t>(const_cast<uint8_t*>(rom_ptr + ROMBank0Size), bank1_size);

//     // NOLINTEND
// }

// NOLINTBEGIN(misc-no-recursion)

uint8_t Mmu::read_byte(uint16_t addr) const
{
    const auto& region = find_region(addr);

    if (&region == &dummy_open_bus_) {
        log::warn("Read from open bus at {}", utils::PrettyHex(addr).to_string());
        return open_bus_;
    }

    if (region.mirrored) {
        if (!region.mirror) {
            throw std::runtime_error("Mirrored region missing mirror target");
        }
        const auto& mirror = map(*region.mirror);
        uint16_t mirror_addr = mirror.start + (addr - region.start);

        log::debug("Read from mirrored region at {} (mirrored to {})",
                   utils::PrettyHex(addr).to_string(),
                   utils::PrettyHex(mirror_addr).to_string());

        return read_byte(mirror_addr);
    }

    if (region.read_handler) {
        return region.read_handler(addr);
    }

    return region.data[addr - region.start];
}

uint16_t Mmu::read_word(uint16_t addr) const
{
    const auto& region = find_region(addr);

    if (&region == &dummy_open_bus_) {
        log::warn("Read from open bus at {}", utils::PrettyHex(addr).to_string());
        return open_bus_;
    }

    if (region.mirrored) {
        if (!region.mirror) {
            throw std::runtime_error("Mirrored region missing mirror target");
        }
        const auto& mirror = map(*region.mirror);
        uint16_t mirror_addr = mirror.start + (addr - region.start);

        log::debug("Read from mirrored region at {} (mirrored to {})",
                   utils::PrettyHex(addr).to_string(),
                   utils::PrettyHex(mirror_addr).to_string());

        return read_word(mirror_addr);
    }

    if (region.read_handler) {
        return region.read_handler(addr);
    }

    uint16_t local_addr = addr - region.start;

    return utils::to_u16(region.data[local_addr + 1], region.data[local_addr]);
}

void Mmu::write_byte(uint16_t addr, uint8_t value)
{
    auto& region = find_region(addr);

    if (region.read_only) {
        log::warn("Attempted write to read-only memory at {}", utils::PrettyHex(addr).to_string());
        return;
    }

    if (dma_.active && addr >= OAMStart && addr < OAMEnd) {
        // Ignore writes to OAM during DMA transfer
        log::warn("Attempted write to OAM during DMA transfer at {}",
                  utils::PrettyHex(addr).to_string());
        return;
    }

    if (&region == &dummy_open_bus_) {
        log::warn("Attempted write to open bus at {}", utils::PrettyHex(addr).to_string());
        return;
    }

    if (region.mirrored) {
        if (!region.mirror) {
            throw std::runtime_error("Mirrored region missing mirror target");
        }
        const auto& mirror = map(*region.mirror);
        uint16_t mirror_addr = mirror.start + (addr - region.start);

        log::debug("Write to mirrored region at {} (mirrored to {})",
                   utils::PrettyHex(addr).to_string(),
                   utils::PrettyHex(mirror_addr).to_string());

        write_byte(mirror_addr, value);
        return;
    }

    if (region.write_handler) {
        region.write_handler(addr, value);
        return;
    }

    region.data[addr - region.start] = value;
}

void Mmu::write_word(uint16_t addr, uint16_t value)
{
    auto& region = find_region(addr);

    if (region.read_only) {
        log::warn("Attempted write to read-only memory at {}", utils::PrettyHex(addr).to_string());
        return;
    }

    if (&region == &dummy_open_bus_) {
        log::warn("Attempted write to open bus at {}", utils::PrettyHex(addr).to_string());
        return;
    }

    if (region.mirrored) {
        if (!region.mirror) {
            throw std::runtime_error("Mirrored region missing mirror target");
        }
        const auto& mirror = map(*region.mirror);
        uint16_t mirror_addr = mirror.start + (addr - region.start);

        log::debug("Write to mirrored region at {} (mirrored to {})",
                   utils::PrettyHex(addr).to_string(),
                   utils::PrettyHex(mirror_addr).to_string());

        write_word(mirror_addr, value);
        return;
    }

    if (region.write_handler) {
        region.write_handler(addr, value);
        return;
    }

    uint16_t local_addr = addr - region.start;
    region.data[local_addr] = utils::lsb(value);
    region.data[local_addr + 1] = utils::msb(value);
}

void Mmu::copy(uint16_t dst_addr, std::span<uint8_t> src)
{
    auto& region = find_region(dst_addr);

    if (region.read_only) {
        log::warn("Attempted copy to read-only memory at {}",
                  utils::PrettyHex(dst_addr).to_string());
        return;
    }

    if (&region == &dummy_open_bus_) {
        log::warn("Attempted copy to open bus at {}", utils::PrettyHex(dst_addr).to_string());
        return;
    }

    if (region.mirrored) {
        if (!region.mirror) {
            throw std::runtime_error("Mirrored region missing mirror target");
        }
        const auto& mirror = map(*region.mirror);
        uint16_t mirror_addr = mirror.start + (dst_addr - region.start);

        log::debug("Copy to mirrored region at {} (mirrored to {})",
                   utils::PrettyHex(dst_addr).to_string(),
                   utils::PrettyHex(mirror_addr).to_string());

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

    log::trace("Starting DMA transfer from {}", utils::PrettyHex(src).to_string());
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
        log::trace("DMA transfer completed, checksum: {}", utils::PrettyHex(cks).to_string());
        if (cks == 0) {
            log::warn("DMA transfer checksum is zero, possible issue with source data");
        }
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
    map(MemoryRegionID::ROMBank0) = {
        .id = MemoryRegionID::ROMBank0,
        .start = ROMBank0Start,
        .end = ROMBank0End,
        .data = cart_,
        // .read_only = true,
        .read_only = false,
    };
    map(MemoryRegionID::ROMBank1) = {
        .id = MemoryRegionID::ROMBank1,
        .start = ROMBank1Start,
        .end = ROMBank1End,
        .data = std::span<uint8_t>(cart_).subspan(ROMBank0Size),
        // .read_only = true,
        .read_only = false,
    };
    map(MemoryRegionID::VRAM) = {
        .id = MemoryRegionID::VRAM,
        .start = VRAMStart,
        .end = VRAMEnd,
        .data = vram_,
    };
    map(MemoryRegionID::ERAM) = {
        .id = MemoryRegionID::ERAM,
        .start = ERAMStart,
        .end = ERAMEnd,
        .data = eram_,
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
        // .read_only = true,
        .read_handler = [](uint16_t) { return uint8_t{0x00}; },
        .write_handler = [](uint16_t, uint8_t) {},
    };
    map(MemoryRegionID::IO) = {
        .id = MemoryRegionID::IO,
        .start = IOStart,
        .end = IOEnd,
        .data = ior_,
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
        // .read_only = true,
        .read_only = false,
    };
}

[[nodiscard]] size_t Mmu::find_region_index(uint16_t addr) const
{
    for (size_t i = 0; i < memory_map_.size(); i++) {
        auto region = memory_map_.at(i);
        if (addr >= region.start && addr <= region.end) {
            return i;
        }
    }
#ifndef NDEBUG
    throw std::out_of_range("Invalid memory access at " + utils::PrettyHex(addr).to_string());
#else
    return size_t(-1);
#endif
}

Mmu::MemoryRegion& Mmu::find_region(uint16_t addr)
{
    size_t idx = find_region_index(addr);
    return (idx == size_t(-1)) ? dummy_open_bus_ : memory_map_.at(idx);
}
const Mmu::MemoryRegion& Mmu::find_region(uint16_t addr) const
{
    size_t idx = find_region_index(addr);
    return (idx == size_t(-1)) ? dummy_open_bus_ : memory_map_.at(idx);
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

} // namespace boyboy::mmu