/**
 * @file test_cartridge.cpp
 * @brief Unit tests for Cartridge loading and parsing.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include <cstdlib>

#include "common/roms.h"
#include "helpers/rom_fixtures.h"

// boyboy
#include "boyboy/core/cartridge/cartridge.h"

using boyboy::test::common::InvalidROM;
using boyboy::test::common::ValidROM;
using boyboy::test::rom::ROMTest;

using boyboy::core::cartridge::CartridgeType;
using boyboy::core::cartridge::RamSize;
using boyboy::core::cartridge::RomSize;

class CartridgeTest : public ROMTest {};

TEST_F(CartridgeTest, LoadValidRom)
{
    EXPECT_NO_THROW(load(ValidROM));
    EXPECT_GT(cart.get_rom_data().size(), 0);
    EXPECT_TRUE(cart.is_loaded());
}

TEST_F(CartridgeTest, LoadInvalidRom)
{
    EXPECT_THROW(load(InvalidROM), std::runtime_error);
    EXPECT_FALSE(cart.is_loaded());
}

TEST_F(CartridgeTest, HeaderParsing)
{
    load(ValidROM);
    const auto& header = cart.get_header();

    EXPECT_EQ(header.title, "LIFE");
    EXPECT_EQ(header.cartridge_type, CartridgeType::ROMOnly);
    EXPECT_EQ(header.rom_size, RomSize::KB32); // 32KB
    EXPECT_EQ(header.ram_size, RamSize::None); // No RAM
}

TEST_F(CartridgeTest, UnloadRom)
{
    load(ValidROM);
    EXPECT_GT(cart.get_rom_data().size(), 0);
    cart.unload_rom();
    EXPECT_EQ(cart.get_rom_data().size(), 0);
    EXPECT_FALSE(cart.is_loaded());
}
