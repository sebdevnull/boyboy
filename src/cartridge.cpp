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
[[maybe_unused]] inline char* as_char_ptr(std::byte* ptr) noexcept {
    static_assert(sizeof(std::byte) == sizeof(char), "std::byte and char must have the same size");
    static_assert(alignof(std::byte) == alignof(char),
                  "std::byte and char must have the same alignment");
    return reinterpret_cast<char*>(ptr);
}

[[maybe_unused]] inline const char* as_char_ptr(const std::byte* ptr) noexcept {
    static_assert(sizeof(std::byte) == sizeof(char), "std::byte and char must have the same size");
    static_assert(alignof(std::byte) == alignof(char),
                  "std::byte and char must have the same alignment");
    return reinterpret_cast<const char*>(ptr);
}
// NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)

} // namespace

void Cartridge::load(std::string_view path) {
    namespace fs = std::filesystem;

    fs::path file_path(path);

    if (!fs::exists(file_path)) {
        throw std::runtime_error(std::format("File not found: {}", path));
    }

    std::uintmax_t file_size = 0;
    try {
        file_size = fs::file_size(file_path);
    } catch (const fs::filesystem_error& e) {
        throw std::runtime_error(std::format("Failed to get file size: {}", e.what()));
    }

    std::ifstream rom_file(file_path, std::ios::binary);
    if (!rom_file.is_open()) {
        throw std::runtime_error(std::format("Failed to open file: {}", path));
    }

    this->rom.resize(file_size);

    rom_file.read(as_char_ptr(this->rom.data()), static_cast<std::streamsize>(file_size));

    if (rom_file.gcount() != static_cast<std::streamsize>(file_size)) {
        this->rom.clear();
        throw std::runtime_error("Failed to read entire file");
    }
}

void Cartridge::unload() {
    rom.clear();
    rom.shrink_to_fit();
}

void Cartridge::parse_header() {
    header.title.clear();
    for (auto pos = Header::kTitlePos; pos < Header::kTitleEnd; ++pos) {
        char cur_c = static_cast<char>(rom.at(pos));
        if (cur_c == 0) {
            break;
        }
        header.title += cur_c;
    }

    header.cgb_flag = static_cast<uint8_t>(rom.at(Header::kCGBFlagPos));
    header.sgb_flag = static_cast<uint8_t>(rom.at(Header::kSGBFlagPos));
    header.cartridge_type = static_cast<CartridgeType>(rom.at(Header::kCartridgeTypePos));
    header.rom_size = static_cast<uint8_t>(rom.at(Header::kROMSizePos));
    header.ram_size = static_cast<uint8_t>(rom.at(Header::kRAMSizePos));
    header.header_checksum = static_cast<uint8_t>(rom.at(Header::kHeaderChecksumPos));
    header.checksum = static_cast<uint16_t>(rom.at(Header::kChecksumPos)) << 8 |
                      static_cast<uint16_t>(rom.at(Header::kChecksumPos + 1));
}

constexpr std::string_view to_string(CartridgeType type) {
    switch (type) {
    case CartridgeType::ROM_ONLY:
        return "ROM_ONLY";
    case CartridgeType::MBC1:
        return "MBC1";
    case CartridgeType::MBC1_RAM:
        return "MBC1_RAM";
    case CartridgeType::MBC1_RAM_BATTERY:
        return "MBC1_RAM_BATTERY";
    case CartridgeType::MBC2:
        return "MBC2";
    case CartridgeType::MBC2_BATTERY:
        return "MBC2_BATTERY";
    case CartridgeType::ROM_RAM:
        return "ROM_RAM";
    case CartridgeType::ROM_RAM_BATTERY:
        return "ROM_RAM_BATTERY";
    case CartridgeType::MMM01:
        return "MMM01";
    case CartridgeType::MMM01_RAM:
        return "MMM01_RAM";
    case CartridgeType::MMM01_RAM_BATTERY:
        return "MMM01_RAM_BATTERY";
    case CartridgeType::MBC3_TIMER_BATTERY:
        return "MBC3_TIMER_BATTERY";
    case CartridgeType::MBC3_TIMER_RAM_BATTERY:
        return "MBC3_TIMER_RAM_BATTERY";
    case CartridgeType::MBC3:
        return "MBC3";
    case CartridgeType::MBC3_RAM:
        return "MBC3_RAM";
    case CartridgeType::MBC3_RAM_BATTERY:
        return "MBC3_RAM_BATTERY";
    case CartridgeType::MBC4:
        return "MBC4";
    case CartridgeType::MBC4_RAM:
        return "MBC4_RAM";
    case CartridgeType::MBC4_RAM_BATTERY:
        return "MBC4_RAM_BATTERY";
    case CartridgeType::MBC5:
        return "MBC5";
    case CartridgeType::MBC5_RAM:
        return "MBC5_RAM";
    case CartridgeType::MBC5_RAM_BATTERY:
        return "MBC5_RAM_BATTERY";
    case CartridgeType::MBC5_RUMBLE:
        return "MBC5_RUMBLE";
    case CartridgeType::MBC5_RUMBLE_RAM:
        return "MBC5_RUMBLE_RAM";
    case CartridgeType::MBC5_RUMBLE_RAM_BATTERY:
        return "MBC5_RUMBLE_RAM_BATTERY";
    case CartridgeType::POCKET_CAMERA:
        return "POCKET_CAMERA";
    case CartridgeType::BANDAI_TAMA5:
        return "BANDAI_TAMA5";
    case CartridgeType::HUC3:
        return "HUC3";
    case CartridgeType::HUC1_RAM_BATTERY:
        return "HUC1_RAM_BATTERY";
    default:
        return "UNKNOWN";
    }
}

void Cartridge::Header::print() const {
    std::cout << "Title: " << this->title << "\n"
              << "CGB Flag: " << std::to_string(this->cgb_flag) << "\n"
              << "SGB Flag: " << std::to_string(this->sgb_flag) << "\n"
              << "Cartridge Type: " << to_string(this->cartridge_type) << "\n"
              << "ROM Size: " << std::to_string(this->rom_size) << "\n"
              << "RAM Size: " << std::to_string(this->ram_size) << "\n"
              << "Header Checksum: " << std::to_string(this->header_checksum) << "\n"
              << "Global Checksum: " << std::to_string(this->checksum) << "\n";
}

} // namespace boyboy::cartridge