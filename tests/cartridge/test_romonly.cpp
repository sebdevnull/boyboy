/**
 * @file test_romonly.cpp
 * @brief Tests for ROM-only cartridges.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include <cstdint>

// boyboy
#include "boyboy/common/utils.h"
#include "boyboy/core/mmu/constants.h"

using boyboy::common::utils::PrettyHex;
using boyboy::core::mmu::ERAMStart;
using boyboy::core::mmu::ROMBank0End;
using boyboy::core::mmu::ROMBank0Start;
using boyboy::core::mmu::ROMBank1End;
using boyboy::core::mmu::ROMBank1Start;

// Helpers
#include "helpers/rom_fixtures.h"

using boyboy::test::rom::FakeROMTest;

class RomOnlyTest : public FakeROMTest {
protected:
    void SetUp() override
    {
        rom_data = make_fake_rom(
            boyboy::core::cartridge::CartridgeType::ROMOnly, 2, 0, "ROM_ONLY_TEST"
        );
        cart = std::make_unique<boyboy::core::cartridge::Cartridge>(
            boyboy::core::cartridge::CartridgeLoader::load(rom_data)
        );
    }
};

TEST_F(RomOnlyTest, InitialState)
{
    const auto& mbc = cart->get_mbc();

    EXPECT_EQ(cart->get_header().cartridge_type, boyboy::core::cartridge::CartridgeType::ROMOnly);
    EXPECT_EQ(mbc.get_type(), boyboy::core::cartridge::mbc::MbcType::None);
    EXPECT_EQ(mbc.rom_bank(), 1) << "Initial ROM bank should be 1";
    EXPECT_EQ(mbc.ram_bank(), 0) << "Initial RAM bank should be 0";
    EXPECT_EQ(mbc.banking_mode(), 0) << "Initial banking mode should be 0 (ROM banking)";
    EXPECT_FALSE(mbc.is_ram_enabled()) << "Initial RAM enable should be false";
}

TEST_F(RomOnlyTest, RomBankRead)
{
    // Read some values from bank 0
    uint16_t addr = ROMBank0Start;
    uint8_t val   = cart->mbc_read(addr);
    EXPECT_EQ(val, 0) << "Value read from ROM bank 0 at address " << PrettyHex(addr)
                      << " should match expected value";
    addr = ROMBank0End;
    val  = cart->mbc_read(addr);
    EXPECT_EQ(val, 0) << "Value read from ROM bank 0 at address " << PrettyHex(addr)
                      << " should match expected value";

    // Read some values from bank 1
    addr = ROMBank1Start;
    val  = cart->mbc_read(addr);
    EXPECT_EQ(val, 1) << "Value read from ROM bank 1 at address " << PrettyHex(addr)
                      << " should match expected value";
    addr = ROMBank1End;
    val  = cart->mbc_read(addr);
    EXPECT_EQ(val, 1) << "Value read from ROM bank 1 at address " << PrettyHex(addr)
                      << " should match expected value";
}

TEST_F(RomOnlyTest, RomBankWriteIgnored)
{
    // Attempt to write to ROM bank 0 (should be ignored)
    uint16_t addr = ROMBank0Start;
    cart->mbc_write(addr, 0xFF);
    uint8_t val = cart->mbc_read(addr);
    EXPECT_EQ(val, 0) << "Value read from ROM bank 0 at address " << PrettyHex(addr)
                      << " should remain unchanged after write attempt";

    // Attempt to write to ROM bank 1 (should be ignored)
    addr = ROMBank1Start;
    cart->mbc_write(addr, 0xFF);
    val = cart->mbc_read(addr);
    EXPECT_EQ(val, 1) << "Value read from ROM bank 1 at address " << PrettyHex(addr)
                      << " should remain unchanged after write attempt";
}

TEST_F(RomOnlyTest, RamAccessIgnored)
{
    // Attempt to read from external RAM (should return 0xFF)
    uint16_t addr = ERAMStart;
    uint8_t val   = cart->mbc_read(addr);
    EXPECT_EQ(val, 0xFF) << "Value read from external RAM at address " << PrettyHex(addr)
                         << " should be 0xFF for ROM-only cartridge";

    // Attempt to write to external RAM (should be ignored)
    cart->mbc_write(addr, 0x55);
    val = cart->mbc_read(addr);
    EXPECT_EQ(val, 0xFF) << "Value read from external RAM at address " << PrettyHex(addr)
                         << " should remain 0xFF after write attempt for ROM-only cartridge";
}
