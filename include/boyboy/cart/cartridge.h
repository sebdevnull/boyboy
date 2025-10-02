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

#include "boyboy/cart/mbc.h"

namespace boyboy::cart {

enum class CartridgeType : uint8_t {
    ROMOnly = 0x00,
    MBC1 = 0x01,
    MBC1RAM = 0x02,
    MBC1RAMBattery = 0x03,
    MBC2 = 0x05,
    MBC2RAMBattery = 0x06,
    ROMRAM = 0x08,        // Not used by any game
    ROMRAMBattery = 0x09, // Not used by any game
    MMM01 = 0x0B,
    MMM01RAM = 0x0C,
    MMM01RAMBattery = 0x0D,
    MBC3TimerBattery = 0x0F,
    MBC3TimerRAMBattery = 0x10,
    MBC3 = 0x11,
    MBC3RAM = 0x12,
    MBC3RAMBattery = 0x13,
    MBC5 = 0X19,
    MBC5RAM = 0x1A,
    MBC5RAMBattery = 0x1B,
    MBC5Rumble = 0x1C,
    MBC5RumbleRAM = 0x1D,
    MBC5RumbleRAMBattery = 0x1E,
    MBC6RAMBattery = 0x20,
    MBC7RAMBatteryAccelerometer = 0x22,
    PocketCamera = 0xFC,
    BandaiTama5 = 0xFD,
    HUC3 = 0xFE,
    HUC1RAMBattery = 0xFF
};

[[nodiscard]] std::string_view to_string(CartridgeType type);

enum class RomSize : uint8_t {
    KB32 = 0x00,  // 32KB (2 banks, no MBC)
    KB64 = 0x01,  // 64KB (4 banks)
    KB128 = 0x02, // 128KB (8 banks)
    KB256 = 0x03, // 256KB (16 banks)
    KB512 = 0x04, // 512KB (32 banks)
    MB1 = 0x05,   // 1MB (64 banks)
    MB2 = 0x06,   // 2MB (128 banks)
    MB4 = 0x07,   // 4MB (256 banks)
    MB8 = 0x08,   // 8MB (512 banks)
    MB1d1 = 0x52, // 1.1MB (72 banks)
    MB1d2 = 0x53, // 1.2MB (80 banks)
    MB1d5 = 0x54  // 1.5MB (96 banks)
};
[[nodiscard]] std::string_view to_string(RomSize size);
[[nodiscard]] uint16_t rom_size_kb(RomSize size);
[[nodiscard]] uint16_t num_rom_banks(RomSize size);
[[nodiscard]] RomSize rom_size_from_banks(uint16_t banks);

enum class RamSize : uint8_t {
    None = 0x00,  // No RAM
    KB2 = 0x01,   // 2KB RAM (unofficial)
    KB8 = 0x02,   // 8KB RAM (1 bank)
    KB32 = 0x03,  // 32KB RAM (4 banks)
    KB128 = 0x04, // 128KB RAM (16 banks)
    KB64 = 0x05   // 64KB RAM (8 banks)
};
[[nodiscard]] std::string_view to_string(RamSize size);
[[nodiscard]] uint16_t ram_size_kb(RamSize size);
[[nodiscard]] uint16_t num_ram_banks(RamSize size);
[[nodiscard]] RamSize ram_size_from_banks(uint8_t banks);

using RomData = std::vector<std::byte>;

class Cartridge {
public:
    struct Header {
        std::string title;
        uint8_t cgb_flag;
        uint8_t sgb_flag;
        CartridgeType cartridge_type;
        RomSize rom_size;
        RamSize ram_size;
        uint8_t header_checksum;
        uint16_t checksum;

        void reset() { *this = Header{}; }
        void print() const;

        // Printable string representations
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

    // Constructors
    // It's recommended to use CartridgeLoader to load ROMs. However, several constructors are
    // provided for convenience and testing.
    // Default constructor creates an empty cartridge. Use load_rom(RomData&&) to load a ROM later.
    Cartridge() = default;
    Cartridge(RomData&& rom_data) : rom_data_(std::move(rom_data)) { load_rom(); }
    Cartridge(const RomData& rom_data) : rom_data_(rom_data) { load_rom(); }

    // ROM loading
    void load_rom(RomData&& rom_data);
    void unload_rom();
    [[nodiscard]] bool is_loaded() const { return rom_loaded_; }
    [[nodiscard]] bool is_cart_supported() const;

    // Accessors
    [[nodiscard]] const RomData& get_rom_data() const { return rom_data_; }
    [[nodiscard]] const Header& get_header() const { return header_; };
    [[nodiscard]] const mbc::Mbc& get_mbc() const { return mbc_; }

    // MBC access
    [[nodiscard]] uint8_t mbc_read(uint16_t addr) const { return mbc_.read(addr); }
    void mbc_write(uint16_t addr, uint8_t value) { mbc_.write(addr, value); }

private:
    Header header_{};
    mbc::Mbc mbc_;
    RomData rom_data_;
    bool rom_loaded_ = false;

    void load_rom();
    void unload_rom_data();
    void parse_header();
    uint8_t header_checksum();
    uint16_t checksum();
};

} // namespace boyboy::cart