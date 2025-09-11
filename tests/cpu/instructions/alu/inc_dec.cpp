/**
 * @file inc_dec.cpp
 * @brief Unit tests for INC and DEC CPU instructions.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include "helpers/cpu_fixtures.h"
#include "helpers/cpu_params.h"

using namespace boyboy::cpu;

using boyboy::test::cpu::ALUOperandType;
using boyboy::test::cpu::R8ALUParam;
using boyboy::test::cpu::R8Test;

// -----------------------------
// Test types
// -----------------------------
using IncR8Test = R8Test<R8ALUParam>;
using IncHLTest = R8Test<R8ALUParam>;
using DecR8Test = R8Test<R8ALUParam>;
using DecHLTest = R8Test<R8ALUParam>;

// -----------------------------
// Test definitions
// -----------------------------
TEST_P(IncR8Test, Works) { run_test(); }
TEST_P(IncHLTest, Works) { run_test(); }
TEST_P(DecR8Test, Works) { run_test(); }
TEST_P(DecHLTest, Works) { run_test(); }

// -----------------------------
// Parameter instantiations
// -----------------------------

// INC r8
INSTANTIATE_TEST_SUITE_P(IncInstructions,
                         IncR8Test,
                         ::testing::Values(
                             R8ALUParam{
                                 .opcode         = Opcode::INC_A,
                                 .src            = Reg8Name::A,
                                 .src_value      = 0x0F,
                                 .expected_value = 0x10,
                                 .expect_h       = true,
                                 .name           = "A_HalfCarry",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::INC_B,
                                 .src            = Reg8Name::B,
                                 .src_value      = 0xFF,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .expect_h       = true,
                                 .name           = "B_OverflowZero",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::INC_C,
                                 .src            = Reg8Name::C,
                                 .src_value      = 0x7E,
                                 .expected_value = 0x7F,
                                 .name           = "C_Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::INC_D,
                                 .src            = Reg8Name::D,
                                 .src_value      = 0x01,
                                 .expected_value = 0x02,
                                 .name           = "D_Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::INC_E,
                                 .src            = Reg8Name::E,
                                 .src_value      = 0x7F,
                                 .expected_value = 0x80,
                                 .expect_h       = true,
                                 .name           = "E_HalfCarry",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::INC_H,
                                 .src            = Reg8Name::H,
                                 .src_value      = 0xFE,
                                 .expected_value = 0xFF,
                                 .name           = "H_Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::INC_L,
                                 .src            = Reg8Name::L,
                                 .src_value      = 0xFE,
                                 .expected_value = 0xFF,
                                 .name           = "L_Normal",
                             }),
                         boyboy::test::cpu::param_name<R8ALUParam>);

// INC [HL]
INSTANTIATE_TEST_SUITE_P(IncInstructions,
                         IncHLTest,
                         ::testing::Values(
                             R8ALUParam{
                                 .opcode         = Opcode::INC_AT_HL,
                                 .operand_type   = ALUOperandType::IndirectHL,
                                 .src_addr       = 0x0000,
                                 .src_value      = 0x00,
                                 .expected_value = 0x01,
                                 .name           = "Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::INC_AT_HL,
                                 .operand_type   = ALUOperandType::IndirectHL,
                                 .src_addr       = 0x1234,
                                 .src_value      = 0x0F,
                                 .expected_value = 0x10,
                                 .expect_h       = true,
                                 .name           = "HalfCarry",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::INC_AT_HL,
                                 .operand_type   = ALUOperandType::IndirectHL,
                                 .src_addr       = 0x5678,
                                 .src_value      = 0xFF,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .expect_h       = true,
                                 .name           = "ZeroOverflow",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::INC_AT_HL,
                                 .operand_type   = ALUOperandType::IndirectHL,
                                 .src_addr       = 0x9ABC,
                                 .src_value      = 0x7F,
                                 .expected_value = 0x80,
                                 .expect_h       = true,
                                 .name           = "SignChange",
                             }),
                         boyboy::test::cpu::param_name<R8ALUParam>);

// DEC r8
INSTANTIATE_TEST_SUITE_P(DecInstructions,
                         DecR8Test,
                         ::testing::Values(
                             // Edge-case tests
                             R8ALUParam{
                                 .opcode         = Opcode::DEC_A,
                                 .src            = Reg8Name::A,
                                 .src_value      = 0x10,
                                 .expected_value = 0x0F,
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .name           = "A_HalfCarry",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::DEC_B,
                                 .src            = Reg8Name::B,
                                 .src_value      = 0x01,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .expect_n       = true,
                                 .name           = "B_ZeroFlagSet",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::DEC_C,
                                 .src            = Reg8Name::C,
                                 .src_value      = 0x00,
                                 .expected_value = 0xFF,
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .name           = "C_Underflow",
                             },

                             // Normal tests
                             R8ALUParam{
                                 .opcode         = Opcode::DEC_D,
                                 .src            = Reg8Name::D,
                                 .src_value      = 0x02,
                                 .expected_value = 0x01,
                                 .expect_n       = true,
                                 .name           = "D_Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::DEC_E,
                                 .src            = Reg8Name::E,
                                 .src_value      = 0x05,
                                 .expected_value = 0x04,
                                 .expect_n       = true,
                                 .name           = "E_Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::DEC_H,
                                 .src            = Reg8Name::H,
                                 .src_value      = 0x80,
                                 .expected_value = 0x7F,
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .name           = "H_HalfCarry",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::DEC_L,
                                 .src            = Reg8Name::L,
                                 .src_value      = 0x0A,
                                 .expected_value = 0x09,
                                 .expect_n       = true,
                                 .name           = "L_Normal",
                             }),
                         boyboy::test::cpu::param_name<R8ALUParam>);

INSTANTIATE_TEST_SUITE_P(DecInstructions,
                         DecHLTest,
                         ::testing::Values(
                             R8ALUParam{
                                 .opcode         = Opcode::DEC_AT_HL,
                                 .operand_type   = ALUOperandType::IndirectHL,
                                 .src_addr       = 0x0000,
                                 .src_value      = 0xAB,
                                 .expected_value = 0xAA,
                                 .expect_n       = true,
                                 .name           = "Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::DEC_AT_HL,
                                 .operand_type   = ALUOperandType::IndirectHL,
                                 .src_addr       = 0x1234,
                                 .src_value      = 0x01,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .expect_n       = true,
                                 .name           = "Zero",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::DEC_AT_HL,
                                 .operand_type   = ALUOperandType::IndirectHL,
                                 .src_addr       = 0x5678,
                                 .src_value      = 0x10,
                                 .expected_value = 0x0F,
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .name           = "HalfCarry",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::DEC_AT_HL,
                                 .operand_type   = ALUOperandType::IndirectHL,
                                 .src_addr       = 0x9ABC,
                                 .src_value      = 0x00,
                                 .expected_value = 0xFF,
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .name           = "Underflow",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::DEC_AT_HL,
                                 .operand_type   = ALUOperandType::IndirectHL,
                                 .src_addr       = 0xDEF0,
                                 .src_value      = 0x80,
                                 .expected_value = 0x7F,
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .name           = "SignChange",
                             }),
                         boyboy::test::cpu::param_name<R8ALUParam>);