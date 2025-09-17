/**
 * @file test_interrupts.cpp
 * @brief Tests for CPU interrupts.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

// boyboy
#include "boyboy/cpu/cpu.h"
#include "boyboy/cpu/cpu_constants.h"
#include "boyboy/cpu/interrupt_handler.h"
#include "boyboy/cpu/interrupts.h"

// Helpers
#include "helpers/cpu_fixtures.h"

using boyboy::cpu::Interrupts;
using boyboy::cpu::InterruptVectors;
using boyboy::cpu::PCStartValue;
using boyboy::cpu::SPStartValue;

using boyboy::test::cpu::CpuTest;

class CpuInterruptsTest : public CpuTest {};

TEST_F(CpuInterruptsTest, EiDi)
{
    // Test enabling and disabling interrupts
    cpu.set_ime(false);
    EXPECT_FALSE(cpu.get_ime());
    cpu.set_ime(true);
    EXPECT_TRUE(cpu.get_ime());
    cpu.set_ime(false);
    EXPECT_FALSE(cpu.get_ime());
}

TEST_F(CpuInterruptsTest, EiSchedulesIME)
{
    // Test that EI schedules IME to be enabled after next instruction
    cpu.set_ime(false);
    EXPECT_FALSE(cpu.get_ime());

    // Execute EI
    set_next_instruction(boyboy::cpu::Opcode::EI);
    cpu.step();
    EXPECT_FALSE(cpu.get_ime()); // IME should still be false immediately after EI

    // Execute a NOP to trigger IME enabling
    set_next_instruction(boyboy::cpu::Opcode::NOP);
    cpu.step();
    EXPECT_TRUE(cpu.get_ime()); // IME should now be true
}

TEST_F(CpuInterruptsTest, EnableInterrupts)
{
    // Test enabling and disabling specific interrupts
    auto& interrupt_handler = cpu.get_interrupt_handler();

    // Initially, no interrupts should be enabled
    for (uint8_t i = 0; i < 5; ++i) {
        EXPECT_FALSE(interrupt_handler.is_enabled(1 << i))
            << "Interrupt " << static_cast<int>(i) << " should be disabled initially";
    }

    // Enable all interrupts
    for (uint8_t i = 0; i < 5; ++i) {
        cpu.enable_interrupt(1 << i);
        EXPECT_TRUE(interrupt_handler.is_enabled(1 << i))
            << "Interrupt " << static_cast<int>(i) << " should be enabled";
    }
}

TEST_F(CpuInterruptsTest, RequestInterrupts)
{
    // Test requesting specific interrupts
    auto& interrupt_handler = cpu.get_interrupt_handler();

    // Initially, no interrupts should be requested
    for (uint8_t i = 0; i < 5; ++i) {
        EXPECT_FALSE(interrupt_handler.is_requested(1 << i))
            << "Interrupt " << static_cast<int>(i) << " should not be requested initially";
    }

    // Request all interrupts
    for (uint8_t i = 0; i < 5; ++i) {
        cpu.request_interrupt(1 << i);
        EXPECT_TRUE(interrupt_handler.is_requested(1 << i))
            << "Interrupt " << static_cast<int>(i) << " should be requested";
    }
}

TEST_F(CpuInterruptsTest, VBlankInterrupt)
{
    // Initial PC and SP values (redundant but explicit for clarity)
    cpu.set_pc(PCStartValue);
    cpu.set_sp(SPStartValue);

    // Test that V-Blank interrupt is handled correctly
    cpu.set_ime(true);
    cpu.enable_interrupt(Interrupts::VBlank);  // Enable V-Blank interrupt
    cpu.request_interrupt(Interrupts::VBlank); // Request V-Blank interrupt

    auto& interrupt_handler = cpu.get_interrupt_handler();
    EXPECT_TRUE(interrupt_handler.is_enabled(Interrupts::VBlank))
        << "V-Blank interrupt should be enabled";
    EXPECT_TRUE(interrupt_handler.is_requested(Interrupts::VBlank))
        << "V-Blank interrupt should be requested";

    interrupt_handler.service();

    EXPECT_FALSE(interrupt_handler.is_requested(Interrupts::VBlank))
        << "V-Blank interrupt should be cleared after servicing";
    EXPECT_TRUE(interrupt_handler.is_enabled(Interrupts::VBlank))
        << "V-Blank interrupt should still be enabled after servicing";
    EXPECT_FALSE(cpu.get_ime()) << "IME should be disabled after servicing interrupt";
    EXPECT_EQ(cpu.get_pc(), InterruptVectors::VBlank) << "PC should jump to V-Blank vector";
    EXPECT_EQ(cpu.get_sp(), SPStartValue - 2) << "SP should be decremented by 2";
    EXPECT_EQ(cpu.read_word(cpu.get_sp()), PCStartValue) << "Original PC should be pushed to stack";
    EXPECT_TRUE(cpu.is_halted() == false) << "CPU should not be in HALT state";
}

TEST_F(CpuInterruptsTest, NoInterruptWhenIMEOff)
{
    // Test that no interrupt is serviced when IME is off
    cpu.set_ime(false);
    cpu.enable_interrupt(Interrupts::VBlank);  // Enable V-Blank interrupt
    cpu.request_interrupt(Interrupts::VBlank); // Request V-Blank interrupt

    auto& interrupt_handler = cpu.get_interrupt_handler();
    EXPECT_TRUE(interrupt_handler.is_enabled(Interrupts::VBlank));
    EXPECT_TRUE(interrupt_handler.is_requested(Interrupts::VBlank));

    interrupt_handler.service();

    EXPECT_TRUE(interrupt_handler.is_requested(Interrupts::VBlank))
        << "Interrupt should still be requested";
    EXPECT_TRUE(interrupt_handler.is_enabled(Interrupts::VBlank))
        << "Interrupt should still be enabled";
    EXPECT_FALSE(cpu.get_ime()) << "IME should still be false";
    EXPECT_EQ(cpu.get_pc(), PCStartValue) << "PC should not change";
    EXPECT_EQ(cpu.get_sp(), SPStartValue) << "SP should not change";
}

TEST_F(CpuInterruptsTest, NoInterruptWhenNoneRequested)
{
    // Test that no interrupt is serviced when none are requested
    cpu.set_ime(true);
    cpu.enable_interrupt(Interrupts::VBlank); // Enable V-Blank interrupt

    auto& interrupt_handler = cpu.get_interrupt_handler();
    EXPECT_TRUE(interrupt_handler.is_enabled(Interrupts::VBlank));
    EXPECT_FALSE(interrupt_handler.is_requested(Interrupts::VBlank));

    interrupt_handler.service();

    EXPECT_FALSE(interrupt_handler.is_requested(Interrupts::VBlank))
        << "No interrupt should be requested";
    EXPECT_TRUE(interrupt_handler.is_enabled(Interrupts::VBlank))
        << "Interrupt should still be enabled";
    EXPECT_TRUE(cpu.get_ime()) << "IME should still be true";
    EXPECT_EQ(cpu.get_pc(), PCStartValue) << "PC should not change";
    EXPECT_EQ(cpu.get_sp(), SPStartValue) << "SP should not change";
}

TEST_F(CpuInterruptsTest, MultipleInterrupts)
{
    // Test that only the highest priority interrupt is serviced
    cpu.set_pc(PCStartValue);
    cpu.set_sp(SPStartValue);
    cpu.set_ime(true);
    cpu.enable_interrupt(Interrupts::VBlank);  // Enable V-Blank interrupt
    cpu.enable_interrupt(Interrupts::LCDStat); // Enable LCD STAT interrupt

    // Request both interrupts
    cpu.request_interrupt(Interrupts::VBlank);  // V-Blank
    cpu.request_interrupt(Interrupts::LCDStat); // LCD STAT

    auto& interrupt_handler = cpu.get_interrupt_handler();
    EXPECT_TRUE(interrupt_handler.is_enabled(Interrupts::VBlank)) << "V-Blank should be enabled";
    EXPECT_TRUE(interrupt_handler.is_enabled(Interrupts::LCDStat)) << "LCD STAT should be enabled";
    EXPECT_TRUE(interrupt_handler.is_requested(Interrupts::VBlank))
        << "V-Blank should be requested";
    EXPECT_TRUE(interrupt_handler.is_requested(Interrupts::LCDStat))
        << "LCD STAT should be requested";

    interrupt_handler.service();

    EXPECT_FALSE(interrupt_handler.is_requested(Interrupts::VBlank))
        << "V-Blank should now be cleared";
    EXPECT_TRUE(interrupt_handler.is_requested(Interrupts::LCDStat))
        << "LCD STAT should still be requested";
    EXPECT_TRUE(interrupt_handler.is_enabled(Interrupts::VBlank))
        << "V-Blank should still be enabled";
    EXPECT_TRUE(interrupt_handler.is_enabled(Interrupts::LCDStat))
        << "LCD STAT should still be enabled";
    EXPECT_FALSE(cpu.get_ime()) << "IME should be disabled after servicing";
    EXPECT_EQ(cpu.get_pc(), InterruptVectors::VBlank) << "PC should jump to V-Blank vector";
    EXPECT_EQ(cpu.get_sp(), SPStartValue - 2) << "SP should be decremented by 2";
    EXPECT_EQ(cpu.read_word(cpu.get_sp()), PCStartValue) << "Original PC should be pushed to stack";

    // Service again to handle LCD STAT
    cpu.set_ime(true); // Re-enable IME for next interrupt
    interrupt_handler.service();

    EXPECT_FALSE(interrupt_handler.is_requested(Interrupts::LCDStat))
        << "LCD STAT should now be cleared";
    EXPECT_TRUE(interrupt_handler.is_enabled(Interrupts::VBlank))
        << "V-Blank should still be enabled";
    EXPECT_TRUE(interrupt_handler.is_enabled(Interrupts::LCDStat))
        << "LCD STAT should still be enabled";
    EXPECT_FALSE(cpu.get_ime()) << "IME should be disabled after servicing";
    EXPECT_EQ(cpu.get_pc(), InterruptVectors::LCDStat) << "PC should jump to LCD STAT vector";
    EXPECT_EQ(cpu.get_sp(), SPStartValue - 4) << "SP should be decremented by another 2";
    EXPECT_EQ(cpu.read_word(cpu.get_sp()), InterruptVectors::VBlank)
        << "Previous PC should be pushed to stack";
}

TEST_F(CpuInterruptsTest, HaltThenInterrupt)
{
    // Test that CPU exits HALT state when an interrupt is requested
    cpu.set_pc(PCStartValue);
    cpu.set_sp(SPStartValue);
    cpu.set_ime(true);
    cpu.enable_interrupt(Interrupts::VBlank); // Enable V-Blank interrupt

    // Enter HALT state (PC += 1 for HALT instruction)
    set_next_instruction(boyboy::cpu::Opcode::HALT);
    cpu.step();
    EXPECT_TRUE(cpu.is_halted());

    // Request V-Blank interrupt
    cpu.request_interrupt(Interrupts::VBlank);

    auto& interrupt_handler = cpu.get_interrupt_handler();
    EXPECT_TRUE(interrupt_handler.is_enabled(Interrupts::VBlank))
        << "V-Blank interrupt should be enabled";
    EXPECT_TRUE(interrupt_handler.is_requested(Interrupts::VBlank))
        << "V-Blank interrupt should be requested";

    // Service interrupts
    interrupt_handler.service();

    EXPECT_FALSE(interrupt_handler.is_requested(Interrupts::VBlank))
        << "Interrupt should be cleared after servicing";
    EXPECT_TRUE(interrupt_handler.is_enabled(Interrupts::VBlank))
        << "Interrupt should still be enabled after servicing";
    EXPECT_FALSE(cpu.get_ime()) << "IME should be disabled after servicing interrupt";
    EXPECT_EQ(cpu.get_pc(), InterruptVectors::VBlank) << "PC should jump to V-Blank vector";
    EXPECT_EQ(cpu.get_sp(), SPStartValue - 2) << "SP should be decremented by 2";
    EXPECT_EQ(cpu.read_word(cpu.get_sp()), PCStartValue + 1)
        << "Original PC+1 should be pushed to stack";
    EXPECT_FALSE(cpu.is_halted()) << "CPU should exit HALT state";
}
