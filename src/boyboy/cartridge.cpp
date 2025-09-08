/**
 * @file cartridge.cpp
 * @brief Cartridge handling for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "cartridge.h"

#include <cstdint>
#include <filesystem>
#include <format>
#include <fstream>
#include <ios>
#include <iostream>
#include <stdexcept>

namespace boyboy::cartridge {

// Safe conversion between std::byte and char for I/O operations
namespace {

// NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
[[maybe_unused]] inline char* as_char_ptr(std::byte* ptr) noexcept
{
    static_assert(sizeof(std::byte) == sizeof(char), "std::byte and char must have the same size");
    static_assert(alignof(std::byte) == alignof(char),
                  "std::byte and char must have the same alignment");
    return reinterpret_cast<char*>(ptr);
}

[[maybe_unused]] inline const char* as_char_ptr(const std::byte* ptr) noexcept
{
    static_assert(sizeof(std::byte) == sizeof(char), "std::byte and char must have the same size");
    static_assert(alignof(std::byte) == alignof(char),
                  "std::byte and char must have the same alignment");
    return reinterpret_cast<const char*>(ptr);
}
// NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)

} // namespace

void Cartridge::load(std::string_view path)
{
    namespace fs = std::filesystem;

    fs::path file_path(path);

    if (!fs::exists(file_path)) {
        throw std::runtime_error(std::format("File not found: {}", path));
    }

    std::uintmax_t file_size = 0;
    try {
        file_size = fs::file_size(file_path);
    }
    catch (const fs::filesystem_error& e) {
        throw std::runtime_error(std::format("Failed to get file size: {}", e.what()));
    }

    std::ifstream rom_file(file_path, std::ios::binary);
    if (!rom_file.is_open()) {
        throw std::runtime_error(std::format("Failed to open file: {}", path));
    }

    rom_.resize(file_size);

    rom_file.read(as_char_ptr(rom_.data()), static_cast<std::streamsize>(file_size));

    if (rom_file.gcount() != static_cast<std::streamsize>(file_size)) {
        rom_.clear();
        throw std::runtime_error("Failed to read entire file");
    }
}

void Cartridge::unload()
{
    rom_.clear();
    rom_.shrink_to_fit();
}

void Cartridge::parse_header()
{
    header_.title.clear();
    for (auto pos = Header::TitlePos; pos < Header::TitleEnd; ++pos) {
        char cur_c = static_cast<char>(rom_.at(pos));
        if (cur_c == 0) {
            break;
        }
        header_.title += cur_c;
    }

    header_.cgb_flag = static_cast<uint8_t>(rom_.at(Header::CGBFlagPos));
    header_.sgb_flag = static_cast<uint8_t>(rom_.at(Header::SGBFlagPos));
    header_.cartridge_type = static_cast<CartridgeType>(rom_.at(Header::CartridgeTypePos));
    header_.rom_size = static_cast<uint8_t>(rom_.at(Header::ROMSizePos));
    header_.ram_size = static_cast<uint8_t>(rom_.at(Header::RAMSizePos));
    header_.header_checksum = static_cast<uint8_t>(rom_.at(Header::HeaderChecksumPos));
    header_.checksum = static_cast<uint16_t>(rom_.at(Header::ChecksumPos)) << 8 |
                       static_cast<uint16_t>(rom_.at(Header::ChecksumPos + 1));
}

constexpr std::string_view to_string(CartridgeType type)
{
    switch (type) {
    case CartridgeType::ROMOnly:
        return "ROM_ONLY";
    case CartridgeType::MBC1:
        return "MBC1";
    case CartridgeType::MBC1RAM:
        return "MBC1_RAM";
    case CartridgeType::MBC1RAMBattery:
        return "MBC1_RAM_BATTERY";
    case CartridgeType::MBC2:
        return "MBC2";
    case CartridgeType::MBC2Battery:
        return "MBC2_BATTERY";
    case CartridgeType::ROMRAM:
        return "ROM_RAM";
    case CartridgeType::ROMRAMBattery:
        return "ROM_RAM_BATTERY";
    case CartridgeType::MMM01:
        return "MMM01";
    case CartridgeType::MMM01RAM:
        return "MMM01_RAM";
    case CartridgeType::MMM01RAMBattery:
        return "MMM01_RAM_BATTERY";
    case CartridgeType::MBC3TimerBattery:
        return "MBC3_TIMER_BATTERY";
    case CartridgeType::MBC3TimerRAMBattery:
        return "MBC3_TIMER_RAM_BATTERY";
    case CartridgeType::MBC3:
        return "MBC3";
    case CartridgeType::MBC3RAM:
        return "MBC3_RAM";
    case CartridgeType::MBC3RAMBattery:
        return "MBC3_RAM_BATTERY";
    case CartridgeType::MBC4:
        return "MBC4";
    case CartridgeType::MBC4RAM:
        return "MBC4_RAM";
    case CartridgeType::MBC4RAMBattery:
        return "MBC4_RAM_BATTERY";
    case CartridgeType::MBC5:
        return "MBC5";
    case CartridgeType::MBC5RAM:
        return "MBC5_RAM";
    case CartridgeType::MBC5RAMBattery:
        return "MBC5_RAM_BATTERY";
    case CartridgeType::MBC5Rumble:
        return "MBC5_RUMBLE";
    case CartridgeType::MBC5RumbleRAM:
        return "MBC5_RUMBLE_RAM";
    case CartridgeType::MBC5RumbleRAMBattery:
        return "MBC5_RUMBLE_RAM_BATTERY";
    case CartridgeType::PocketCamera:
        return "POCKET_CAMERA";
    case CartridgeType::BandaiTama5:
        return "BANDAI_TAMA5";
    case CartridgeType::HUC3:
        return "HUC3";
    case CartridgeType::HUC1RAMBattery:
        return "HUC1_RAM_BATTERY";
    default:
        return "UNKNOWN";
    }
}

void Cartridge::Header::print() const
{
    std::cout << "Title: " << title << "\n"
              << "CGB Flag: " << std::to_string(cgb_flag) << "\n"
              << "SGB Flag: " << std::to_string(sgb_flag) << "\n"
              << "Cartridge Type: " << to_string(cartridge_type) << "\n"
              << "ROM Size: " << std::to_string(rom_size) << "\n"
              << "RAM Size: " << std::to_string(ram_size) << "\n"
              << "Header Checksum: " << std::to_string(header_checksum) << "\n"
              << "Global Checksum: " << std::to_string(checksum) << "\n";
}

} // namespace boyboy::cartridge