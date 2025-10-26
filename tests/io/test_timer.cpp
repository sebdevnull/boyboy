/**
 * @file test_timer.cpp
 * @brief Tests for I/O Timer.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>

// boyboy
#include "boyboy/core/cpu/interrupts.h"
#include "boyboy/core/io/io.h"
#include "boyboy/core/io/registers.h"
#include "boyboy/core/io/timer.h"

using boyboy::core::cpu::Interrupts;
using boyboy::core::io::Io;
using boyboy::core::io::IoReg;
using boyboy::core::io::Timer;

class IoTimerTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        io_    = std::make_shared<Io>();
        timer_ = std::make_shared<Timer>();

        io_->register_component(timer_);
        io_->init();

        // Start with clean DIV counter (0)
        reset_div();
    }

    // Helpers because I'm feeling lazy
    uint8_t read_reg(uint16_t reg) { return timer_->read(reg); }
    void write_reg(uint16_t reg, uint8_t val) { timer_->write(reg, val); }
    uint8_t read_div() { return read_reg(IoReg::Timer::DIV); }
    uint8_t read_tima() { return read_reg(IoReg::Timer::TIMA); }
    uint8_t read_tma() { return read_reg(IoReg::Timer::TMA); }
    uint8_t read_tac() { return read_reg(IoReg::Timer::TAC); }
    void reset_div() { write_reg(IoReg::Timer::DIV, 0xFF); }
    void write_tima(uint8_t val) { write_reg(IoReg::Timer::TIMA, val); }
    void write_tma(uint8_t val) { write_reg(IoReg::Timer::TMA, val); }
    void write_tac(uint8_t val) { write_reg(IoReg::Timer::TAC, val); }

    std::shared_ptr<Io> io_;
    std::shared_ptr<Timer> timer_;

    static constexpr uint8_t Tma                 = 0xAB;
    static constexpr uint8_t OverflowDelayCycles = Timer::InterruptDelay + Timer::TimaReloadDelay;
};

TEST_F(IoTimerTest, InitialState)
{
    timer_->reset();
    EXPECT_EQ(read_div(), Timer::DivStartValue);
    EXPECT_EQ(read_tima(), 0);
    EXPECT_EQ(read_tma(), 0);
    EXPECT_EQ(read_tac(), 0);
}

TEST_F(IoTimerTest, DivResetsOnWrite)
{
    reset_div(); // Writing any value resets DIV
    EXPECT_EQ(read_div(), 0);

    timer_->tick(Timer::Frequency::DivIncrement);
    EXPECT_EQ(read_div(), 1);

    reset_div();
    EXPECT_EQ(read_div(), 0);
}

TEST_F(IoTimerTest, DivIncrements)
{
    // Ensure we start at 0
    EXPECT_EQ(read_div(), 0);

    // Tick until 1 left for increment
    timer_->tick(Timer::Frequency::DivIncrement - 1);
    EXPECT_EQ(read_div(), 0);

    // Complete DIV period
    timer_->tick(1);
    EXPECT_EQ(read_div(), 1);

    // Increase by 2
    timer_->tick(Timer::Frequency::DivIncrement * 2);
    EXPECT_EQ(read_div(), 3);
}

TEST_F(IoTimerTest, TimaIncrementsWhenEnabled)
{
    // Set TAC to enable timer with 4096 Hz (1024 cycles per increment)
    write_tac(Timer::Flags::TimerEnable | Timer::Flags::Clock256M);
    write_tima(0);
    write_tma(0xAB);
    EXPECT_EQ(read_tima(), 0);

    // Tick until 1 left for increment
    timer_->tick(Timer::Frequency::Tima256M - 1);
    EXPECT_EQ(read_tima(), 0);

    // Complete TIMA period
    timer_->tick(1);
    EXPECT_EQ(read_tima(), 1);

    // Increase by 2
    timer_->tick(Timer::Frequency::Tima256M * 2);
    EXPECT_EQ(read_tima(), 3);
}

TEST_F(IoTimerTest, TimaDoesNotIncrementWhenDisabled)
{
    // Set TAC to disable timer
    write_tac(Timer::Flags::Clock256M);
    write_tima(0);
    EXPECT_EQ(read_tima(), 0);

    timer_->tick(Timer::Frequency::Tima256M * 4);
    EXPECT_EQ(read_tima(), 0);
}

// After TIMA overflows, we need 4 extra T-cycles to trigger the interrupt and another 4 to reload
// TIMA with TMA
TEST_F(IoTimerTest, TimaOverflowsToTma)
{
    // Set TAC to enable timer with 4096 Hz (1024 cycles per increment)
    write_tac(Timer::Flags::TimerEnable | Timer::Flags::Clock256M);
    write_tima(0xFE);
    write_tma(Tma);
    EXPECT_EQ(read_tima(), 0xFE);
    EXPECT_FALSE(io_->read(IoReg::Interrupts::IF) & Interrupts::Timer);

    // Increment TIMA to 0xFF
    timer_->tick(Timer::Frequency::Tima256M);
    EXPECT_EQ(read_tima(), 0xFF);
    EXPECT_FALSE(io_->read(IoReg::Interrupts::IF) & Interrupts::Timer);

    // Overflow TIMA, no interrupt or reload yet
    timer_->tick(Timer::Frequency::Tima256M);
    EXPECT_EQ(read_tima(), 0);
    EXPECT_FALSE(io_->read(IoReg::Interrupts::IF) & Interrupts::Timer);

    // Advance 4 cycles, interrupt should be requested; no reload yet
    timer_->tick(Timer::InterruptDelay);
    EXPECT_EQ(read_tima(), 0);
    EXPECT_TRUE(io_->read(IoReg::Interrupts::IF) & Interrupts::Timer);

    // Advance 4 more cycles, interrupt should be still be requested and TIMA reloaded with TMA
    timer_->tick(Timer::TimaReloadDelay);
    EXPECT_EQ(read_tima(), Tma);
    EXPECT_TRUE(io_->read(IoReg::Interrupts::IF) & Interrupts::Timer);
}

TEST_F(IoTimerTest, TimaMultipleOverflows)
{
    int irq_count = 0;
    timer_->set_interrupt_cb([&irq_count](uint8_t) { irq_count++; });

    // Set TAC to enable timer with 4096 Hz (1024 cycles per increment)
    write_tac(Timer::Flags::TimerEnable | Timer::Flags::Clock256M);
    write_tima(0xFF);
    write_tma(Tma);
    EXPECT_EQ(read_tima(), 0xFF);

    // Tick enough cycles to cause multiple overflows
    timer_->tick(Timer::Frequency::Tima256M + OverflowDelayCycles); // 1 increment, should overflow
    write_tima(0xFF); // Set TIMA to 0xFF to cause overflow on next increment
    timer_->tick(Timer::Frequency::Tima256M * 5); // 5 increments, should overflow once
    EXPECT_EQ(read_tima(), 0xAB + 4);             // Should overflow to TMA and increment
    EXPECT_EQ(irq_count, 2);                      // Interrupt should have been requested twice
}

// Edge conditions for TIMA overflow
TEST_F(IoTimerTest, TimaOverflowEdgeCases)
{
    int irq_count = 0;
    timer_->set_interrupt_cb([&irq_count](uint8_t) { irq_count++; });

    // Initial timer conditions
    auto init_timer = [&]() {
        timer_->reset();
        irq_count = 0;
        reset_div();
        write_tac(Timer::Flags::TimerEnable | Timer::Flags::Clock4M);
        write_tima(0xFF);
        write_tma(Tma);
    };

    // Writing TIMA during the first 4 delay cycles should cancel interrupt and reload
    {
        init_timer();

        // Tick until overflow happens
        timer_->tick(timer_->get_frequency());
        EXPECT_EQ(read_tima(), 0);
        EXPECT_EQ(irq_count, 0);

        // Write TIMA during delay
        write_tima(0xFF);
        EXPECT_EQ(read_tima(), 0xFF);
        EXPECT_EQ(irq_count, 0);

        // Advance delay period, nothing should happen
        timer_->tick(OverflowDelayCycles);
        EXPECT_EQ(read_tima(), 0xFF);
        EXPECT_EQ(irq_count, 0);
    }

    // Writing TIMA during the second 4 delay cycles should ignore write
    {
        init_timer();

        // Tick until overflow happens
        timer_->tick(timer_->get_frequency());
        EXPECT_EQ(read_tima(), 0);
        EXPECT_EQ(irq_count, 0);

        // Tick first delay cycle, interrupt should be triggered
        timer_->tick(Timer::InterruptDelay);
        EXPECT_EQ(read_tima(), 0);
        EXPECT_EQ(irq_count, 1);

        // Write TIMA during second delay, nothing should happen
        write_tima(0xFF);
        EXPECT_EQ(read_tima(), 0);
        EXPECT_EQ(irq_count, 1);

        // Advance second delay period, TIMA should reload with TMA
        timer_->tick(Timer::TimaReloadDelay);
        EXPECT_EQ(read_tima(), Tma);
        EXPECT_EQ(irq_count, 1);
    }

    // Writing TMA during the second 4 delay cycles should load new TMA value
    {
        constexpr auto NewTma = static_cast<uint8_t>(~Tma);

        init_timer();

        // Tick until overflow happens
        timer_->tick(timer_->get_frequency());
        EXPECT_EQ(read_tima(), 0);
        EXPECT_EQ(irq_count, 0);

        // Tick first delay cycle, interrupt should be triggered
        timer_->tick(Timer::InterruptDelay);
        EXPECT_EQ(read_tima(), 0);
        EXPECT_EQ(irq_count, 1);

        // Write new TMA during second delay
        write_tma(NewTma);
        EXPECT_EQ(read_tima(), 0);
        EXPECT_EQ(irq_count, 1);

        // Advance second delay period, TIMA should reload with the new TMA
        timer_->tick(Timer::TimaReloadDelay);
        EXPECT_EQ(read_tima(), NewTma);
        EXPECT_EQ(irq_count, 1);
    }
}

TEST_F(IoTimerTest, TimaFrequencySettings)
{
    for (uint8_t tac = 0; tac <= 0b11; ++tac) {
        write_tac(tac | Timer::Flags::TimerEnable);
        write_tima(0);
        reset_div();
        EXPECT_EQ(read_tima(), 0);

        timer_->tick(Timer::get_frequency(tac) - 1);
        EXPECT_EQ(read_tima(), 0);

        timer_->tick(1);
        EXPECT_EQ(read_tima(), 1);
    }
}

TEST_F(IoTimerTest, StartStop)
{
    // Initially timer is running
    EXPECT_FALSE(timer_->is_stopped());
    timer_->tick(Timer::Frequency::DivIncrement);
    EXPECT_EQ(read_div(), 1);

    // Stop the timer
    timer_->stop();
    EXPECT_TRUE(timer_->is_stopped());
    timer_->tick(Timer::Frequency::DivIncrement * 5);
    EXPECT_EQ(read_div(), 0); // DIV is reset and should not increment

    // Start the timer again
    timer_->start();
    EXPECT_FALSE(timer_->is_stopped());
    timer_->tick(Timer::Frequency::DivIncrement);
    EXPECT_EQ(read_div(), 1); // DIV should increment again
}

// Timer obscure behaviour tests.
// The following tests are consequences of how the timer hardware circuit works.
// Basically, TIMA is incremented with the falling edge of an AND between TAC enable and certain
// bits of the internal system counter based on TAC selected frequency. This means two things:
//     1) Any change in DIV, TAC.enable or TAC.freq that triggers the falling edge detector, will
//     increase TIMA. i.e. TAC.enable && DIV(TAC.freq) 1 -> 0; TIMA++
//     2) If resetting DIV before the tested bit is set, TIMA will never be incremented no matter
//     how many ticks.
// For more information check Timer implementation, Pan Docs and/or TCAGBD.

// If the DIV register is resetted at a rate higher than TIMA clock, TIMA won't increase.
// This is only true if DIV reset frequency is higher than double of TIMA clock, because the tested
// bit is never set.
TEST_F(IoTimerTest, TimaResetWithDivReset)
{
    // Set TIMA clock to 16 T-cycles
    write_tac(Timer::Flags::TimerEnable | Timer::Flags::Clock4M);
    EXPECT_EQ(read_tima(), 0);

    // Tick enough to not increase TIMA
    auto cycles = (1 << timer_->get_test_bit()) - 1;
    timer_->tick(cycles);
    EXPECT_EQ(read_tima(), 0);

    // Reset DIV and tick again, which would increment TIMA if it were independent from DIV
    reset_div();
    timer_->tick(cycles);
    EXPECT_EQ(read_tima(), 0);

    // Repeat the same for all frequencies several times in a loop
    for (int clk = 0; clk <= 0b11; ++clk) {
        write_tac(Timer::Flags::TimerEnable | clk);
        auto test_bit   = timer_->get_test_bit();
        auto clk_cycles = (1 << test_bit) - 1;

        // Loop for an arbitrary N
        for (int n = 10; n > 0; --n) {
            auto acc_cycles = 0;

            reset_div();
            write_tima(0);

            // Repeat tick-reset-tick pattern
            timer_->tick(clk_cycles);
            acc_cycles += clk_cycles;
            EXPECT_EQ(read_tima(), 0);

            reset_div();
            timer_->tick(clk_cycles);
            acc_cycles += clk_cycles;
            EXPECT_EQ(read_tima(), 0);

            // Check that accumulated cycles should have triggered TIMA
            EXPECT_LE(1 << test_bit, acc_cycles);
        }
    }

    // Reset again and tick just enough to complete one TIMA increment
    reset_div();
    timer_->tick(timer_->get_frequency());
    EXPECT_EQ(read_tima(), 1);
}

// If the DIV tested bit for TIMA falls from 1 to 0 because of a DIV write, TIMA WILL increment
TEST_F(IoTimerTest, TimaIncWithDivReset)
{
    // Set clock and get test bit of the system counter
    constexpr auto Clock   = Timer::Flags::Clock4M;
    constexpr auto TestBit = Timer::get_test_bit(Clock);
    write_tac(Timer::Flags::TimerEnable | Clock);

    // Tick timer until system counter has test bit set and ensure we have valid TIMA
    timer_->tick(1 << TestBit);
    EXPECT_EQ(read_tima(), 0);

    // Reset DIV, TIMA should increment because of the falling-edge detection
    reset_div();
    EXPECT_EQ(read_div(), 0);
    EXPECT_EQ(read_tima(), 1);

    // Repeat for all clocks
    for (auto clk = 0; clk <= Timer::Flags::ClockSelectMask; ++clk) {
        // Reset state
        write_tac(Timer::Flags::TimerEnable | clk);
        reset_div();
        write_tima(0);

        // Tick
        auto test_bit = Timer::get_test_bit(clk);
        auto cycles   = 1 << test_bit;
        timer_->tick(cycles);
        EXPECT_EQ(read_tima(), 0);

        // Reset div
        reset_div();
        EXPECT_EQ(read_tima(), 1);
    }
}

// If a frequency change would cause to trigger the falling edge detector, TIMA should increment
TEST_F(IoTimerTest, TimaIncWithFreqChange)
{
    // Increase the internal system counter up to the tested bit, then change to a frequency for
    // which tested bit is 0 to trigger the falling edge detector

    // Set clock and get test bit of the system counter
    constexpr auto Clock   = Timer::Flags::Clock4M;
    constexpr auto TestBit = Timer::get_test_bit(Clock);
    write_tac(Timer::Flags::TimerEnable | Clock);

    // Tick timer until system counter has test bit set and ensure we have valid DIV and TIMA
    timer_->tick(1 << TestBit);
    EXPECT_EQ(read_div(), 0);
    EXPECT_EQ(read_tima(), 0);

    // Now set a clock that tests a different bit. That bit should be 0 and TIMA should increment
    write_tac(Timer::Flags::TimerEnable | ((Clock + 1) & Timer::Flags::ClockSelectMask));
    EXPECT_EQ(read_tima(), 1);

    // Repeat for all clocks
    for (int clk = 1; clk < 4; ++clk) {
        // Reset state
        write_tac(Timer::Flags::TimerEnable | Clock);
        reset_div();
        write_tima(0);

        // Tick and switch clock
        timer_->tick(1 << TestBit);
        write_tac(Timer::Flags::TimerEnable | ((Clock + clk) & Timer::Flags::ClockSelectMask));
        EXPECT_EQ(read_tima(), 1);
    }
}

// If a timer disable change would cause to trigger the falling edge detector, TIMA should increment
TEST_F(IoTimerTest, TimaIncWithTimerDisable)
{
    // Increase the internal system counter up to the tested bit, then disable the timer to trigger
    // the falling edge detector

    // Set clock and get test bit of the system counter
    constexpr auto Clock   = Timer::Flags::Clock4M;
    constexpr auto TestBit = Timer::get_test_bit(Clock);
    write_tac(Timer::Flags::TimerEnable | Clock);

    // Tick timer until system counter has test bit set and ensure we have valid DIV and TIMA
    timer_->tick(1 << TestBit);
    EXPECT_EQ(read_div(), 0);
    EXPECT_EQ(read_tima(), 0);

    // Disable timer to trigger detector, TIMA should increment
    write_tac(Clock);
    EXPECT_EQ(read_tima(), 1);

    // Repeat for all clocks
    for (auto clk = 0; clk <= Timer::Flags::ClockSelectMask; ++clk) {
        // Reset state
        write_tac(Timer::Flags::TimerEnable | clk);
        reset_div();
        write_tima(0);

        // Tick
        auto test_bit = Timer::get_test_bit(clk);
        auto cycles   = 1 << test_bit;
        timer_->tick(cycles);
        EXPECT_EQ(read_tima(), 0);

        // Disable timer
        write_tac(clk);
        EXPECT_EQ(read_tima(), 1) << "TIMA should be incremented after disable for " << cycles
                                  << " cycles with clock " << clk;
    }
}