/**
 * @file test_logic.cpp
 * @brief Unit tests for AND, OR, XOR and CP CPU instructions.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

// Helpers
#include "helpers/cpu_fixtures.h"
#include "helpers/cpu_params.h"

using namespace boyboy::cpu;

using boyboy::test::cpu::R8ALUParam;
using boyboy::test::cpu::R8Test;

// -----------------------------
// Test types
// -----------------------------
using ANDR8Test = R8Test<R8ALUParam>;
using ORR8Test  = R8Test<R8ALUParam>;
using XORR8Test = R8Test<R8ALUParam>;
using CPR8Test  = R8Test<R8ALUParam>;

// -----------------------------
// Test definitions
// -----------------------------
TEST_P(ANDR8Test, Works) { run_test(); }
TEST_P(ORR8Test, Works) { run_test(); }
TEST_P(XORR8Test, Works) { run_test(); }
TEST_P(CPR8Test, Works) { run_test(); }

// -----------------------------
// Parameter instantiations
// -----------------------------
INSTANTIATE_TEST_SUITE_P(AndInstructions,
                         ANDR8Test,
                         ::testing::Values(
                             R8ALUParam{
                                 .opcode         = Opcode::AND_A_A,
                                 .src            = Reg8Name::A,
                                 .src_value      = 0xAA,
                                 .expected_value = 0xAA,
                                 .expect_h       = true,
                                 .name           = "A_SameBits",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::AND_A_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = 0x00,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .expect_h       = true,
                                 .name           = "B_Zero",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::AND_A_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = 0x55,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .expect_h       = true,
                                 .name           = "C_NoCommonBits",
                             },

                             R8ALUParam{
                                 .opcode         = Opcode::AND_A_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = 0xF0,
                                 .expected_value = 0xA0,
                                 .expect_h       = true,
                                 .name           = "D_UpperNibble",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::AND_A_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = 0x0F,
                                 .expected_value = 0x0A,
                                 .expect_h       = true,
                                 .name           = "E_LowerNibble",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::AND_A_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = 0xAA,
                                 .expected_value = 0xAA,
                                 .expect_h       = true,
                                 .name           = "H_SameBits",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::AND_A_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = 0xFF,
                                 .expected_value = 0xAA,
                                 .expect_h       = true,
                                 .name           = "L_SameBits",
                             }

                             ),
                         boyboy::test::cpu::param_name<R8ALUParam>);

INSTANTIATE_TEST_SUITE_P(OrInstructions,
                         ORR8Test,
                         ::testing::Values(
                             R8ALUParam{
                                 .opcode         = Opcode::OR_A_A,
                                 .src            = Reg8Name::A,
                                 .src_value      = 0x55,
                                 .expected_value = 0x55,
                                 .name           = "A_A_Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::OR_A_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x55,
                                 .src_value      = 0x0F,
                                 .expected_value = 0x5F,
                                 .name           = "A_B_Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::OR_A_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x00,
                                 .src_value      = 0x00,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .name           = "A_C_Zero",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::OR_A_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xF0,
                                 .src_value      = 0x07,
                                 .expected_value = 0xF7,
                                 .name           = "A_D_Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::OR_A_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x0F,
                                 .src_value      = 0xF0,
                                 .expected_value = 0xFF,
                                 .name           = "A_E_Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::OR_A_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = 0x55,
                                 .expected_value = 0xFF,
                                 .name           = "A_H_Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::OR_A_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x00,
                                 .src_value      = 0xFF,
                                 .expected_value = 0xFF,
                                 .name           = "A_L_Normal",
                             }),
                         boyboy::test::cpu::param_name<R8ALUParam>);

INSTANTIATE_TEST_SUITE_P(XorInstructions,
                         XORR8Test,
                         ::testing::Values(
                             R8ALUParam{
                                 .opcode         = Opcode::XOR_A_A,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg8Name::A,
                                 .src_value      = 0xFF,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .name           = "A_A_Zero",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::XOR_A_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xFF,
                                 .src_value      = 0x0F,
                                 .expected_value = 0xF0,
                                 .name           = "A_B_Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::XOR_A_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x0F,
                                 .src_value      = 0x0F,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .name           = "A_C_Zero",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::XOR_A_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xF0,
                                 .src_value      = 0xFF,
                                 .expected_value = 0x0F,
                                 .name           = "A_D_Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::XOR_A_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xFF,
                                 .src_value      = 0xFF,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .name           = "A_E_Zero",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::XOR_A_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = 0x55,
                                 .expected_value = 0xFF,
                                 .name           = "A_H_Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::XOR_A_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xFF,
                                 .src_value      = 0x00,
                                 .expected_value = 0xFF,
                                 .name           = "A_L_Normal",
                             }),
                         boyboy::test::cpu::param_name<R8ALUParam>);

INSTANTIATE_TEST_SUITE_P(CpInstructions,
                         CPR8Test,
                         ::testing::Values(
                             R8ALUParam{
                                 .opcode         = Opcode::CP_A_A,
                                 .src            = Reg8Name::A,
                                 .src_value      = 0xFF,
                                 .expected_value = 0xFF, // A is unchanged
                                 .expect_z       = true,
                                 .expect_n       = true,
                                 .name           = "A_A_Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::CP_A_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x0F,
                                 .src_value      = 0x0F,
                                 .expected_value = 0x0F, // A is unchanged
                                 .expect_z       = true,
                                 .expect_n       = true,
                                 .name           = "A_B_Zero",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::CP_A_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x10,
                                 .src_value      = 0x01,
                                 .expected_value = 0x10, // A is unchanged
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .name           = "A_C_HalfCarry",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::CP_A_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x00,
                                 .src_value      = 0x01,
                                 .expected_value = 0x00, // A is unchanged
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .expect_c       = true,
                                 .name           = "A_D_Underflow",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::CP_A_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x80,
                                 .src_value      = 0x70,
                                 .expected_value = 0x80, // A is unchanged
                                 .expect_n       = true,
                                 .name           = "A_E_Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::CP_A_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x7F,
                                 .src_value      = 0x80,
                                 .expected_value = 0x7F, // A is unchanged
                                 .expect_n       = true,
                                 .expect_c       = true,
                                 .name           = "A_H_Underflow",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::CP_A_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x01,
                                 .src_value      = 0x01,
                                 .expected_value = 0x01, // A is unchanged
                                 .expect_z       = true,
                                 .expect_n       = true,
                                 .name           = "A_L_Zero",
                             }),
                         boyboy::test::cpu::param_name<R8ALUParam>);
