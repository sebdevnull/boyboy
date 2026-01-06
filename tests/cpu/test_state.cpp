/**
 * @file test_state.cpp
 * @brief Tests for CPU execution state.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include <utility>

// helpers
#include "helpers/cpu_fixtures.h"

// boyboy
#include "boyboy/core/cpu/cpu.h"
#include "boyboy/core/cpu/cpu_constants.h"
#include "boyboy/core/cpu/cycles.h"
#include "boyboy/core/cpu/instructions_table.h"
#include "boyboy/core/cpu/interrupts.h"
#include "boyboy/core/cpu/opcodes.h"
#include "boyboy/core/cpu/state.h"

using boyboy::test::cpu::CpuTest;

using namespace boyboy::core::cpu;

class CpuExecutionState : public CpuTest {
protected:
    void SetUp() override
    {
        CpuTest::SetUp();
        cpu->set_tick_mode(TickMode::TCycle);
    }

    // Assumes we are in T-cycle tick mode
    void tick(uint16_t tick = 1) const
    {
        while (tick > 0) {
            cpu->tick();
            --tick;
        }
    }
};

TEST_F(CpuExecutionState, InitialState)
{
    EXPECT_FALSE(cpu->get_ime());
    EXPECT_FALSE(cpu->is_halted());
    EXPECT_EQ(cpu->get_cycles(), 0);

    auto state = cpu->get_execution_state();
    EXPECT_EQ(state.stage, Stage::Fetch);
    EXPECT_EQ(state.cycles_left, FetchCycles);
}

TEST_F(CpuExecutionState, UnprefixedFetch)
{
    cpu->enable_fe_overlap(true);

    const auto& nop_instr = InstructionTable::get_instruction(Opcode::NOP);
    const auto& inc_instr = InstructionTable::get_instruction(Opcode::INC_A);

    // Setup PC
    set_next_bytes({std::to_underlying(Opcode::NOP), std::to_underlying(Opcode::INC_A)});

    const auto& state = cpu->get_execution_state();
    EXPECT_EQ(state.stage, Stage::Fetch);
    EXPECT_EQ(state.cycles_left, FetchCycles);

    // Tick once, should not advance state
    tick();
    EXPECT_EQ(state.stage, Stage::Fetch);
    EXPECT_EQ(state.cycles_left, FetchCycles - 1);

    // Tick to complete fetch stage
    tick(FetchCycles - 1);
    EXPECT_EQ(state.stage, Stage::Execute);
    EXPECT_EQ(state.cycles_left, nop_instr.cycles);
    EXPECT_EQ(state.fetched, std::to_underlying(Opcode::NOP));

    // Tick once, since NOP is 4 cycles, it should start fetching next instruction
    tick();
    EXPECT_EQ(state.stage, Stage::Execute | Stage::Fetch);
    EXPECT_EQ(state.cycles_left, nop_instr.cycles - 1);

    // Tick to complete NOP and next instruction fetch
    tick(nop_instr.cycles - 1);
    EXPECT_EQ(state.stage, Stage::Execute);
    EXPECT_EQ(state.cycles_left, inc_instr.cycles);
    EXPECT_EQ(state.fetched, std::to_underlying(Opcode::INC_A));

    // Tick to complete INC instruction
    tick(inc_instr.cycles);

    // Total cycles ticked
    EXPECT_EQ(cpu->get_cycles(), FetchCycles + nop_instr.cycles + inc_instr.cycles);
}

TEST_F(CpuExecutionState, CBPrefixedFetch)
{
    cpu->enable_fe_overlap(true);

    const auto& rlc_instr  = InstructionTable::get_instruction(CBOpcode::RLC_A);
    const auto& swap_instr = InstructionTable::get_instruction(CBOpcode::SWAP_A);

    // Setup PC
    set_next_bytes(
        {CBInstructionPrefix,
         std::to_underlying(CBOpcode::RLC_A),
         CBInstructionPrefix,
         std::to_underlying(CBOpcode::SWAP_A)}
    );

    const auto& state = cpu->get_execution_state();
    EXPECT_EQ(state.stage, Stage::Fetch);
    EXPECT_EQ(state.cycles_left, FetchCycles);

    // Tick enough to fetch CB prefix
    tick(FetchCycles);
    EXPECT_EQ(state.stage, Stage::Fetch | Stage::CBInstruction);
    EXPECT_EQ(state.cycles_left, FetchCycles);
    EXPECT_EQ(state.fetched, CBInstructionPrefix);

    // Tick enough to fetch first instruction
    tick(FetchCycles);
    EXPECT_EQ(state.stage, Stage::Execute);
    EXPECT_EQ(state.cycles_left, rlc_instr.cycles);
    EXPECT_EQ(state.fetched, std::to_underlying(CBOpcode::RLC_A));

    // Tick once, it should still be only in Execute stage since instruction is 8 cycles
    tick();
    EXPECT_EQ(state.stage, Stage::Execute);
    EXPECT_EQ(state.cycles_left, rlc_instr.cycles - 1);

    // Tick until it starts fetching next instruction
    tick(rlc_instr.cycles - FetchCycles - 1);
    EXPECT_EQ(state.stage, Stage::Execute | Stage::Fetch);
    EXPECT_EQ(state.cycles_left, FetchCycles);

    // Tick to complete current instruction and CB prefix fetch
    tick(FetchCycles);
    EXPECT_EQ(state.stage, Stage::Fetch | Stage::CBInstruction);
    EXPECT_EQ(state.cycles_left, FetchCycles);
    EXPECT_EQ(state.fetched, CBInstructionPrefix);

    // Tick to complete CB prefixed instruction fetch
    tick(FetchCycles);
    EXPECT_EQ(state.stage, Stage::Execute);
    EXPECT_EQ(state.cycles_left, swap_instr.cycles);
    EXPECT_EQ(state.fetched, std::to_underlying(CBOpcode::SWAP_A));

    // Tick to complete instruction
    tick(swap_instr.cycles);

    // Total cycles ticked
    EXPECT_EQ(cpu->get_cycles(), (3 * FetchCycles) + rlc_instr.cycles + swap_instr.cycles);
}

TEST_F(CpuExecutionState, InterruptService)
{
    cpu->enable_fe_overlap(true);

    // Set next instructions
    const auto& instr = InstructionTable::get_instruction(Opcode::INC_A);
    set_next_instruction(Opcode::INC_A);

    const auto& state = cpu->get_execution_state();
    EXPECT_EQ(state.stage, Stage::Fetch);
    EXPECT_EQ(state.cycles_left, FetchCycles);

    // Fetch instruction to begin execution
    tick(FetchCycles);
    EXPECT_EQ(state.stage, Stage::Execute);
    EXPECT_EQ(state.cycles_left, instr.cycles);
    EXPECT_EQ(state.fetched, std::to_underlying(Opcode::INC_A));

    // Set artificial interrupt request
    cpu->set_ime(true);
    cpu->get_interrupt_handler().enable(Interrupt::VBlank);
    cpu->get_interrupt_handler().request(Interrupt::VBlank);

    // Tick once, it shouldn't service the interrupt until finishing execute stage
    tick();
    EXPECT_EQ(state.stage, Stage::Execute | Stage::Fetch);
    EXPECT_EQ(state.cycles_left, instr.cycles - 1);
    EXPECT_EQ(state.fetched, std::to_underlying(Opcode::INC_A));

    // Complete execution, it should start servicing the interrupt and reset the fetch stage
    tick(instr.cycles - 1);
    EXPECT_EQ(state.stage, Stage::Fetch | Stage::InterruptService);
    EXPECT_EQ(state.cycles_left, FetchCycles);
    EXPECT_EQ(state.fetched, std::to_underlying(Opcode::INC_A)); // no prefetch

    // Tick once, it should disable IME and clear IF
    tick();
    EXPECT_EQ(state.stage, Stage::Fetch | Stage::InterruptService);
    EXPECT_EQ(state.cycles_left, FetchCycles);
    EXPECT_EQ(state.fetched, std::to_underlying(Opcode::INC_A)); // no prefetch
    EXPECT_FALSE(cpu->get_ime());
    EXPECT_EQ(cpu->get_interrupt_handler().pending(), 0);

    // Complete interrupt service, it should be back to fetch stage and PC should be at the
    // interrupt vector
    tick(InterruptServiceCycles - 1);
    EXPECT_EQ(state.stage, Stage::Fetch);
    EXPECT_EQ(state.cycles_left, FetchCycles);
    EXPECT_EQ(cpu->get_pc(), std::to_underlying(InterruptVector::VBlank));

    // Total cycles ticked
    EXPECT_EQ(cpu->get_cycles(), FetchCycles + InterruptServiceCycles + instr.cycles);
}