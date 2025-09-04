/**
 * @file cartridge.h
 * @brief Cartridge handling for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace boyboy::cartridge {

enum class CartridgeType {
    ROM_ONLY = 0x00,
    MBC1 = 0x01,
    MBC1_RAM = 0x02,
    MBC1_RAM_BATTERY = 0x03,
    MBC2 = 0x05,
    MBC2_BATTERY = 0x06,
    ROM_RAM = 0x08,
    ROM_RAM_BATTERY = 0x09,
    MMM01 = 0x0B,
    MMM01_RAM = 0x0C,
    MMM01_RAM_BATTERY = 0x0D,
    MBC3_TIMER_BATTERY = 0x0F,
    MBC3_TIMER_RAM_BATTERY = 0x10,
    MBC3 = 0x11,
    MBC3_RAM = 0x12,
    MBC3_RAM_BATTERY = 0x13,
    MBC4 = 0X15,
    MBC4_RAM = 0x16,
    MBC4_RAM_BATTERY = 0x17,
    MBC5 = 0X19,
    MBC5_RAM = 0x1A,
    MBC5_RAM_BATTERY = 0x1B,
    MBC5_RUMBLE = 0x1C,
    MBC5_RUMBLE_RAM = 0x1D,
    MBC5_RUMBLE_RAM_BATTERY = 0x1E,
    POCKET_CAMERA = 0xFC,
    BANDAI_TAMA5 = 0xFD,
    HUC3 = 0xFE,
    HUC1_RAM_BATTERY = 0xFF
};

constexpr std::string_view to_string(CartridgeType type);

class Cartridge {
public:
    struct Header {
        std::string title;
        uint8_t cgb_flag;
        uint8_t sgb_flag;
        CartridgeType cartridge_type;
        uint8_t rom_size;
        uint8_t ram_size;
        uint8_t header_checksum;
        uint16_t checksum;

        void print() const;

        // Header field constants
        static constexpr uint16_t kTitlePos = 0x134;
        static constexpr uint16_t kTitleLen = 16;
        static constexpr uint16_t kTitleEnd = kTitlePos + kTitleLen;
        static constexpr uint16_t kCGBFlagPos = 0x143;
        static constexpr uint16_t kSGBFlagPos = 0x146;
        static constexpr uint16_t kCartridgeTypePos = 0x147;
        static constexpr uint16_t kROMSizePos = 0x148;
        static constexpr uint16_t kRAMSizePos = 0x149;
        static constexpr uint16_t kHeaderChecksumPos = 0x14D;
        static constexpr uint16_t kChecksumPos = 0x14E;
    };

    void load(std::string_view path);
    void unload();

    void parse_header();

    [[nodiscard]] const std::vector<std::byte>& get_rom() const { return rom; }
    [[nodiscard]] const std::byte* data() const { return rom.data(); }
    [[nodiscard]] size_t size() const { return rom.size(); }
    [[nodiscard]] const Header& get_header() const { return header; };

private:
    Header header{};
    std::vector<std::byte> rom;
};

} // namespace boyboy::cartridge