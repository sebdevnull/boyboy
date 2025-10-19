/**
 * @file mbc.h
 * @brief Memory Bank Controller (MBC) for BoyBoy emulator.
 *
 * Supports ROM Only and MBC1 types.
 *
 * ------------------------------------- MBC1 Registers ------------------------------------
 * 0x0000-0x1FFF - RAM Enable (Write Only)
 *                 0x0A - Enable RAM
 *                 Must be enabled before writing to ERAM
 * 0x2000-0x3FFF - ROM Bank Number (Write Only)
 *                 Lower 5 bits select the ROM bank number (0-31), higher bits discarded
 *                 Writing 0 selects bank 1 instead (0 is fixed)
 *                 Writing 0x20, 0x40, 0x60 selects banks 0x21, 0x41, 0x61 respectively
 * 0x4000-0x5FFF - RAM Bank Number or Upper Bits of ROM Bank Number (Write Only)
 *                 2 bits for RAM bank number (0-3) or upper 2 bits of ROM bank number (1MiB+ carts)
 * 0x6000-0x7FFF - Banking Mode Select (Write Only)
 *                 0 - ROM Banking Mode:
 *                     Upper 2 bits of 0x4000-0x5FFF are used as upper bits of ROM bank number.
 *                     0x0000-0x3FFF - Fixed ROM Bank 0
 *                     0x4000-0x7FFF - Switchable ROM Bank (0x01-0x7F as selected by ROM Bank
 * Number) 0xA000-0xBFFF - Fixed RAM Bank 0 1 - RAM Banking Mode: Upper 2 bits of 0x4000-0x5FFF are
 * used as RAM bank number. 0x0000-0x3FFF - Fixed ROM Bank 0 0x4000-0x7FFF - Switchable ROM Bank
 * (0x01-0x1F as selected by ROM Bank Number) 0xA000-0xBFFF - Switchable RAM Bank (as selected by
 * RAM Bank Number)
 * ---------------------------------------------------------------------------------------
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <array>
#include <chrono>
#include <cstdint>
#include <vector>

// Forward declaration
namespace boyboy::core::cartridge {
class Cartridge;
}

namespace boyboy::core::cartridge::mbc {

// MBC types
enum class MbcType : uint8_t {
    None,
    MBC1,
    MBC2,
    MBC3,
    MBC5,
    MBC6,
    MBC7,
    Unsupported,
};
[[nodiscard]] const char* to_string(MbcType type);

// MBC constants
static constexpr uint16_t RomBankSizeKB = 16;                 // 16KB
static constexpr uint16_t RomBankSize = RomBankSizeKB * 1024; // 16KB (in bytes)
static constexpr uint16_t RamBankSizeKB = 8;                  // 8KB
static constexpr uint16_t RamBankSize = RamBankSizeKB * 1024; // 8KB (in bytes)

// Type aliases for ROM and RAM banks
using RomBank = std::array<uint8_t, RomBankSize>;
using RamBank = std::array<uint8_t, RamBankSize>;

class Mbc {
public:
    // Bank management
    void load_banks(const Cartridge& cart);
    void unload_banks();

    // Memory access
    [[nodiscard]] uint8_t read(uint16_t addr) const;
    void write(uint16_t addr, uint8_t value);

    // Full ERAM data access
    [[nodiscard]] std::vector<uint8_t> get_ram() const;
    void set_ram(std::span<const uint8_t> ram);

    // Tick for battery-backed RAM saves and RTC
    void tick();

    // Save state
    [[nodiscard]] bool is_save_pending() const { return save_pending_; };
    void clear_save()
    {
        eram_dirty_ = false;
        save_pending_ = false;
        last_save_ = BatteryClock::now();
    }
    [[nodiscard]] uint32_t get_save_interval_ms() const { return save_interval_ms_; }
    void set_save_interval_ms(uint32_t interval_ms) { save_interval_ms_ = interval_ms; }

    // Accessors
    [[nodiscard]] MbcType get_type() const { return type_; }
    [[nodiscard]] bool is_ram_enabled() const { return ram_enable_; }
    [[nodiscard]] uint8_t rom_bank() const { return rom_bank_select_; }
    [[nodiscard]] uint8_t ram_bank() const { return ram_bank_select_; }
    [[nodiscard]] uint8_t banking_mode() const { return banking_mode_; }
    [[nodiscard]] bool has_battery() const { return has_battery_; }
    [[nodiscard]] size_t ram_size() const { return ram_banks_.size() * RamBankSize; }

    // Registers address ranges
    static constexpr uint16_t RAMEnableStart = 0x0000;
    static constexpr uint16_t RAMEnableEnd = 0x1FFF;
    static constexpr uint16_t ROMBankNumberStart = 0x2000;
    static constexpr uint16_t ROMBankNumberEnd = 0x3FFF;
    static constexpr uint16_t RAMBankNumberStart = 0x4000;
    static constexpr uint16_t RAMBankNumberEnd = 0x5FFF;
    static constexpr uint16_t BankingModeSelectStart = 0x6000;
    static constexpr uint16_t BankingModeSelectEnd = 0x7FFF;

private:
    using BatteryClock = std::chrono::steady_clock;
    static constexpr uint32_t DefaultSaveIntervalMs = 5000;

    // MBC status and information
    MbcType type_{MbcType::None};

    // Battery status
    bool has_battery_{false};
    bool eram_dirty_{false};
    bool save_pending_{false};
    uint32_t save_interval_ms_ = DefaultSaveIntervalMs;
    BatteryClock::time_point last_save_{BatteryClock::now()};

    // MBC registers
    bool ram_enable_{false};     // RAM enable flag
    uint8_t rom_bank_select_{1}; // ROM bank number (default to 1, as bank 0 is fixed)
    uint8_t ram_bank_select_{0}; // RAM bank number
    uint8_t banking_mode_{0};    // 0 = simple, 1 = advanced

    // ROM and RAM banks
    std::vector<RomBank> rom_banks_;
    std::vector<RamBank> ram_banks_;
    uint8_t rom_bank_cnt_{0};
    uint8_t ram_bank_cnt_{0};

    // ROM bank selection helpers
    RomBank& selected_rom_bank() { return rom_banks_.at(rom_bank_select_); }
    [[nodiscard]] const RomBank& selected_rom_bank() const
    {
        return rom_banks_.at(rom_bank_select_);
    }
    RamBank& selected_ram_bank() { return ram_banks_.at(ram_bank_select_); }
    [[nodiscard]] const RamBank& selected_ram_bank() const
    {
        return ram_banks_.at(ram_bank_select_);
    }

    // Cartridge type helpers
    static MbcType mbc_type(const Cartridge& cart);
    static bool cart_has_battery(const Cartridge& cart);
};

} // namespace boyboy::core::cartridge::mbc