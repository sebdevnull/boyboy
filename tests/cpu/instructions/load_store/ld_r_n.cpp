/**
 * @file ld_r_n.cpp
 * @brief Unit tests for LD r8, n8 CPU instruction.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include "boyboy/cpu/opcodes.h"
#include "boyboy/cpu/registers.h"

// Helpers
#include "helpers/cpu_fixtures.h"
#include "helpers/cpu_params.h"

using boyboy::cpu::Opcode;
using boyboy::cpu::Reg8Name;

using boyboy::test::cpu::OperandType;
using boyboy::test::cpu::R8Param;
using boyboy::test::cpu::R8Test;

// -----------------------------
// Test types
// -----------------------------
using LdR8N8Test = R8Test<R8Param>;

// -----------------------------
// Test definitions
// -----------------------------
TEST_P(LdR8N8Test, Works) { run_test(); }

// -----------------------------
// Parameter instantiations
// -----------------------------

// LD r8, n8
INSTANTIATE_TEST_SUITE_P(LdAR8Instructions,
                         LdR8N8Test,
                         ::testing::Values(
                             R8Param{
                                 .opcode         = Opcode::LD_A_N8,
                                 .operand_type   = OperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .src_value      = 0xAA,
                                 .expected_value = 0xAA,
                                 .name           = "LD_A_N8",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_B_N8,
                                 .operand_type   = OperandType::Immediate,
                                 .dst            = Reg8Name::B,
                                 .src_value      = 0xBB,
                                 .expected_value = 0xBB,
                                 .name           = "LD_B_N8",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_C_N8,
                                 .operand_type   = OperandType::Immediate,
                                 .dst            = Reg8Name::C,
                                 .src_value      = 0xCC,
                                 .expected_value = 0xCC,
                                 .name           = "LD_C_N8",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_D_N8,
                                 .operand_type   = OperandType::Immediate,
                                 .dst            = Reg8Name::D,
                                 .src_value      = 0xDD,
                                 .expected_value = 0xDD,
                                 .name           = "LD_D_N8",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_E_N8,
                                 .operand_type   = OperandType::Immediate,
                                 .dst            = Reg8Name::E,
                                 .src_value      = 0xEE,
                                 .expected_value = 0xEE,
                                 .name           = "LD_E_N8",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_H_N8,
                                 .operand_type   = OperandType::Immediate,
                                 .dst            = Reg8Name::H,
                                 .src_value      = 0xFF,
                                 .expected_value = 0xFF,
                                 .name           = "LD_H_N8",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_L_N8,
                                 .operand_type   = OperandType::Immediate,
                                 .dst            = Reg8Name::L,
                                 .src_value      = 0x11,
                                 .expected_value = 0x11,
                                 .name           = "LD_L_N8",
                             }),
                         boyboy::test::cpu::param_name<R8Param>);
