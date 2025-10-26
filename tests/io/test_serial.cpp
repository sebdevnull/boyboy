/**
 * @file test_serial.cpp
 * @brief Unit tests for serial I/O functionality in the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include <memory>

#include "boyboy/core/io/io.h"
#include "boyboy/core/io/serial.h"
#include "boyboy/core/mmu/mmu.h"

using boyboy::core::io::Io;
using boyboy::core::io::Serial;
using SerialReg = boyboy::core::io::IoReg::Serial;

class SerialIOTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        buffer_.str(""); // Clear the buffer
        buffer_.clear();

        io_     = std::make_shared<Io>();
        serial_ = std::make_shared<Serial>(buffer_);
        io_->register_component(serial_);

        io_->init();
    }

    std::ostringstream buffer_;
    std::shared_ptr<Io> io_;
    std::shared_ptr<Serial> serial_;
};

TEST_F(SerialIOTest, WritesCharacterToSB)
{
    io_->write(SerialReg::SB, 'H');
    io_->write(SerialReg::SB, 'i');
    EXPECT_EQ(buffer_.str(), "Hi");
}

// test through mmu
TEST_F(SerialIOTest, WritesCharacterToMmu)
{
    // std::ostringstream buffer;
    boyboy::core::mmu::Mmu mmu{io_};
    mmu.init();

    // auto* cout_buf = std::cout.rdbuf(buffer.rdbuf()); // Redirect std::cout to buffer

    mmu.write_byte(SerialReg::SB, 'H');
    mmu.write_byte(SerialReg::SB, 'i');

    // std::cout.rdbuf(cout_buf); // Restore original std::cout buffer

    EXPECT_EQ(buffer_.str(), "Hi");
}