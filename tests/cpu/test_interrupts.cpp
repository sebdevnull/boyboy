/**
 * @file test_interrupts.cpp
 * @brief Tests for CPU interrupts.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include <utility>

// boyboy
#include "boyboy/core/cpu/cpu.h"
#include "boyboy/core/cpu/cycles.h"
#include "boyboy/core/cpu/interrupt_handler.h"
#include "boyboy/core/cpu/interrupts.h"
#include "boyboy/core/cpu/state.h"

// Helpers
#include "helpers/cpu_fixtures.h"

using boyboy::core::cpu::Interrupt;
using boyboy::core::cpu::InterruptVector;
using boyboy::core::cpu::Stage;
using boyboy::core::cpu::TickMode;

using boyboy::test::cpu::CpuTest;

class CpuInterruptsTest : public CpuTest {};

TEST_F(CpuInterruptsTest, EiDi)
{
    // Test enabling and disabling interrupts
    cpu->set_ime(false);
    EXPECT_FALSE(cpu->get_ime());
    cpu->set_ime(true);
    EXPECT_TRUE(cpu->get_ime());
    cpu->set_ime(false);
    EXPECT_FALSE(cpu->get_ime());
}

TEST_F(CpuInterruptsTest, EiSchedulesIME)
{
    set_next_bytes(
        {std::to_underlying(boyboy::core::cpu::Opcode::EI),
         std::to_underlying(boyboy::core::cpu::Opcode::NOP)}
    );

    // Test that EI schedules IME to be enabled after next instruction
    cpu->set_ime(false);
    EXPECT_FALSE(cpu->get_ime());

    // Execute EI
    step();
    EXPECT_FALSE(cpu->get_ime()); // IME should still be false immediately after EI

    // Execute a NOP to trigger IME enabling
    step();
    EXPECT_TRUE(cpu->get_ime()); // IME should now be true
}

TEST_F(CpuInterruptsTest, EnableInterrupts)
{
    // Test enabling and disabling specific interrupts
    auto& interrupt_handler = cpu->get_interrupt_handler();

    // Initially, no interrupts should be enabled
    for (uint8_t i = 0; i < 5; ++i) {
        EXPECT_FALSE(interrupt_handler.is_enabled(static_cast<Interrupt>(1 << i)))
            << "Interrupt " << static_cast<int>(i) << " should be disabled initially";
    }

    // Enable all interrupts
    for (uint8_t i = 0; i < 5; ++i) {
        cpu->enable_interrupt(static_cast<Interrupt>(1 << i));
        EXPECT_TRUE(interrupt_handler.is_enabled(static_cast<Interrupt>(1 << i)))
            << "Interrupt " << static_cast<int>(i) << " should be enabled";
    }
}

TEST_F(CpuInterruptsTest, RequestInterrupts)
{
    // Test requesting specific interrupts
    auto& interrupt_handler = cpu->get_interrupt_handler();

    // Initially, no interrupts should be requested
    for (uint8_t i = 0; i < 5; ++i) {
        EXPECT_FALSE(interrupt_handler.is_requested(static_cast<Interrupt>(1 << i)))
            << "Interrupt " << static_cast<int>(i) << " should not be requested initially";
    }

    // Request all interrupts
    for (uint8_t i = 0; i < 5; ++i) {
        cpu->request_interrupt(static_cast<Interrupt>(1 << i));
        EXPECT_TRUE(interrupt_handler.is_requested(static_cast<Interrupt>(1 << i)))
            << "Interrupt " << static_cast<int>(i) << " should be requested";
    }
}

TEST_F(CpuInterruptsTest, VBlankInterrupt)
{
    // Initial PC and SP values (redundant but explicit for clarity)
    auto initial_pc = cpu->get_pc();
    auto initial_sp = cpu->get_sp();

    // Test that V-Blank interrupt is handled correctly
    cpu->set_ime(true);
    cpu->enable_interrupt(Interrupt::VBlank);  // Enable V-Blank interrupt
    cpu->request_interrupt(Interrupt::VBlank); // Request V-Blank interrupt

    auto& interrupt_handler = cpu->get_interrupt_handler();
    EXPECT_TRUE(interrupt_handler.is_enabled(Interrupt::VBlank))
        << "V-Blank interrupt should be enabled";
    EXPECT_TRUE(interrupt_handler.is_requested(Interrupt::VBlank))
        << "V-Blank interrupt should be requested";

    service_interrupts();

    EXPECT_FALSE(interrupt_handler.is_requested(Interrupt::VBlank))
        << "V-Blank interrupt should be cleared after servicing";
    EXPECT_TRUE(interrupt_handler.is_enabled(Interrupt::VBlank))
        << "V-Blank interrupt should still be enabled after servicing";
    EXPECT_FALSE(cpu->get_ime()) << "IME should be disabled after servicing interrupt";
    EXPECT_EQ(cpu->get_pc(), std::to_underlying(InterruptVector::VBlank))
        << "PC should jump to V-Blank vector";
    EXPECT_EQ(cpu->get_sp(), initial_sp - 2) << "SP should be decremented by 2";
    EXPECT_TRUE(cpu->is_halted() == false) << "CPU should not be in HALT state";

    auto expected_pc = initial_pc;
    if (cpu->get_tick_mode() != TickMode::Instruction) {
        ++expected_pc;
    }
    EXPECT_EQ(cpu->read_word(cpu->get_sp()), expected_pc) << "PC should be pushed to stack";
}

TEST_F(CpuInterruptsTest, NoInterruptWhenIMEOff)
{
    // Initial PC and SP values (redundant but explicit for clarity)
    uint16_t initial_pc = cpu->get_pc();
    uint16_t initial_sp = cpu->get_sp();

    // Test that no interrupt is serviced when IME is off
    cpu->set_ime(false);
    cpu->enable_interrupt(Interrupt::VBlank);  // Enable V-Blank interrupt
    cpu->request_interrupt(Interrupt::VBlank); // Request V-Blank interrupt

    auto& interrupt_handler = cpu->get_interrupt_handler();
    EXPECT_TRUE(interrupt_handler.is_enabled(Interrupt::VBlank));
    EXPECT_TRUE(interrupt_handler.is_requested(Interrupt::VBlank));

    if (cpu->get_tick_mode() == TickMode::Instruction) {
        service_interrupts();
    }
    else {
        // Tick once to see if we enter Interrupt Service stage
        cpu->tick();
        EXPECT_FALSE(cpu->get_execution_state().has_stage(Stage::InterruptService));
    }

    EXPECT_TRUE(interrupt_handler.is_requested(Interrupt::VBlank))
        << "Interrupt should still be requested";
    EXPECT_TRUE(interrupt_handler.is_enabled(Interrupt::VBlank))
        << "Interrupt should still be enabled";
    EXPECT_FALSE(cpu->get_ime()) << "IME should still be false";
    EXPECT_EQ(cpu->get_sp(), initial_sp) << "SP should not change";

    auto expected_pc = initial_pc;
    if (cpu->get_tick_mode() == TickMode::MCycle) {
        ++expected_pc;
    }
    EXPECT_EQ(cpu->get_pc(), expected_pc) << "PC should not change";
}

TEST_F(CpuInterruptsTest, NoInterruptWhenNoneRequested)
{
    // Initial PC and SP values (redundant but explicit for clarity)
    uint16_t initial_pc = cpu->get_pc();
    uint16_t initial_sp = cpu->get_sp();

    // Test that no interrupt is serviced when none are requested
    cpu->set_ime(true);
    cpu->enable_interrupt(Interrupt::VBlank); // Enable V-Blank interrupt

    auto& interrupt_handler = cpu->get_interrupt_handler();
    EXPECT_TRUE(interrupt_handler.is_enabled(Interrupt::VBlank));
    EXPECT_FALSE(interrupt_handler.is_requested(Interrupt::VBlank));

    if (cpu->get_tick_mode() == TickMode::Instruction) {
        service_interrupts();
    }
    else {
        // Tick once to see if we enter Interrupt Service stage
        cpu->tick();
        EXPECT_FALSE(cpu->get_execution_state().has_stage(Stage::InterruptService));
    }

    EXPECT_FALSE(interrupt_handler.is_requested(Interrupt::VBlank))
        << "No interrupt should be requested";
    EXPECT_TRUE(interrupt_handler.is_enabled(Interrupt::VBlank))
        << "Interrupt should still be enabled";
    EXPECT_TRUE(cpu->get_ime()) << "IME should still be true";
    EXPECT_EQ(cpu->get_sp(), initial_sp) << "SP should not change";

    auto expected_pc = initial_pc;
    if (cpu->get_tick_mode() == TickMode::MCycle) {
        ++expected_pc;
    }
    EXPECT_EQ(cpu->get_pc(), expected_pc) << "PC should not change";
}

TEST_F(CpuInterruptsTest, MultipleInterrupts)
{
    // Test that only the highest priority interrupt is serviced
    auto initial_pc = cpu->get_pc();
    auto initial_sp = cpu->get_sp();
    cpu->set_ime(true);
    cpu->enable_interrupt(Interrupt::VBlank);  // Enable V-Blank interrupt
    cpu->enable_interrupt(Interrupt::LCDStat); // Enable LCD STAT interrupt

    // Request both interrupts
    cpu->request_interrupt(Interrupt::VBlank);  // V-Blank
    cpu->request_interrupt(Interrupt::LCDStat); // LCD STAT

    auto& interrupt_handler = cpu->get_interrupt_handler();
    EXPECT_TRUE(interrupt_handler.is_enabled(Interrupt::VBlank)) << "V-Blank should be enabled";
    EXPECT_TRUE(interrupt_handler.is_enabled(Interrupt::LCDStat)) << "LCD STAT should be enabled";
    EXPECT_TRUE(interrupt_handler.is_requested(Interrupt::VBlank)) << "V-Blank should be requested";
    EXPECT_TRUE(interrupt_handler.is_requested(Interrupt::LCDStat))
        << "LCD STAT should be requested";

    service_interrupts();

    EXPECT_FALSE(interrupt_handler.is_requested(Interrupt::VBlank))
        << "V-Blank should now be cleared";
    EXPECT_TRUE(interrupt_handler.is_requested(Interrupt::LCDStat))
        << "LCD STAT should still be requested";
    EXPECT_TRUE(interrupt_handler.is_enabled(Interrupt::VBlank))
        << "V-Blank should still be enabled";
    EXPECT_TRUE(interrupt_handler.is_enabled(Interrupt::LCDStat))
        << "LCD STAT should still be enabled";
    EXPECT_FALSE(cpu->get_ime()) << "IME should be disabled after servicing";
    EXPECT_EQ(cpu->get_pc(), std::to_underlying(InterruptVector::VBlank))
        << "PC should jump to V-Blank vector";
    EXPECT_EQ(cpu->get_sp(), initial_sp - 2) << "SP should be decremented by 2";

    auto expected_pc = initial_pc;
    if (cpu->get_tick_mode() != TickMode::Instruction) {
        ++expected_pc;
    }
    EXPECT_EQ(cpu->read_word(cpu->get_sp()), expected_pc) << "PC should be pushed to stack";

    // Service again to handle LCD STAT
    cpu->set_pc(initial_pc); // reset PC as if jumping out of interrupt vector
    cpu->set_ime(true);      // Re-enable IME for next interrupt
    service_interrupts();

    EXPECT_FALSE(interrupt_handler.is_requested(Interrupt::LCDStat))
        << "LCD STAT should now be cleared";
    EXPECT_TRUE(interrupt_handler.is_enabled(Interrupt::VBlank))
        << "V-Blank should still be enabled";
    EXPECT_TRUE(interrupt_handler.is_enabled(Interrupt::LCDStat))
        << "LCD STAT should still be enabled";
    EXPECT_FALSE(cpu->get_ime()) << "IME should be disabled after servicing";
    EXPECT_EQ(cpu->get_pc(), std::to_underlying(InterruptVector::LCDStat))
        << "PC should jump to LCD STAT vector";
    EXPECT_EQ(cpu->get_sp(), initial_sp - 4) << "SP should be decremented by another 2";

    expected_pc = initial_pc;
    if (cpu->get_tick_mode() != TickMode::Instruction) {
        ++expected_pc;
    }
    EXPECT_EQ(cpu->read_word(cpu->get_sp()), expected_pc)
        << "Previous PC should be pushed to stack";
}

TEST_F(CpuInterruptsTest, HaltThenInterrupt)
{
    // Test that CPU exits HALT state when an interrupt is requested
    auto initial_pc = cpu->get_pc();
    auto initial_sp = cpu->get_sp();
    cpu->set_ime(true);
    cpu->enable_interrupt(Interrupt::VBlank); // Enable V-Blank interrupt

    // Enter HALT state (PC += 1 for HALT instruction)
    set_next_instruction(boyboy::core::cpu::Opcode::HALT);
    step();
    EXPECT_TRUE(cpu->is_halted());

    // Request V-Blank interrupt
    cpu->request_interrupt(Interrupt::VBlank);

    auto& interrupt_handler = cpu->get_interrupt_handler();
    EXPECT_TRUE(interrupt_handler.is_enabled(Interrupt::VBlank))
        << "V-Blank interrupt should be enabled";
    EXPECT_TRUE(interrupt_handler.is_requested(Interrupt::VBlank))
        << "V-Blank interrupt should be requested";

    // Service interrupts
    service_interrupts();

    EXPECT_FALSE(interrupt_handler.is_requested(Interrupt::VBlank))
        << "Interrupt should be cleared after servicing";
    EXPECT_TRUE(interrupt_handler.is_enabled(Interrupt::VBlank))
        << "Interrupt should still be enabled after servicing";
    EXPECT_FALSE(cpu->get_ime()) << "IME should be disabled after servicing interrupt";
    EXPECT_EQ(cpu->get_pc(), std::to_underlying(InterruptVector::VBlank))
        << "PC should jump to V-Blank vector";
    EXPECT_EQ(cpu->get_sp(), initial_sp - 2) << "SP should be decremented by 2";
    // In "cycle" mode SP is PC+2 because of fetch/execute overlap
    auto expected_sp = initial_pc +
                       ((cpu->get_tick_mode() == boyboy::core::cpu::TickMode::Instruction) ? 1 : 2);
    EXPECT_EQ(cpu->read_word(cpu->get_sp()), expected_sp)
        << "Original PC+1 should be pushed to stack";
    EXPECT_FALSE(cpu->is_halted()) << "CPU should exit HALT state";
}
