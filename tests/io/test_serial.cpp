/**
 * @file test_serial.cpp
 * @brief Unit tests for serial I/O functionality in the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include "boyboy/core/io/io.h"
#include "boyboy/core/mmu/mmu.h"

using boyboy::core::io::Io;
using Serial = boyboy::core::io::IoReg::Serial;

class SerialIOTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        buffer_.str(""); // Clear the buffer
        buffer_.clear();
        io_.reset();
        io_.serial().set_output_stream(buffer_);
    }

    std::ostringstream buffer_;
    Io io_;
};

TEST_F(SerialIOTest, WritesCharacterToSB)
{
    io_.write(Serial::SB, 'H');
    io_.write(Serial::SB, 'i');
    EXPECT_EQ(buffer_.str(), "Hi");
}

// test through mmu
TEST(SerialMMUTest, WritesCharacterToSB)
{
    std::ostringstream buffer;
    boyboy::core::mmu::Mmu mmu;

    auto* cout_buf = std::cout.rdbuf(buffer.rdbuf()); // Redirect std::cout to buffer

    mmu.write_byte(Serial::SB, 'H');
    mmu.write_byte(Serial::SB, 'i');

    std::cout.rdbuf(cout_buf); // Restore original std::cout buffer

    EXPECT_EQ(buffer.str(), "Hi");
}