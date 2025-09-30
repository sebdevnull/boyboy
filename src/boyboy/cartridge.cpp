/**
 * @file cartridge.cpp
 * @brief Cartridge handling for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/cartridge.h"

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <format>
#include <fstream>
#include <ios>
#include <iostream>
#include <stdexcept>

#include "boyboy/common/errors.h"
#include "boyboy/common/utils.h"
#include "boyboy/log/logging.h"

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

void Cartridge::load_rom(std::string_view path)
{
    load(path);
    parse_header();

    if (auto cks = header_checksum(); cks != 0) {
        unload_rom();
        throw errors::ChecksumError("header", header_.header_checksum, cks);
    }

    if (!is_cart_supported()) {
        auto cart_type = header_.cartridge_type;
        unload_rom();
        throw std::runtime_error(
            std::format("Unsupported cartridge type: {} ({})",
                        to_string(cart_type),
                        utils::PrettyHex{static_cast<uint8_t>(cart_type)}.to_string()));
    }

    if (auto cks = checksum(); cks != 0) {
        unload_rom();
        throw errors::ChecksumError("global", header_.checksum, cks);
    }
}

void Cartridge::unload_rom()
{
    unload();
    header_.reset();
}

bool Cartridge::is_cart_supported() const
{
    switch (header_.cartridge_type) {
    case CartridgeType::ROMOnly:
        return true;
    case CartridgeType::MBC1:
    case CartridgeType::MBC1RAM:
    case CartridgeType::MBC1RAMBattery:
    case CartridgeType::MBC2:
    case CartridgeType::MBC2Battery:
    case CartridgeType::ROMRAM:
    case CartridgeType::ROMRAMBattery:
    case CartridgeType::MBC3TimerBattery:
    case CartridgeType::MBC3TimerRAMBattery:
    case CartridgeType::MBC3:
    case CartridgeType::MBC3RAM:
    case CartridgeType::MBC3RAMBattery:
    case CartridgeType::MBC5:
    case CartridgeType::MBC5RAM:
    case CartridgeType::MBC5RAMBattery:
    case CartridgeType::MBC5Rumble:
    case CartridgeType::MBC5RumbleRAM:
    case CartridgeType::MBC5RumbleRAMBattery:
    default:
        return false;
    }
}

/**
 * @brief Calculate and verify the header checksum.
 *
 * @return uint8_t 0 if checksum matches, non-zero computed checksum otherwise.
 */
uint8_t Cartridge::header_checksum()
{
    uint8_t cks = 0;
    std::ranges::for_each(rom_.begin() + Header::HeaderStart,
                          rom_.begin() + Header::HeaderEnd + 1,
                          [&cks](auto b) { cks -= utils::to_u8(b) + 1; });

    bool pass = cks == header_.header_checksum;
    if (!pass) {
        log::warn("ROM header checksum mismatch: {} != {}",
                  utils::PrettyHex{header_.header_checksum}.to_string(),
                  utils::PrettyHex{cks}.to_string());
    }

    return pass ? 0 : cks;
}

/**
 * @brief Calculate and verify the global ROM checksum.
 *
 * @return uint16_t 0 if checksum matches, non-zero computed checksum otherwise.
 */
uint16_t Cartridge::checksum()
{
    uint16_t cks = 0;
    std::ranges::for_each(rom_.begin(), rom_.end(), [&cks](auto b) { cks += utils::to_u8(b); });

    // Don't compute the checksum bytes
    cks -= utils::msb(header_.checksum) + utils::lsb(header_.checksum);

    bool pass = cks == header_.checksum;
    if (!pass) {
        log::warn("ROM checksum mismatch: {} != {}",
                  utils::PrettyHex{header_.checksum}.to_string(),
                  utils::PrettyHex{cks}.to_string());
    }

    return pass ? 0 : cks;
}

/**
 * @brief Load a ROM from the specified file path.
 *
 * It loads into local memory, doesn't send to MMU yet.
 *
 * @param path Path to the ROM file.
 */
void Cartridge::load(std::string_view path)
{
    namespace fs = std::filesystem;

    fs::path file_path(path);

    if (!fs::exists(file_path)) {
        log::debug("Current path: {}", fs::current_path().string());
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

    rom_loaded_ = true;
}

void Cartridge::unload()
{
    rom_.clear();
    rom_.shrink_to_fit();
    rom_loaded_ = false;
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

    log::debug("Header loaded: {}", header_.to_string());
}

std::string_view to_string(CartridgeType type)
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

[[nodiscard]] std::string Cartridge::Header::to_string() const
{
    using namespace utils;
    std::ostringstream oss;

    oss << "{title: " << title << ", "
        << "cbg_flag: " << PrettyHex{cgb_flag} << ", "
        << "sgb_flag: " << PrettyHex{sgb_flag} << ", "
        << "cart_Type: " << cartridge::to_string(cartridge_type).data() << ", "
        << "rom_size: " << PrettyHex{rom_size} << ", "
        << "ram_size: " << PrettyHex{ram_size} << ", "
        << "header_cks: " << PrettyHex{header_checksum} << ", "
        << "cks: " << PrettyHex{checksum} << "}";

    return oss.str();
}

[[nodiscard]] std::string Cartridge::Header::pretty_string() const
{
    using namespace utils;
    std::ostringstream oss;

    oss << "Title: " << title << "\n"
        << "CGB Flag: " << PrettyHex{cgb_flag} << "\n"
        << "SGB Flag: " << PrettyHex{sgb_flag} << "\n"
        << "Cartridge Type: " << cartridge::to_string(cartridge_type).data() << "\n"
        << "ROM Size: " << PrettyHex{rom_size} << "\n"
        << "RAM Size: " << PrettyHex{ram_size} << "\n"
        << "Header Checksum: " << PrettyHex{header_checksum} << "\n"
        << "Global Checksum: " << PrettyHex{checksum} << "\n";

    return oss.str();
}

} // namespace boyboy::cartridge