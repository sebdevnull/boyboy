/**
 * @file test_timer.cpp
 * @brief Tests for I/O Timer.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

// boyboy
#include "boyboy/core/cpu/interrupts.h"
#include "boyboy/core/io/io.h"
#include "boyboy/core/io/timer.h"

using boyboy::core::cpu::Interrupts;
using boyboy::core::io::Io;
using boyboy::core::io::IoReg;
using boyboy::core::io::Timer;

class IoTimerTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        io_.reset();
        timer_ = &io_.timer();
    }

    Io io_;
    Timer* timer_;
};

TEST_F(IoTimerTest, DivIncrements)
{
    // DIV should increment every 256 cycles
    EXPECT_EQ(timer_->read(IoReg::Timer::DIV), 0);

    timer_->tick(Timer::Frequency::DivIncrement - 1);
    EXPECT_EQ(timer_->read(IoReg::Timer::DIV), 0);

    timer_->tick(1);
    EXPECT_EQ(timer_->read(IoReg::Timer::DIV), 1);

    timer_->tick(Timer::Frequency::DivIncrement * 2);
    EXPECT_EQ(timer_->read(IoReg::Timer::DIV), 3);
}

TEST_F(IoTimerTest, TimaIncrementsWhenEnabled)
{
    // Set TAC to enable timer with 4096 Hz (1024 cycles per increment)
    timer_->write(IoReg::Timer::TAC, Timer::Flags::TimerEnable | Timer::Flags::Clock256M);
    timer_->write(IoReg::Timer::TIMA, 0);
    timer_->write(IoReg::Timer::TMA, 0xAB);
    EXPECT_EQ(timer_->read(IoReg::Timer::TIMA), 0);

    timer_->tick(Timer::Frequency::Tima256M - 1);
    EXPECT_EQ(timer_->read(IoReg::Timer::TIMA), 0);

    timer_->tick(1);
    EXPECT_EQ(timer_->read(IoReg::Timer::TIMA), 1);

    timer_->tick(Timer::Frequency::Tima256M * 2);
    EXPECT_EQ(timer_->read(IoReg::Timer::TIMA), 3);
}

TEST_F(IoTimerTest, TimaDoesNotIncrementWhenDisabled)
{
    // Set TAC to disable timer
    timer_->write(IoReg::Timer::TAC, Timer::Flags::Clock256M);
    timer_->write(IoReg::Timer::TIMA, 0);
    EXPECT_EQ(timer_->read(IoReg::Timer::TIMA), 0);

    timer_->tick(Timer::Frequency::Tima256M * 4);
    EXPECT_EQ(timer_->read(IoReg::Timer::TIMA), 0);
}

TEST_F(IoTimerTest, TimaOverflowsToTma)
{
    // Set TAC to enable timer with 4096 Hz (1024 cycles per increment)
    timer_->write(IoReg::Timer::TAC, Timer::Flags::TimerEnable | Timer::Flags::Clock256M);
    timer_->write(IoReg::Timer::TIMA, 0xFE);
    timer_->write(IoReg::Timer::TMA, 0xAB);
    EXPECT_EQ(timer_->read(IoReg::Timer::TIMA), 0xFE);
    EXPECT_FALSE(io_.read(IoReg::Interrupts::IF) & Interrupts::Timer);

    timer_->tick(Timer::Frequency::Tima256M);
    EXPECT_EQ(timer_->read(IoReg::Timer::TIMA), 0xFF);
    EXPECT_FALSE(io_.read(IoReg::Interrupts::IF) & Interrupts::Timer);

    timer_->tick(Timer::Frequency::Tima256M);
    EXPECT_EQ(timer_->read(IoReg::Timer::TIMA), 0xAB); // Should overflow to TMA
    EXPECT_TRUE(io_.read(IoReg::Interrupts::IF) & Interrupts::Timer);
}

TEST_F(IoTimerTest, TimaMultipleOverflows)
{
    // Set TAC to enable timer with 4096 Hz (1024 cycles per increment)
    timer_->write(IoReg::Timer::TAC, Timer::Flags::TimerEnable | Timer::Flags::Clock256M);
    timer_->write(IoReg::Timer::TIMA, 0xFF);
    timer_->write(IoReg::Timer::TMA, 0xAB);
    EXPECT_EQ(timer_->read(IoReg::Timer::TIMA), 0xFF);
    EXPECT_FALSE(io_.read(IoReg::Interrupts::IF) & Interrupts::Timer);

    int count = 0;
    timer_->set_interrupt_cb([&count](uint8_t) { count++; });

    // Tick enough cycles to cause multiple overflows
    timer_->tick(Timer::Frequency::Tima256M); // 1 increment, should overflow
    timer_->write(IoReg::Timer::TIMA, 0xFF); // Set TIMA to 0xFF to cause overflow on next increment
    timer_->tick(Timer::Frequency::Tima256M * 5);          // 5 increments, should overflow once
    EXPECT_EQ(timer_->read(IoReg::Timer::TIMA), 0xAB + 4); // Should overflow to TMA and increment
    EXPECT_EQ(count, 2); // Interrupt should have been requested twice
}

TEST_F(IoTimerTest, TimaFrequencySettings)
{
    for (uint8_t tac = 0; tac <= 0b11; ++tac) {
        timer_->write(IoReg::Timer::TAC, tac | Timer::Flags::TimerEnable);
        timer_->write(IoReg::Timer::TIMA, 0);
        EXPECT_EQ(timer_->read(IoReg::Timer::TIMA), 0);

        timer_->tick(Timer::get_frequency(tac) - 1);
        EXPECT_EQ(timer_->read(IoReg::Timer::TIMA), 0);

        timer_->tick(1);
        EXPECT_EQ(timer_->read(IoReg::Timer::TIMA), 1);
    }
}

TEST_F(IoTimerTest, DivResetsOnWrite)
{
    timer_->write(IoReg::Timer::DIV, 0xFF); // Writing any value resets DIV
    EXPECT_EQ(timer_->read(IoReg::Timer::DIV), 0);

    timer_->tick(Timer::Frequency::DivIncrement);
    EXPECT_EQ(timer_->read(IoReg::Timer::DIV), 1);
}

TEST_F(IoTimerTest, StartStop)
{
    // Initially timer is running
    EXPECT_FALSE(timer_->is_stopped());
    timer_->tick(Timer::Frequency::DivIncrement);
    EXPECT_EQ(timer_->read(IoReg::Timer::DIV), 1);

    // Stop the timer
    timer_->stop();
    EXPECT_TRUE(timer_->is_stopped());
    timer_->tick(Timer::Frequency::DivIncrement * 5);
    EXPECT_EQ(timer_->read(IoReg::Timer::DIV), 0); // DIV is reset and should not increment

    // Start the timer again
    timer_->start();
    EXPECT_FALSE(timer_->is_stopped());
    timer_->tick(Timer::Frequency::DivIncrement);
    EXPECT_EQ(timer_->read(IoReg::Timer::DIV), 1); // DIV should increment again
}