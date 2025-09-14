/**
 * @file ld_r_n.cpp
 * @brief Unit tests for LD r8, n8 CPU instruction.
 *
 * LD r8, n8
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

// Helpers
#include "helpers/cpu_fixtures.h"
#include "helpers/cpu_params.h"

using boyboy::cpu::Opcode;
using boyboy::cpu::Reg8Name;

using boyboy::test::cpu::InstrParam;
using boyboy::test::cpu::InstrTest;
using boyboy::test::cpu::OperandType;

// -----------------------------
// Test types
// -----------------------------
using LdR8N8Test = InstrTest<InstrParam>;

// -----------------------------
// Test definitions
// -----------------------------
TEST_P(LdR8N8Test, Works) { run_test(); }

// -----------------------------
// Parameter instantiations
// -----------------------------
// LD r8, n8
INSTANTIATE_TEST_SUITE_P(LdInstructions,
                         LdR8N8Test,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::LD_A_N8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .src_value      = uint8_t{0xAA},
                                 .expected_value = uint8_t{0xAA},
                                 .name           = "LD_A_N8",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_B_N8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg8Name::B,
                                 .src_value      = uint8_t{0xBB},
                                 .expected_value = uint8_t{0xBB},
                                 .name           = "LD_B_N8",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_C_N8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg8Name::C,
                                 .src_value      = uint8_t{0xCC},
                                 .expected_value = uint8_t{0xCC},
                                 .name           = "LD_C_N8",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_D_N8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg8Name::D,
                                 .src_value      = uint8_t{0xDD},
                                 .expected_value = uint8_t{0xDD},
                                 .name           = "LD_D_N8",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_E_N8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg8Name::E,
                                 .src_value      = uint8_t{0xEE},
                                 .expected_value = uint8_t{0xEE},
                                 .name           = "LD_E_N8",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_H_N8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg8Name::H,
                                 .src_value      = uint8_t{0xFF},
                                 .expected_value = uint8_t{0xFF},
                                 .name           = "LD_H_N8",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_L_N8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg8Name::L,
                                 .src_value      = uint8_t{0x11},
                                 .expected_value = uint8_t{0x11},
                                 .name           = "LD_L_N8",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);
