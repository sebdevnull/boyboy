/**
 * @file mbc.cpp
 * @brief Memory Bank Controller (MBC) implementation for BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/core/cartridge/mbc.h"

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <format>
#include <ranges>
#include <vector>

#include "boyboy/common/log/logging.h"
#include "boyboy/common/utils.h"
#include "boyboy/core/cartridge/cartridge.h"
#include "boyboy/core/mmu/constants.h"

namespace boyboy::core::cartridge::mbc {

using namespace boyboy::common;

void Mbc::load_banks(const Cartridge& cart)
{
    // Unload any existing banks first and reset state
    unload_banks();

    // Get MBC type and check if supported
    type_ = mbc_type(cart);
    if (type_ != MbcType::None && type_ != MbcType::MBC1) {
        // Currently only ROM_ONLY and MBC1 are supported
        throw std::runtime_error(std::format(
            "Unsupported MBC type: {}", cartridge::to_string(cart.get_header().cartridge_type)
        ));
    }
    has_battery_ = cart_has_battery(cart);

    // Get bank count from cartridge header
    const auto& header = cart.get_header();
    rom_bank_cnt_ = cartridge::num_rom_banks(header.rom_size);
    ram_bank_cnt_ = cartridge::num_ram_banks(header.ram_size);

    rom_banks_.resize(rom_bank_cnt_);

    // Transform std::byte vector to uint8_t view
    auto bytes_to_uint8 = std::views::transform([](std::byte byte) {
        return static_cast<uint8_t>(byte);
    });

    // Split ROM into 16KB chunks and copy into rom_banks_
    auto rom_chunks = cart.get_rom_data() | bytes_to_uint8 | std::views::chunk(RomBankSize);
    int bank_idx = 0;
    for (const auto& chunk : rom_chunks) {
        std::ranges::copy(chunk, rom_banks_.at(bank_idx).begin());
        bank_idx++;
    }

    // Initialize RAM banks to 0
    ram_banks_.resize(ram_bank_cnt_, {0});

    log::info(
        "MBC initialized: type={}, ROM banks={}, RAM banks={}",
        to_string(type_),
        rom_bank_cnt_,
        ram_bank_cnt_
    );
}

void Mbc::unload_banks()
{
    // Clear save state
    clear_save();

    // Reset banks and registers
    rom_banks_.clear();
    rom_banks_.shrink_to_fit();
    ram_banks_.clear();
    ram_banks_.shrink_to_fit();
    rom_bank_cnt_ = 0;
    ram_bank_cnt_ = 0;
    rom_bank_select_ = 1;
    ram_bank_select_ = 0;
    banking_mode_ = 0;
    ram_enable_ = false;

    // Reset status
    type_ = MbcType::None;
    has_battery_ = false;
}

[[nodiscard]] uint8_t Mbc::read(uint16_t addr) const
{
    if (addr >= mmu::ROMBank0Start && addr <= mmu::ROMBank0End) {
        return rom_banks_.at(0).at(addr - mmu::ROMBank0Start);
    }
    if (rom_bank_cnt_ > 1 && addr >= mmu::ROMBank1Start && addr <= mmu::ROMBank1End) {
        return selected_rom_bank().at(addr - mmu::ROMBank1Start);
    }
    if (ram_enable_ && ram_bank_cnt_ > 0 && addr >= mmu::SRAMStart && addr <= mmu::SRAMEnd) {
        return selected_ram_bank().at(addr - mmu::SRAMStart);
    }

    // return openbus otherwise
    return 0xFF;
}

void Mbc::write(uint16_t addr, uint8_t value)
{
    if (type_ != MbcType::MBC1) {
        // Currently only MBC1 is supported
        log::warn(
            "Ignoring write to unsupported MBC type at {}: {}",
            common::utils::PrettyHex(addr).to_string(),
            common::utils::PrettyHex(value).to_string()
        );
        return;
    }

    if (addr <= RAMEnableEnd) {
        // enable only on 0x0A, disable otherwise
        // discard upper 4 bits
        ram_enable_ = (value & 0x0F) == 0x0A;
        log::trace("RAM enable set to {}", ram_enable_);
    }
    else if (addr >= ROMBankNumberStart && addr <= ROMBankNumberEnd) {
        // Mask value to max number of banks or 5-bits max
        // e.g. 16 banks (0x10) masked to 4 bits (0x0F)
        value &= std::min((rom_bank_cnt_ - 1), 0x1F);
        rom_bank_select_ = value == 0 ? 1 : value; // if 0 always set to 1
        log::trace("ROM bank selected: {}", rom_bank_select_);
    }
    else if (addr >= RAMBankNumberStart && addr <= RAMBankNumberEnd) {
        // 2 bit register
        value &= 0x03;
        if (banking_mode_ == 0) {
            // in mode 0 use as 2 upper bits for ROM selection
            rom_bank_select_ = (rom_bank_select_ & 0x1F) | (value << 5);
            ram_bank_select_ = 0; // always bank 0 in mode 0

            // Mask to available banks
            if (rom_bank_select_ >= rom_bank_cnt_) {
                rom_bank_select_ %= rom_bank_cnt_;
                if (rom_bank_select_ == 0) {
                    rom_bank_select_ = 1; // never bank 0
                }
            }

            log::trace(
                "ROM/RAM banking mode 0: ROM bank selected: {}, RAM bank selected: {}",
                rom_bank_select_,
                ram_bank_select_
            );
        }
        else {
            // in mode 1 select RAM bank
            ram_bank_select_ = value;
            rom_bank_select_ &= 0x1F; // only lower 5 bits valid in mode 1

            // Mask to available banks
            if (ram_bank_select_ >= ram_bank_cnt_) {
                ram_bank_select_ %= ram_bank_cnt_;
            }

            log::trace(
                "ROM/RAM banking mode 1: ROM bank selected: {}, RAM bank selected: {}",
                rom_bank_select_,
                ram_bank_select_
            );
        }
    }
    else if (addr >= BankingModeSelectStart && addr <= BankingModeSelectEnd) {
        // 1 bit register
        banking_mode_ = value & 0x01;
        log::trace("Banking mode set to {}", banking_mode_);
    }
    else if (addr >= mmu::SRAMStart && addr <= mmu::SRAMEnd && ram_bank_cnt_ > 0) {
        // write to RAM bank if any
        selected_ram_bank().at(addr - mmu::SRAMStart) = value;
        sram_dirty_ = true;
    }
    // else ignore
}

[[nodiscard]] std::vector<uint8_t> Mbc::get_ram() const
{
    auto ram_view = std::views::join(ram_banks_);
    return {ram_view.begin(), ram_view.end()};
}

void Mbc::set_ram(std::span<const uint8_t> ram)
{
    if (ram.size() != ram_size()) {
        log::error(
            "Error setting RAM of different sizes: got {}B, expected {}B", ram.size(), ram_size()
        );
        return;
    }

    for (size_t i = 0; i < ram_banks_.size(); ++i) {
        auto bank_view = ram | std::views::drop(i * RamBankSize) | std::views::take(RamBankSize);
        std::ranges::copy(bank_view, ram_banks_[i].begin());
    }
}

void Mbc::tick()
{
    if (has_battery_ && sram_dirty_ && !save_pending_) {
        auto now = BatteryClock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_save_);
        if (elapsed.count() >= save_interval_ms_) {
            save_pending_ = true;
            log::debug("[MBC] Pending SRAM save");
        }
    }
}

MbcType Mbc::mbc_type(const Cartridge& cart)
{
    switch (cart.get_header().cartridge_type) {
        case CartridgeType::ROMOnly:
            return MbcType::None;
        case CartridgeType::MBC1:
        case CartridgeType::MBC1RAM:
        case CartridgeType::MBC1RAMBattery:
            return MbcType::MBC1;
        case CartridgeType::MBC2:
        case CartridgeType::MBC2RAMBattery:
            return MbcType::MBC2;
        case CartridgeType::MBC3:
        case CartridgeType::MBC3RAM:
        case CartridgeType::MBC3RAMBattery:
        case CartridgeType::MBC3TimerBattery:
        case CartridgeType::MBC3TimerRAMBattery:
            return MbcType::MBC3;
        case CartridgeType::MBC5:
        case CartridgeType::MBC5RAM:
        case CartridgeType::MBC5RAMBattery:
        case CartridgeType::MBC5Rumble:
        case CartridgeType::MBC5RumbleRAM:
        case CartridgeType::MBC5RumbleRAMBattery:
            return MbcType::MBC5;
        case CartridgeType::MBC6RAMBattery:
            return MbcType::MBC6;
        case CartridgeType::MBC7RAMBatteryAccelerometer:
            return MbcType::MBC7;
        default:
            return MbcType::Unsupported;
    }
}

bool Mbc::cart_has_battery(const Cartridge& cart)
{
    switch (cart.get_header().cartridge_type) {
        case CartridgeType::MBC1RAMBattery:
        case CartridgeType::MBC2RAMBattery:
        case CartridgeType::MBC3RAMBattery:
        case CartridgeType::MBC3TimerBattery:
        case CartridgeType::MBC3TimerRAMBattery:
        case CartridgeType::MBC5RAMBattery:
        case CartridgeType::MBC5RumbleRAMBattery:
        case CartridgeType::MBC6RAMBattery:
            return true;
        default:
            return false;
    }
}

[[nodiscard]] const char* to_string(MbcType type)
{
    switch (type) {
        case MbcType::None:
            return "None";
        case MbcType::MBC1:
            return "MBC1";
        case MbcType::MBC2:
            return "MBC2";
        case MbcType::MBC3:
            return "MBC3";
        case MbcType::MBC5:
            return "MBC5";
        case MbcType::MBC6:
            return "MBC6";
        case MbcType::MBC7:
            return "MBC7";
        case MbcType::Unsupported:
            return "Unsupported";
        default:
            return "Unknown";
    }
}

} // namespace boyboy::core::cartridge::mbc