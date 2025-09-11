/**
 * @file ld_r_hl.cpp
 * @brief Unit tests for LD r8, [HL] CPU instruction.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

// Helpers
#include "boyboy/cpu/registers.h"
#include "helpers/cpu_fixtures.h"
#include "helpers/cpu_params.h"

using boyboy::cpu::Opcode;
using boyboy::cpu::Reg16Name;
using boyboy::cpu::Reg8Name;

using boyboy::test::cpu::OperandType;
using boyboy::test::cpu::R8Param;
using boyboy::test::cpu::R8Test;

// -----------------------------
// Test types
// -----------------------------
using LdR8HLTest = R8Test<R8Param>;

// -----------------------------
// Test definitions
// -----------------------------
TEST_P(LdR8HLTest, Works) { run_test(); }

// -----------------------------
// Parameter instantiations
// -----------------------------
// LD r8, [HL]
INSTANTIATE_TEST_SUITE_P(LdInstructions,
                         LdR8HLTest,
                         ::testing::Values(
                             R8Param{
                                 .opcode         = Opcode::LD_A_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = 0x0000,
                                 .src_value      = 0xAA,
                                 .expected_value = 0xAA,
                                 .name           = "LD_A_AT_HL",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_B_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg8Name::B,
                                 .src_addr       = 0x1234,
                                 .src_value      = 0xBB,
                                 .expected_value = 0xBB,
                                 .name           = "LD_B_AT_HL",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_C_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg8Name::C,
                                 .src_addr       = 0xFFFF,
                                 .src_value      = 0xCC,
                                 .expected_value = 0xCC,
                                 .name           = "LD_C_AT_HL",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_D_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg8Name::D,
                                 .src_addr       = 0x8000,
                                 .src_value      = 0xDD,
                                 .expected_value = 0xDD,
                                 .name           = "LD_D_AT_HL",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_E_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg8Name::E,
                                 .src_addr       = 0x7FFF,
                                 .src_value      = 0xEE,
                                 .expected_value = 0xEE,
                                 .name           = "LD_E_AT_HL",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_H_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg8Name::H,
                                 .src_addr       = 0x4000,
                                 .src_value      = 0xFF,
                                 .expected_value = 0xFF,
                                 .name           = "LD_H_AT_HL",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_L_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg8Name::L,
                                 .src_addr       = 0x2000,
                                 .src_value      = 0x11,
                                 .expected_value = 0x11,
                                 .name           = "LD_L_AT_HL",
                             }),
                         boyboy::test::cpu::param_name<R8Param>);