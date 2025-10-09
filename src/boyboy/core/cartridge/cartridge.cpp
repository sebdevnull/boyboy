/**
 * @file cartridge.cpp
 * @brief Cartridge handling for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/core/cartridge/cartridge.h"

#include <cstdint>
#include <format>
#include <iostream>
#include <numeric>
#include <stdexcept>

#include "boyboy/core/cartridge/mbc.h"
#include "boyboy/common/errors.h"
#include "boyboy/common/utils.h"
#include "boyboy/common/log/logging.h"

namespace boyboy::core::cartridge {

using namespace boyboy::common;

void Cartridge::load_rom(RomData&& rom_data)
{
    rom_data_ = std::move(rom_data);
    load_rom();
}

void Cartridge::load_rom()
{
    parse_header();

    if (auto cks = validate_header_checksum(); cks != 0) {
        unload_rom();
        throw errors::ChecksumError("header", header_.header_checksum, cks);
    }

    if (!is_cart_supported()) {
        auto cart_type = header_.cartridge_type;
        unload_rom();
        throw std::runtime_error(std::format(
            "Unsupported cartridge type: {} ({})",
            to_string(cart_type),
            common::utils::PrettyHex{static_cast<uint8_t>(cart_type)}.to_string()
        ));
    }

    if (auto cks = validate_rom_checksum(); cks != 0) {
        // Game Boy hardware doesn't verify the global checksum but we will at least log a warning
        log::warn("ROM checksum validation failed, but continuing to load ROM");
    }

    mbc_.load_banks(*this);

    rom_loaded_ = true;
    log::info("Loaded ROM: {} ({} KB)", header_.title, rom_data_.size() / 1024);
    log::info("ROM Header: {}", header_.to_string());
}

void Cartridge::unload_rom()
{
    unload_rom_data();
    header_.reset();
    mbc_.unload_banks();
}

bool Cartridge::is_cart_supported() const
{
    switch (header_.cartridge_type) {
        case CartridgeType::ROMOnly:
        case CartridgeType::MBC1:
        case CartridgeType::MBC1RAM:
        // TODO: support battery-backed RAM
        case CartridgeType::MBC1RAMBattery:
            return true;
        case CartridgeType::MBC2:
        case CartridgeType::MBC2RAMBattery:
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

uint8_t Cartridge::header_checksum(const RomData& rom_data)
{
    return std::accumulate(
        rom_data.begin() + Header::HeaderStart,
        rom_data.begin() + Header::HeaderEnd + 1,
        uint8_t{0},
        [](uint8_t acc, std::byte b) { return acc - (utils::to_u8(b) + 1); }
    );
}

uint16_t Cartridge::rom_checksum(const RomData& rom_data)
{
    uint16_t cks = 0;
    cks = std::accumulate(
        rom_data.begin(),
        rom_data.end(),
        uint16_t{0},
        [](uint16_t acc, std::byte b) { return acc + common::utils::to_u8(b); }
    );

    // Don't compute the checksum bytes
    cks -= common::utils::to_u8(rom_data.at(Header::ChecksumPos));
    cks -= common::utils::to_u8(rom_data.at(Header::ChecksumPos + 1));

    return cks;
}

/**
 * @brief Calculate and verify the header checksum.
 *
 * @return uint8_t 0 if checksum matches, non-zero computed checksum otherwise.
 */
uint8_t Cartridge::validate_header_checksum()
{
    uint8_t cks = header_checksum(rom_data_);

    bool pass = cks == header_.header_checksum;
    if (!pass) {
        log::warn(
            "ROM header checksum mismatch: {} != {}",
            common::utils::PrettyHex{header_.header_checksum}.to_string(),
            common::utils::PrettyHex{cks}.to_string()
        );
    }

    return pass ? 0 : cks;
}

/**
 * @brief Calculate and verify the global ROM checksum.
 *
 * @return uint16_t 0 if checksum matches, non-zero computed checksum otherwise.
 */
uint16_t Cartridge::validate_rom_checksum()
{
    uint16_t cks = rom_checksum(rom_data_);

    bool pass = cks == header_.checksum;
    if (!pass) {
        log::warn(
            "ROM checksum mismatch: {} != {}",
            common::utils::PrettyHex{header_.checksum}.to_string(),
            common::utils::PrettyHex{cks}.to_string()
        );
    }

    return pass ? 0 : cks;
}

void Cartridge::unload_rom_data()
{
    rom_data_.clear();
    rom_data_.shrink_to_fit();
    rom_loaded_ = false;
}

void Cartridge::parse_header()
{
    header_.title.clear();
    for (auto pos = Header::TitlePos; pos < Header::TitleEnd; ++pos) {
        char cur_c = static_cast<char>(rom_data_.at(pos));
        if (cur_c == 0) {
            break;
        }
        header_.title += cur_c;
    }

    header_.cgb_flag = static_cast<uint8_t>(rom_data_.at(Header::CGBFlagPos));
    header_.sgb_flag = static_cast<uint8_t>(rom_data_.at(Header::SGBFlagPos));
    header_.cartridge_type = static_cast<CartridgeType>(rom_data_.at(Header::CartridgeTypePos));
    header_.rom_size = static_cast<RomSize>(rom_data_.at(Header::ROMSizePos));
    header_.ram_size = static_cast<RamSize>(rom_data_.at(Header::RAMSizePos));
    header_.header_checksum = static_cast<uint8_t>(rom_data_.at(Header::HeaderChecksumPos));
    header_.checksum = static_cast<uint16_t>(rom_data_.at(Header::ChecksumPos)) << 8 |
                       static_cast<uint16_t>(rom_data_.at(Header::ChecksumPos + 1));

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
        case CartridgeType::MBC2RAMBattery:
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
        case CartridgeType::MBC6RAMBattery:
            return "MBC6_RAM_BATTERY";
        case CartridgeType::MBC7RAMBatteryAccelerometer:
            return "MBC7_RAM_BATTERY_ACCELEROMETER";
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

[[nodiscard]] std::string_view to_string(RomSize size)
{
    switch (size) {
        case RomSize::KB32:
            return "32KB";
        case RomSize::KB64:
            return "64KB";
        case RomSize::KB128:
            return "128KB";
        case RomSize::KB256:
            return "256KB";
        case RomSize::KB512:
            return "512KB";
        case RomSize::MB1:
            return "1MB";
        case RomSize::MB2:
            return "2MB";
        case RomSize::MB4:
            return "4MB";
        case RomSize::MB8:
            return "8MB";
        case RomSize::MB1d1:
            return "1.1MB";
        case RomSize::MB1d2:
            return "1.2MB";
        case RomSize::MB1d5:
            return "1.5MB";
        default:
            return "Unknown";
    }
}

[[nodiscard]] uint16_t rom_size_kb(RomSize size)
{
    switch (size) {
        case RomSize::KB32:
            return 32;
        case RomSize::KB64:
            return 64;
        case RomSize::KB128:
            return 128;
        case RomSize::KB256:
            return 256;
        case RomSize::KB512:
            return 512;
        case RomSize::MB1:
            return 1024;
        case RomSize::MB2:
            return 2048;
        case RomSize::MB4:
            return 4096;
        case RomSize::MB8:
            return 8192;
        case RomSize::MB1d1:
            return 1152; // 1.1MB
        case RomSize::MB1d2:
            return 1280; // 1.2MB
        case RomSize::MB1d5:
            return 1536; // 1.5MB
        default:
            throw std::runtime_error(std::format(
                "Unknown ROM size code: {}",
                common::utils::PrettyHex{static_cast<uint8_t>(size)}.to_string()
            ));
    }
}

[[nodiscard]] uint16_t num_rom_banks(RomSize size)
{
    return rom_size_kb(size) / mbc::RomBankSizeKB; // Each bank is 16KB
}

[[nodiscard]] RomSize rom_size_from_banks(uint16_t banks)
{
    switch (banks) {
        case 2:
            return RomSize::KB32;
        case 4:
            return RomSize::KB64;
        case 8:
            return RomSize::KB128;
        case 16:
            return RomSize::KB256;
        case 32:
            return RomSize::KB512;
        case 64:
            return RomSize::MB1;
        case 128:
            return RomSize::MB2;
        case 256:
            return RomSize::MB4;
        case 512:
            return RomSize::MB8;
        case 72:
            return RomSize::MB1d1; // 1.1MB
        case 80:
            return RomSize::MB1d2; // 1.2MB
        case 96:
            return RomSize::MB1d5; // 1.5MB
        default:
            throw std::runtime_error(std::format("Unsupported number of ROM banks: {}", banks));
    }
}

[[nodiscard]] std::string_view to_string(RamSize size)
{
    switch (size) {
        case RamSize::None:
            return "None";
        case RamSize::KB2:
            return "2KB";
        case RamSize::KB8:
            return "8KB";
        case RamSize::KB32:
            return "32KB";
        case RamSize::KB128:
            return "128KB";
        case RamSize::KB64:
            return "64KB";
        default:
            return "Unknown";
    }
}

[[nodiscard]] uint16_t ram_size_kb(RamSize size)
{
    switch (size) {
        case RamSize::None:
            return 0;
        case RamSize::KB2:
            log::warn("Cartridge RAM size code 0x01 is unofficial, assuming 2KB RAM");
            return 2; // Unused, but some carts use it to indicate 2KB RAM
        case RamSize::KB8:
            return 8;
        case RamSize::KB32:
            return 32;
        case RamSize::KB128:
            return 128;
        case RamSize::KB64:
            return 64;
        default:
            throw std::runtime_error(std::format(
                "Unknown RAM size code: {}",
                common::utils::PrettyHex{static_cast<uint8_t>(size)}.to_string()
            ));
    }
}

[[nodiscard]] uint16_t num_ram_banks(RamSize size)
{
    return ram_size_kb(size) / mbc::RamBankSizeKB; // Each bank is 8KB
}

[[nodiscard]] RamSize ram_size_from_banks(uint8_t banks)
{
    switch (banks) {
        case 0:
            return RamSize::None;
        case 1:
            return RamSize::KB8;
        case 4:
            return RamSize::KB32;
        case 16:
            return RamSize::KB128;
        case 8:
            return RamSize::KB64;
        default:
            throw std::runtime_error(std::format("Unsupported number of RAM banks: {}", banks));
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
        << "rom_size: " << PrettyHex{static_cast<uint8_t>(rom_size)} << " ("
        << cartridge::to_string(rom_size) << ", " << num_rom_banks(rom_size) << " banks), "
        << "ram_size: " << PrettyHex{static_cast<uint8_t>(ram_size)} << " ("
        << cartridge::to_string(ram_size) << ", " << num_ram_banks(ram_size) << " banks), "
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
        << "ROM Size: " << PrettyHex{static_cast<uint8_t>(rom_size)} << " ("
        << cartridge::to_string(rom_size) << ", " << num_rom_banks(rom_size) << " banks)\n"
        << "RAM Size: " << PrettyHex{static_cast<uint8_t>(ram_size)} << " ("
        << cartridge::to_string(ram_size) << ", " << num_ram_banks(ram_size) << " banks)\n"
        << "Header Checksum: " << PrettyHex{header_checksum} << "\n"
        << "Global Checksum: " << PrettyHex{checksum} << "\n";

    return oss.str();
}

} // namespace boyboy::core::cartridge