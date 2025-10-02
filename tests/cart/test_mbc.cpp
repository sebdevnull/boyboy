/**
 * @file test_mbc.cpp
 * @brief Unit tests for Memory Bank Controller (MBC) functionality.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include "helpers/common.h"
#include "helpers/rom_fixtures.h"

// boyboy
#include "boyboy/cart/cartridge.h"
#include "boyboy/cart/mbc.h"
#include "boyboy/mmu/constants.h"

using boyboy::test::common::param_name;
using boyboy::test::rom::MBCParam;
using boyboy::test::rom::MBCParamTest;

using boyboy::cart::CartridgeType;
using boyboy::cart::mbc::Mbc;
using boyboy::cart::mbc::MbcType;
using boyboy::mmu::ERAMStart;
using boyboy::mmu::ROMBank1Start;
using boyboy::utils::PrettyHex;

TEST_P(MBCParamTest, InitialState)
{
    const auto& p   = GetParam();
    const auto& mbc = cart->get_mbc();

    EXPECT_EQ(cart->get_header().cartridge_type, p.type);
    if (p.type == CartridgeType::ROMOnly) {
        EXPECT_EQ(mbc.get_type(), MbcType::None);
    }
    else if (p.type == CartridgeType::MBC1 || p.type == CartridgeType::MBC1RAM) {
        EXPECT_EQ(mbc.get_type(), MbcType::MBC1);
    }
    else {
        EXPECT_EQ(mbc.get_type(), MbcType::Unsupported);
    }

    EXPECT_EQ(mbc.rom_bank(), 1) << "Initial ROM bank should be 1";
    EXPECT_EQ(mbc.ram_bank(), 0) << "Initial RAM bank should be 0";
    EXPECT_EQ(mbc.banking_mode(), 0) << "Initial banking mode should be 0 (ROM banking)";
    EXPECT_FALSE(mbc.is_ram_enabled()) << "Initial RAM enable should be false";
}

TEST_P(MBCParamTest, EnableDisableRam)
{
    const auto& mbc = cart->get_mbc();

    // Skip test if non-MBC
    if (mbc.get_type() == MbcType::None || mbc.get_type() == MbcType::Unsupported) {
        GTEST_SKIP() << "Skipping ROM bank selection test for non-MBC cartridge type";
    }

    // Initially RAM should be disabled
    EXPECT_FALSE(mbc.is_ram_enabled());

    // Enable RAM with 0x0A
    cart->mbc_write(Mbc::RAMEnableStart, 0x0A);
    EXPECT_TRUE(mbc.is_ram_enabled())
        << "RAM should be enabled after writing 0x0A to RAM enable register";

    // Disable RAM with 0x00
    cart->mbc_write(Mbc::RAMEnableStart + 0x0200, 0x00);
    EXPECT_FALSE(mbc.is_ram_enabled())
        << "RAM should be disabled after writing 0x00 to RAM enable register";

    // Enable RAM with other value
    cart->mbc_write(Mbc::RAMEnableStart + 0x01FF, 0xFA);
    EXPECT_TRUE(mbc.is_ram_enabled())
        << "RAM should be enabled after writing 0xFA to RAM enable register";

    // Any other value should disable RAM
    cart->mbc_write(Mbc::RAMEnableEnd, 0xFF);
    EXPECT_FALSE(mbc.is_ram_enabled())
        << "RAM should be disabled after writing 0xFF to RAM enable register";
}

TEST_P(MBCParamTest, BankingModeSelect)
{
    const auto& mbc = cart->get_mbc();

    // Skip test if non-MBC
    if (mbc.get_type() == MbcType::None || mbc.get_type() == MbcType::Unsupported) {
        GTEST_SKIP() << "Skipping banking mode select test for non-MBC cartridge type";
    }

    EXPECT_EQ(mbc.banking_mode(), 0) << "Initial banking mode should be 0 (ROM banking)";

    // Set to mode 1
    cart->mbc_write(Mbc::BankingModeSelectStart, 0x01);
    EXPECT_EQ(mbc.banking_mode(), 1) << "Banking mode should be 1 after writing 0x01";

    // Set back to mode 0
    cart->mbc_write(Mbc::BankingModeSelectEnd, 0x00);
    EXPECT_EQ(mbc.banking_mode(), 0) << "Banking mode should be 0 after writing 0x00";

    // Any other value should be masked to 1 bit
    cart->mbc_write(Mbc::BankingModeSelectStart + 0x100, 0xFF);
    EXPECT_EQ(mbc.banking_mode(), 1) << "Banking mode should be 1 after writing 0xFF";
    cart->mbc_write(Mbc::BankingModeSelectEnd - 0x100, 0x02);
    EXPECT_EQ(mbc.banking_mode(), 0) << "Banking mode should be 0 after writing 0x02";
}

TEST_P(MBCParamTest, RomBankSelection)
{
    const auto& p   = GetParam();
    const auto& mbc = cart->get_mbc();

    // Skip test if non-MBC
    if (mbc.get_type() == MbcType::None || mbc.get_type() == MbcType::Unsupported) {
        GTEST_SKIP() << "Skipping ROM bank selection test for non-MBC cartridge type";
    }

    // Limit to 0x1F (5 bits) to avoid banking mode select for now
    auto max_selectable = std::min(p.rom_banks - 1, 0x1F);

    EXPECT_EQ(mbc.banking_mode(), 0) << "Initial banking mode should be 0 (ROM banking)";
    EXPECT_EQ(mbc.rom_bank(), 1) << "Initial ROM bank should be 1";

    // Select some middle bank
    auto mid_bank = std::max(max_selectable / 2, 1);
    cart->mbc_write(Mbc::ROMBankNumberStart, mid_bank);
    EXPECT_EQ(mbc.rom_bank(), mid_bank)
        << "ROM bank should be " << mid_bank << " after selecting it";

    // Read from bank to verify correct bank is mapped
    uint8_t val = cart->mbc_read(ROMBank1Start);
    EXPECT_EQ(val, mid_bank) << "Reading from ROM bank should return value equal to bank number "
                             << mid_bank;

    // Select bank 0 (should map to bank 1)
    cart->mbc_write(Mbc::ROMBankNumberStart + 0x100, 0x00);
    EXPECT_EQ(mbc.rom_bank(), 1) << "ROM bank should map to 1 when selecting bank 0";
    val = cart->mbc_read(ROMBank1Start);
    EXPECT_EQ(val, 1) << "Reading from ROM bank should return value equal to bank number 1";

    // Select maximum bank
    cart->mbc_write(Mbc::ROMBankNumberEnd, max_selectable);
    EXPECT_EQ(mbc.rom_bank(), max_selectable)
        << "ROM bank should be max bank (" << max_selectable << ") after selecting it";
    val = cart->mbc_read(ROMBank1Start);
    EXPECT_EQ(val, max_selectable)
        << "Reading from ROM bank should return value equal to bank number " << max_selectable;

    // Select bank beyond max (should wrap)
    auto beyond_max = max_selectable + 2;
    cart->mbc_write(Mbc::ROMBankNumberStart + 0x200, beyond_max);
    EXPECT_EQ(mbc.rom_bank(), beyond_max - (max_selectable + 1))
        << "ROM bank should wrap around when selecting beyond max bank";
    val = cart->mbc_read(ROMBank1Start);
    EXPECT_EQ(val, beyond_max - (max_selectable + 1))
        << "Reading from ROM bank should return value equal to wrapped bank number";

    // If more than 32 banks, test upper bits in banking mode 0
    if (p.rom_banks > 0x1F) {
        // Test upper bits in banking mode 0
        cart->mbc_write(Mbc::BankingModeSelectStart, 0x00); // Ensure mode 0

        uint16_t expected_bank = p.rom_banks - 1;
        cart->mbc_write(Mbc::ROMBankNumberStart, expected_bank & 0x1F);        // lower 5 bits
        cart->mbc_write(Mbc::RAMBankNumberStart, (expected_bank >> 5) & 0x03); // upper 2 bits

        EXPECT_EQ(mbc.rom_bank(), expected_bank)
            << "ROM bank should be " << expected_bank
            << " after combining upper and lower bits in mode 0";

        val = cart->mbc_read(ROMBank1Start);
        EXPECT_EQ(val, expected_bank)
            << "Reading from ROM bank should return value equal to bank number " << expected_bank;
    }
}

TEST_P(MBCParamTest, RamBankSelection)
{
    const auto& p   = GetParam();
    const auto& mbc = cart->get_mbc();

    // Skip test if non-MBC or no RAM
    if (mbc.get_type() == MbcType::None || mbc.get_type() == MbcType::Unsupported) {
        GTEST_SKIP() << "Skipping RAM bank selection test for non-MBC cartridge type";
    }
    if (p.ram_banks == 0) {
        GTEST_SKIP() << "Skipping RAM bank selection test for cartridge with no RAM";
    }

    EXPECT_EQ(mbc.banking_mode(), 0) << "Initial banking mode should be 0 (ROM banking)";
    EXPECT_EQ(mbc.ram_bank(), 0) << "Initial RAM bank should be 0";

    // Switch to banking mode 1 to enable RAM bank switching
    cart->mbc_write(Mbc::BankingModeSelectStart, 0x01);
    EXPECT_EQ(mbc.banking_mode(), 1) << "Banking mode should be 1 after writing 0x01";

    // Select each RAM bank in turn and verify
    for (uint8_t bank = 0; bank < p.ram_banks; bank++) {
        cart->mbc_write(Mbc::RAMBankNumberStart, bank);
        EXPECT_EQ(mbc.ram_bank(), bank) << "RAM bank should be " << bank << " after selecting it";
    }

    // Select bank beyond max (should wrap)
    auto beyond_max = p.ram_banks + 2;
    cart->mbc_write(Mbc::RAMBankNumberStart + 0x100, beyond_max);
    EXPECT_EQ(mbc.ram_bank(), beyond_max % p.ram_banks)
        << "RAM bank should wrap around when selecting beyond max bank";

    // Write to each RAM bank and verify data is stored correctly
    cart->mbc_write(Mbc::RAMEnableStart, 0x0A); // Enable RAM
    EXPECT_TRUE(mbc.is_ram_enabled()) << "RAM should be enabled for RAM bank read/write test";
    for (uint8_t bank = 0; bank < p.ram_banks; bank++) {
        cart->mbc_write(Mbc::RAMBankNumberStart, bank);
        uint16_t addr = ERAMStart;
        for (uint8_t val = 0; val < 0x10; val++) {
            cart->mbc_write(addr + val, val + (bank * 0x10)); // Distinguish banks by offset
            uint8_t read_val = cart->mbc_read(addr + val);
            EXPECT_EQ(read_val, val + (bank * 0x10))
                << "Value read from RAM bank " << bank << " at address "
                << PrettyHex(static_cast<uint16_t>(addr + val)).to_string()
                << " should match value written";
        }
    }
}

INSTANTIATE_TEST_SUITE_P(MBC1Tests,
                         MBCParamTest,
                         ::testing::Values(
                             MBCParam{
                                 .type      = CartridgeType::MBC1,
                                 .rom_banks = 2,
                                 .ram_banks = 0,
                                 .name      = "MBC1_ROM2_RAM0",
                             },
                             MBCParam{
                                 .type      = CartridgeType::MBC1,
                                 .rom_banks = 16,
                                 .ram_banks = 0,
                                 .name      = "MBC1_ROM16_RAM0",
                             },
                             MBCParam{
                                 .type      = CartridgeType::MBC1RAM,
                                 .rom_banks = 16,
                                 .ram_banks = 1,
                                 .name      = "MBC1_ROM16_RAM1",
                             },
                             MBCParam{
                                 .type      = CartridgeType::MBC1RAM,
                                 .rom_banks = 32,
                                 .ram_banks = 1,
                                 .name      = "MBC1_ROM32_RAM2",
                             },
                             MBCParam{
                                 .type      = CartridgeType::MBC1RAM,
                                 .rom_banks = 64,
                                 .ram_banks = 4,
                                 .name      = "MBC1_ROM64_RAM4",
                             },
                             MBCParam{
                                 .type      = CartridgeType::MBC1RAM,
                                 .rom_banks = 128,
                                 .ram_banks = 4,
                                 .name      = "MBC1_ROM128_RAM4",
                             }),
                         param_name<MBCParam>);
