/**
 * @file test_mmu.cpp
 * @brief Unit tests for MMU functionality.
 *
 * @license GPLv3 (see LICENSE file)
 */

// TODO: write tests for read-only, mirrored, etc.

#include <gtest/gtest.h>

#include "boyboy/common/utils.h"
#include "boyboy/mmu.h"

using namespace boyboy::mmu;
using namespace boyboy::utils;

TEST(MmuTest, ReadWriteByte)
{
    Mmu mmu;
    uint16_t address = 0x1234;
    uint8_t value    = 0xAB;

    mmu.write_byte(address, value);
    EXPECT_EQ(mmu.read_byte(address), value);
}

TEST(MmuTest, ReadWriteWord)
{
    Mmu mmu;
    uint16_t address = 0x1234;
    uint16_t value   = 0xABCD;

    mmu.write_word(address, value);
    EXPECT_EQ(mmu.read_word(address), value);
}

TEST(MmuTest, Copy)
{
    Mmu mmu;
    uint16_t address            = 0x2000;
    std::array<uint8_t, 4> data = {0xDE, 0xAD, 0xBE, 0xEF};

    mmu.copy(address, data);
    for (size_t i = 0; i < data.size(); i++) {
        EXPECT_EQ(mmu.read_byte(address + i), data.at(i));
    }
}

TEST(MmuTest, WriteByteReadWord)
{
    Mmu mmu;
    uint16_t address = 0x3000;
    uint16_t word    = 0x1234;

    mmu.write_byte(address, lsb(word));
    mmu.write_byte(address + 1, msb(word));
    EXPECT_EQ(mmu.read_word(address), word);
}

TEST(MmuTest, WriteWordReadByte)
{
    Mmu mmu;
    uint16_t address = 0x4000;
    uint16_t word    = 0x5678;

    mmu.write_word(address, word);
    EXPECT_EQ(mmu.read_byte(address), lsb(word));
    EXPECT_EQ(mmu.read_byte(address + 1), msb(word));
}