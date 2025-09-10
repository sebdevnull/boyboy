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

using boyboy::test::cpu::ALUOperandType;
using boyboy::test::cpu::R8ALUParam;
using boyboy::test::cpu::R8Test;

// -----------------------------
// Test types
// -----------------------------
using ANDR8Test  = R8Test<R8ALUParam>;
using ANDHLTest  = R8Test<R8ALUParam>;
using ANDImmTest = R8Test<R8ALUParam>;
using ORR8Test   = R8Test<R8ALUParam>;
using ORHLTest   = R8Test<R8ALUParam>;
using ORImmTest  = R8Test<R8ALUParam>;
using XORR8Test  = R8Test<R8ALUParam>;
using XORHLTest  = R8Test<R8ALUParam>;
using XORImmTest = R8Test<R8ALUParam>;
using CPR8Test   = R8Test<R8ALUParam>;
using CPHLTest   = R8Test<R8ALUParam>;
using CPImmTest  = R8Test<R8ALUParam>;

// -----------------------------
// Test definitions
// -----------------------------
TEST_P(ANDR8Test, Works) { run_test(); }
TEST_P(ANDHLTest, Works) { run_test(); }
TEST_P(ANDImmTest, Works) { run_test(); }
TEST_P(ORR8Test, Works) { run_test(); }
TEST_P(ORHLTest, Works) { run_test(); }
TEST_P(ORImmTest, Works) { run_test(); }
TEST_P(XORR8Test, Works) { run_test(); }
TEST_P(XORHLTest, Works) { run_test(); }
TEST_P(XORImmTest, Works) { run_test(); }
TEST_P(CPR8Test, Works) { run_test(); }
TEST_P(CPHLTest, Works) { run_test(); }
TEST_P(CPImmTest, Works) { run_test(); }

// -----------------------------
// Parameter instantiations
// -----------------------------

// AND A, r8
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

// AND A, [HL]
INSTANTIATE_TEST_SUITE_P(AndInstructions,
                         ANDHLTest,
                         ::testing::Values(
                             R8ALUParam{
                                 .opcode         = Opcode::AND_A_AT_HL,
                                 .operand_type   = ALUOperandType::IndirectHL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = 0x0000,
                                 .initial_a      = 0xF0,
                                 .src_value      = 0xAA,
                                 .expected_value = 0xA0,
                                 .expect_h       = true,
                                 .name           = "Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::AND_A_AT_HL,
                                 .operand_type   = ALUOperandType::IndirectHL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = 0x1234,
                                 .initial_a      = 0x0F,
                                 .src_value      = 0xF0,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .expect_h       = true,
                                 .name           = "Zero",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::AND_A_AT_HL,
                                 .operand_type   = ALUOperandType::IndirectHL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = 0x5678,
                                 .initial_a      = 0x55,
                                 .src_value      = 0x55,
                                 .expected_value = 0x55,
                                 .expect_h       = true,
                                 .name           = "SameValue",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::AND_A_AT_HL,
                                 .operand_type   = ALUOperandType::IndirectHL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = 0x9ABC,
                                 .initial_a      = 0xAA,
                                 .src_value      = 0xFF,
                                 .expected_value = 0xAA,
                                 .expect_h       = true,
                                 .name           = "AllBits",
                             }),
                         boyboy::test::cpu::param_name<R8ALUParam>);

// AND A, n8
INSTANTIATE_TEST_SUITE_P(AndInstructions,
                         ANDImmTest,
                         ::testing::Values(
                             R8ALUParam{
                                 .opcode         = Opcode::AND_A_N8,
                                 .operand_type   = ALUOperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xF0,
                                 .src_value      = 0xAA,
                                 .expected_value = 0xA0,
                                 .expect_h       = true,
                                 .name           = "Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::AND_A_N8,
                                 .operand_type   = ALUOperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x0F,
                                 .src_value      = 0xF0,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .expect_h       = true,
                                 .name           = "Zero",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::AND_A_N8,
                                 .operand_type   = ALUOperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x55,
                                 .src_value      = 0x55,
                                 .expected_value = 0x55,
                                 .expect_h       = true,
                                 .name           = "SameValue",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::AND_A_N8,
                                 .operand_type   = ALUOperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = 0xFF,
                                 .expected_value = 0xAA,
                                 .expect_h       = true,
                                 .name           = "AllBits",
                             }),
                         boyboy::test::cpu::param_name<R8ALUParam>);

// OR A, r8
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

// OR A, [HL]
INSTANTIATE_TEST_SUITE_P(OrInstructions,
                         ORHLTest,
                         ::testing::Values(
                             R8ALUParam{
                                 .opcode         = Opcode::OR_A_AT_HL,
                                 .operand_type   = ALUOperandType::IndirectHL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = 0x0000,
                                 .initial_a      = 0xF0,
                                 .src_value      = 0x0A,
                                 .expected_value = 0xFA,
                                 .name           = "Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::OR_A_AT_HL,
                                 .operand_type   = ALUOperandType::IndirectHL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = 0x1234,
                                 .initial_a      = 0x00,
                                 .src_value      = 0x00,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .name           = "Zero",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::OR_A_AT_HL,
                                 .operand_type   = ALUOperandType::IndirectHL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = 0x5678,
                                 .initial_a      = 0x55,
                                 .src_value      = 0x55,
                                 .expected_value = 0x55,
                                 .name           = "SameValue",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::OR_A_AT_HL,
                                 .operand_type   = ALUOperandType::IndirectHL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = 0x9ABC,
                                 .initial_a      = 0x5A,
                                 .src_value      = 0xFF,
                                 .expected_value = 0xFF,
                                 .name           = "AllBits",
                             }),
                         boyboy::test::cpu::param_name<R8ALUParam>);

// OR A, n8
INSTANTIATE_TEST_SUITE_P(OrInstructions,
                         ORImmTest,
                         ::testing::Values(
                             R8ALUParam{
                                 .opcode         = Opcode::OR_A_N8,
                                 .operand_type   = ALUOperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xF0,
                                 .src_value      = 0x0A,
                                 .expected_value = 0xFA,
                                 .name           = "Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::OR_A_N8,
                                 .operand_type   = ALUOperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x00,
                                 .src_value      = 0x00,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .name           = "Zero",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::OR_A_N8,
                                 .operand_type   = ALUOperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x55,
                                 .src_value      = 0x55,
                                 .expected_value = 0x55,
                                 .name           = "SameValue",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::OR_A_N8,
                                 .operand_type   = ALUOperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x5A,
                                 .src_value      = 0xFF,
                                 .expected_value = 0xFF,
                                 .name           = "AllBits",
                             }),
                         boyboy::test::cpu::param_name<R8ALUParam>);

// XOR A, r8
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

// XOR A, [HL]
INSTANTIATE_TEST_SUITE_P(XorInstructions,
                         XORHLTest,
                         ::testing::Values(
                             R8ALUParam{
                                 .opcode         = Opcode::XOR_A_AT_HL,
                                 .operand_type   = ALUOperandType::IndirectHL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = 0x0000,
                                 .initial_a      = 0x00,
                                 .src_value      = 0x00,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .name           = "Zero",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::XOR_A_AT_HL,
                                 .operand_type   = ALUOperandType::IndirectHL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = 0x1234,
                                 .initial_a      = 0x55,
                                 .src_value      = 0x55,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .name           = "ZeroSameBits",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::XOR_A_AT_HL,
                                 .operand_type   = ALUOperandType::IndirectHL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = 0x5678,
                                 .initial_a      = 0xAA,
                                 .src_value      = 0x55,
                                 .expected_value = 0xFF,
                                 .name           = "DifferentBits",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::XOR_A_AT_HL,
                                 .operand_type   = ALUOperandType::IndirectHL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = 0x9ABC,
                                 .initial_a      = 0x5A,
                                 .src_value      = 0xFF,
                                 .expected_value = 0xA5,
                                 .name           = "AllBits",
                             }),
                         boyboy::test::cpu::param_name<R8ALUParam>);

// XOR A, n8
INSTANTIATE_TEST_SUITE_P(XorInstructions,
                         XORImmTest,
                         ::testing::Values(
                             R8ALUParam{
                                 .opcode         = Opcode::XOR_A_N8,
                                 .operand_type   = ALUOperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x00,
                                 .src_value      = 0x00,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .name           = "Zero",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::XOR_A_N8,
                                 .operand_type   = ALUOperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x55,
                                 .src_value      = 0x55,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .name           = "ZeroSameBits",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::XOR_A_N8,
                                 .operand_type   = ALUOperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = 0x55,
                                 .expected_value = 0xFF,
                                 .name           = "DifferentBits",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::XOR_A_N8,
                                 .operand_type   = ALUOperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x5A,
                                 .src_value      = 0xFF,
                                 .expected_value = 0xA5,
                                 .name           = "AllBits",
                             }),
                         boyboy::test::cpu::param_name<R8ALUParam>);

// CP A, r8
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

// CP A, [HL]
INSTANTIATE_TEST_SUITE_P(CpInstructions,
                         CPHLTest,
                         ::testing::Values(
                             R8ALUParam{
                                 .opcode         = Opcode::CP_A_AT_HL,
                                 .operand_type   = ALUOperandType::IndirectHL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = 0x0000,
                                 .initial_a      = 0x80,
                                 .src_value      = 0x70,
                                 .expected_value = 0x80, // A is unchanged
                                 .expect_n       = true,
                                 .name           = "Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::CP_A_AT_HL,
                                 .operand_type   = ALUOperandType::IndirectHL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = 0x0000,
                                 .initial_a      = 0x55,
                                 .src_value      = 0x55,
                                 .expected_value = 0x55, // A is unchanged
                                 .expect_z       = true,
                                 .expect_n       = true,
                                 .name           = "Zero",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::CP_A_AT_HL,
                                 .operand_type   = ALUOperandType::IndirectHL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = 0x0000,
                                 .initial_a      = 0x10,
                                 .src_value      = 0x20,
                                 .expected_value = 0x10, // A is unchanged
                                 .expect_n       = true,
                                 .expect_c       = true,
                                 .name           = "Underflow",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::CP_A_AT_HL,
                                 .operand_type   = ALUOperandType::IndirectHL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = 0x0000,
                                 .initial_a      = 0x10,
                                 .src_value      = 0x01,
                                 .expected_value = 0x10, // A is unchanged
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .name           = "HalfCarry",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::CP_A_AT_HL,
                                 .operand_type   = ALUOperandType::IndirectHL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = 0x0000,
                                 .initial_a      = 0x00,
                                 .src_value      = 0x01,
                                 .expected_value = 0x00, // A is unchanged
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .expect_c       = true,
                                 .name           = "UnderflowHalfCarry",
                             }),
                         boyboy::test::cpu::param_name<R8ALUParam>);

// CP A, n8
INSTANTIATE_TEST_SUITE_P(CpInstructions,
                         CPImmTest,
                         ::testing::Values(
                             R8ALUParam{
                                 .opcode         = Opcode::CP_A_N8,
                                 .operand_type   = ALUOperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x80,
                                 .src_value      = 0x70,
                                 .expected_value = 0x80, // A is unchanged
                                 .expect_n       = true,
                                 .name           = "Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::CP_A_N8,
                                 .operand_type   = ALUOperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x55,
                                 .src_value      = 0x55,
                                 .expected_value = 0x55, // A is unchanged
                                 .expect_z       = true,
                                 .expect_n       = true,
                                 .name           = "Zero",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::CP_A_N8,
                                 .operand_type   = ALUOperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x10,
                                 .src_value      = 0x20,
                                 .expected_value = 0x10, // A is unchanged
                                 .expect_n       = true,
                                 .expect_c       = true,
                                 .name           = "Underflow",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::CP_A_N8,
                                 .operand_type   = ALUOperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x10,
                                 .src_value      = 0x01,
                                 .expected_value = 0x10, // A is unchanged
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .name           = "HalfCarry",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::CP_A_N8,
                                 .operand_type   = ALUOperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x00,
                                 .src_value      = 0x01,
                                 .expected_value = 0x00, // A is unchanged
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .expect_c       = true,
                                 .name           = "UnderflowHalfCarry",
                             }),
                         boyboy::test::cpu::param_name<R8ALUParam>);