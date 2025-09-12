/**
 * @file ld_hl_r.cpp
 * @brief Unit tests for LD [HL], r8 CPU instruction.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

// Helpers
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
using LdHLR8Test = R8Test<R8Param>;

// -----------------------------
// Test definitions
// -----------------------------
TEST_P(LdHLR8Test, Works) { run_test(); }

// -----------------------------
// Parameter instantiations
// -----------------------------
// LD [HL], r8
INSTANTIATE_TEST_SUITE_P(LdInstructions,
                         LdHLR8Test,
                         ::testing::Values(
                             R8Param{
                                 .opcode         = Opcode::LD_AT_HL_A,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg16Name::HL,
                                 .dst_addr       = 0x0000,
                                 .src_value      = 0xAA,
                                 .expected_value = 0xAA,
                                 .name           = "LD_AT_HL_A",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_AT_HL_B,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg16Name::HL,
                                 .dst_addr       = 0x1000,
                                 .src_value      = 0xBB,
                                 .expected_value = 0xBB,
                                 .name           = "LD_AT_HL_B",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_AT_HL_C,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg16Name::HL,
                                 .dst_addr       = 0x2000,
                                 .src_value      = 0xCC,
                                 .expected_value = 0xCC,
                                 .name           = "LD_AT_HL_C",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_AT_HL_D,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg16Name::HL,
                                 .dst_addr       = 0x3000,
                                 .src_value      = 0xDD,
                                 .expected_value = 0xDD,
                                 .name           = "LD_AT_HL_D",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_AT_HL_E,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg16Name::HL,
                                 .dst_addr       = 0x4000,
                                 .src_value      = 0xEE,
                                 .expected_value = 0xEE,
                                 .name           = "LD_AT_HL_E",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_AT_HL_H,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg16Name::HL,
                                 .dst_addr       = 0xFF11,
                                 .src_value      = 0xFF,
                                 .expected_value = 0xFF,
                                 .name           = "LD_AT_HL_H",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_AT_HL_L,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg16Name::HL,
                                 .dst_addr       = 0xFF11,
                                 .src_value      = 0x11,
                                 .expected_value = 0x11,
                                 .name           = "LD_AT_HL_L",
                             }),
                         boyboy::test::cpu::param_name<R8Param>);