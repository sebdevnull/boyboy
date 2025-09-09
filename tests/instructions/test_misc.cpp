/**
 * @file test_misc.cpp
 * @brief Unit tests for miscellaneous CPU instructions.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include "boyboy/cpu/cpu.h"
#include "boyboy/cpu/opcodes.h"

using namespace boyboy::cpu;

// ======================= NOP =======================
TEST(NopTest, NOPDefault)
{
    Cpu cpu;
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