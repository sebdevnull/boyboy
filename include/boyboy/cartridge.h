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

enum class CartridgeType : uint8_t {
    ROMOnly = 0x00,
    MBC1 = 0x01,
    MBC1RAM = 0x02,
    MBC1RAMBattery = 0x03,
    MBC2 = 0x05,
    MBC2Battery = 0x06,
    ROMRAM = 0x08,
    ROMRAMBattery = 0x09,
    MMM01 = 0x0B,
    MMM01RAM = 0x0C,
    MMM01RAMBattery = 0x0D,
    MBC3TimerBattery = 0x0F,
    MBC3TimerRAMBattery = 0x10,
    MBC3 = 0x11,
    MBC3RAM = 0x12,
    MBC3RAMBattery = 0x13,
    MBC4 = 0X15,
    MBC4RAM = 0x16,
    MBC4RAMBattery = 0x17,
    MBC5 = 0X19,
    MBC5RAM = 0x1A,
    MBC5RAMBattery = 0x1B,
    MBC5Rumble = 0x1C,
    MBC5RumbleRAM = 0x1D,
    MBC5RumbleRAMBattery = 0x1E,
    PocketCamera = 0xFC,
    BandaiTama5 = 0xFD,
    HUC3 = 0xFE,
    HUC1RAMBattery = 0xFF
};

[[nodiscard]] std::string_view to_string(CartridgeType type);

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

        void reset() { *this = Header{}; }
        void print() const;

        [[nodiscard]] std::string to_string() const;
        [[nodiscard]] std::string pretty_string() const;

        // Header field constants
        static constexpr uint16_t HeaderStart = 0x134;
        static constexpr uint16_t HeaderEnd = 0x14C;
        static constexpr uint16_t TitlePos = 0x134;
        static constexpr uint16_t TitleLen = 16;
        static constexpr uint16_t TitleEnd = TitlePos + TitleLen;
        static constexpr uint16_t CGBFlagPos = 0x143;
        static constexpr uint16_t SGBFlagPos = 0x146;
        static constexpr uint16_t CartridgeTypePos = 0x147;
        static constexpr uint16_t ROMSizePos = 0x148;
        static constexpr uint16_t RAMSizePos = 0x149;
        static constexpr uint16_t HeaderChecksumPos = 0x14D;
        static constexpr uint16_t ChecksumPos = 0x14E;
    };

    Cartridge() = default;
    Cartridge(std::string_view path) { load_rom(path); }

    // ROM loading
    void load_rom(std::string_view path);
    void unload_rom();
    [[nodiscard]] bool is_loaded() const { return rom_loaded_; }

    // Accessors
    [[nodiscard]] const std::vector<std::byte>& get_rom() const { return rom_; }
    [[nodiscard]] const std::byte* data() const { return rom_.data(); }
    [[nodiscard]] size_t size() const { return rom_.size(); }
    [[nodiscard]] const Header& get_header() const { return header_; };

private:
    Header header_{};
    std::vector<std::byte> rom_;
    bool rom_loaded_ = false;

    void load(std::string_view path);
    void unload();
    void parse_header();
    uint8_t header_checksum();
    uint16_t checksum();
};

} // namespace boyboy::cartridge