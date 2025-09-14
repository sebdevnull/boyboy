/**
 * @file misc.cpp
 * @brief Unit tests for miscellaneous CPU instructions.
 *
 * NOP
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

// boyboy
#include "boyboy/cpu/cpu.h"
#include "boyboy/cpu/opcodes.h"

// Helpers
#include "helpers/cpu_fixtures.h"

using boyboy::test::cpu::CpuTest;

using boyboy::cpu::Opcode;

// ======================= NOP =======================
class NopTest : public CpuTest {};

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