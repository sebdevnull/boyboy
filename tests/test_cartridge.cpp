/**
 * @file test_cartridge.cpp
 * @brief Unit tests for Cartridge loading and parsing.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include <cstdlib>

#include "boyboy/cartridge.h"
#include "common/paths.h"
#include "common/roms.h"

using namespace boyboy::test::common;

using boyboy::cartridge::Cartridge;
using boyboy::cartridge::CartridgeType;

class CartridgeTest : public ::testing::Test {};

TEST_F(CartridgeTest, LoadValidRom)
{
    Cartridge cart;
    EXPECT_NO_THROW(cart.load_rom(local_file(ValidROM, __FILE__)));
    EXPECT_GT(cart.size(), 0);
}

TEST_F(CartridgeTest, LoadInvalidRom)
{
    Cartridge cart;
    EXPECT_THROW(cart.load_rom(local_file(InvalidROM, __FILE__)), std::runtime_error);
}

TEST_F(CartridgeTest, HeaderParsing)
{
    Cartridge cart;
    cart.load_rom(local_file(ValidROM, __FILE__));
    const auto& header = cart.get_header();

    EXPECT_EQ(header.title, "LIFE");
    EXPECT_EQ(header.cartridge_type, CartridgeType::ROMOnly);
    EXPECT_EQ(header.rom_size, 0); // 32KB
    EXPECT_EQ(header.ram_size, 0); // No RAM
}

TEST_F(CartridgeTest, UnloadRom)
{
    Cartridge cart;
    cart.load_rom(local_file(ValidROM, __FILE__));
    EXPECT_GT(cart.size(), 0);
    cart.unload_rom();
    EXPECT_EQ(cart.size(), 0);
}
