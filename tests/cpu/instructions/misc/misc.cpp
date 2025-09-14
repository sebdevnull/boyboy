/**
 * @file misc.cpp
 * @brief Unit tests for miscellaneous CPU instructions.
 *
 * NOP
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>
#include <cstdint>

// boyboy
#include "boyboy/cpu/cpu.h"
#include "boyboy/cpu/opcodes.h"

// Helpers
#include "helpers/cpu_fixtures.h"

using boyboy::test::cpu::CpuTest;

using boyboy::cpu::Opcode;

// -----------------------------
// Test types
// -----------------------------
class NopTest : public CpuTest {};
class EiTest : public CpuTest {};
class DiTest : public CpuTest {};
class HaltTest : public CpuTest {};
class StopTest : public CpuTest {};

// -----------------------------
// Test definitions
// -----------------------------
// NOP
TEST_F(NopTest, NOPDefault)
{
    uint16_t initial_pc = cpu.get_pc();

    // Load NOP instruction at current PC
    cpu.write_byte(initial_pc, static_cast<uint8_t>(Opcode::NOP));

    // Fetch
    uint8_t opcode = cpu.fetch();
    EXPECT_EQ(opcode, static_cast<uint8_t>(Opcode::NOP));

    // Execute
    cpu.execute(opcode);
    EXPECT_EQ(cpu.get_pc(), initial_pc + 1);

    EXPECT_FALSE(cpu.get_flags());
}

// EI
TEST_F(EiTest, Works)
{
    cpu.set_ime(false);
    run(Opcode::EI);
    EXPECT_FALSE(cpu.get_ime()) << "IME flag should be delayed after EI";

    // Next instruction should set IME
    cpu.write_byte(cpu.get_pc(), static_cast<uint8_t>(Opcode::NOP));
    cpu.step();
    EXPECT_TRUE(cpu.get_ime()) << "IME flag not set after EI";
}

// DI
TEST_F(DiTest, Works)
{
    cpu.set_ime(true);
    run(Opcode::DI);
    EXPECT_FALSE(cpu.get_ime()) << "IME flag not cleared after DI";
}

// HALT
TEST_F(HaltTest, Works)
{
    EXPECT_FALSE(cpu.is_halted()) << "CPU should not be halted at start";
    run(Opcode::HALT);
    EXPECT_TRUE(cpu.is_halted()) << "CPU not halted after HALT instruction";
}

// STOP
TEST_F(StopTest, Works)
{
    // We don't implement STOP yet, so we just check that we hit the instruction
    EXPECT_THROW(run(Opcode::STOP_N8), std::runtime_error);
}