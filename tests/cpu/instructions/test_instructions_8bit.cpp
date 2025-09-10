/**
 * @file test_instructions_8bit.cpp
 * @brief Workbench file for developing and debugging 8-bit CPU tests.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include <optional>

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
using ADCR8Test = R8Test<R8ALUParam>;
using SBCR8Test = R8Test<R8ALUParam>;

// -----------------------------
// Test definitions
// -----------------------------
TEST_P(ANDR8Test, Works) { run_test(); }
TEST_P(ORR8Test, Works) { run_test(); }
TEST_P(XORR8Test, Works) { run_test(); }
TEST_P(CPR8Test, Works) { run_test(); }
TEST_P(ADCR8Test, Works) { run_test(); }
TEST_P(SBCR8Test, Works) { run_test(); }

// -----------------------------
// Parameter instantiations
// -----------------------------
INSTANTIATE_TEST_SUITE_P(AndInstructions,
                         ANDR8Test,
                         ::testing::Values(
                             R8ALUParam{
                                 .opcode         = Opcode::AND_A_A,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = 0xFF, // ignored
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0xAA,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = true,
                                 .expect_c       = false,
                                 .name           = "A_SameBits",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::AND_A_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = 0x00,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .expect_n       = false,
                                 .expect_h       = true,
                                 .expect_c       = false,
                                 .name           = "B_Zero",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::AND_A_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = 0x55,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .expect_n       = false,
                                 .expect_h       = true,
                                 .expect_c       = false,
                                 .name           = "C_NoCommonBits",
                             },

                             R8ALUParam{
                                 .opcode         = Opcode::AND_A_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = 0xF0,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0xA0,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = true,
                                 .expect_c       = false,
                                 .name           = "D_UpperNibble",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::AND_A_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = 0x0F,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x0A,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = true,
                                 .expect_c       = false,
                                 .name           = "E_LowerNibble",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::AND_A_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = 0xAA,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0xAA,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = true,
                                 .expect_c       = false,
                                 .name           = "H_SameBits",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::AND_A_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = 0xFF,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0xAA,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = true,
                                 .expect_c       = false,
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
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x55,
                                 .src_value      = 0xFF, // ignored
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x55,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_A",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::OR_A_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x55,
                                 .src_value      = 0x0F,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x5F,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_B",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::OR_A_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x00,
                                 .src_value      = 0x00,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_C_Zero",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::OR_A_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xF0,
                                 .src_value      = 0x07,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0xF7,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_D",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::OR_A_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x0F,
                                 .src_value      = 0xF0,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0xFF,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_E",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::OR_A_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = 0x55,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0xFF,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_H",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::OR_A_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x00,
                                 .src_value      = 0xFF,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0xFF,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_L",
                             }),
                         boyboy::test::cpu::param_name<R8ALUParam>);

INSTANTIATE_TEST_SUITE_P(XorInstructions,
                         XORR8Test,
                         ::testing::Values(
                             R8ALUParam{
                                 .opcode         = Opcode::XOR_A_A,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xFF,
                                 .src_value      = 0xFF, // ignored
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_A",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::XOR_A_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xFF,
                                 .src_value      = 0x0F,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0xF0,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_B",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::XOR_A_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x0F,
                                 .src_value      = 0x0F,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_C_Zero",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::XOR_A_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xF0,
                                 .src_value      = 0xFF,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x0F,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_D",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::XOR_A_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xFF,
                                 .src_value      = 0xFF,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_E_Zero",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::XOR_A_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = 0x55,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0xFF,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_H",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::XOR_A_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xFF,
                                 .src_value      = 0x00,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0xFF,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_L",
                             }),
                         boyboy::test::cpu::param_name<R8ALUParam>);

INSTANTIATE_TEST_SUITE_P(CpInstructions,
                         CPR8Test,
                         ::testing::Values(
                             R8ALUParam{
                                 .opcode         = Opcode::CP_A_A,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xFF,
                                 .src_value      = 0xFF, // ignored
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0xFF, // A is unchanged
                                 .expect_z       = true,
                                 .expect_n       = true,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_A",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::CP_A_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x0F,
                                 .src_value      = 0x0F,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x0F, // A is unchanged
                                 .expect_z       = true,
                                 .expect_n       = true,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_B_Zero",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::CP_A_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x10,
                                 .src_value      = 0x01,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x10, // A is unchanged
                                 .expect_z       = false,
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .expect_c       = false,
                                 .name           = "A_C_HalfCarry",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::CP_A_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x00,
                                 .src_value      = 0x01,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x00, // A is unchanged
                                 .expect_z       = false,
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
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x80, // A is unchanged
                                 .expect_z       = false,
                                 .expect_n       = true,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_E_Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::CP_A_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x7F,
                                 .src_value      = 0x80,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x7F, // A is unchanged
                                 .expect_z       = false,
                                 .expect_n       = true,
                                 .expect_h       = false,
                                 .expect_c       = true,
                                 .name           = "A_H_Underflow",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::CP_A_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x01,
                                 .src_value      = 0x01,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x01, // A is unchanged
                                 .expect_z       = true,
                                 .expect_n       = true,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_L_Zero",
                             }),
                         boyboy::test::cpu::param_name<R8ALUParam>);

INSTANTIATE_TEST_SUITE_P(AdcInstructions,
                         ADCR8Test,
                         ::testing::Values(
                             // ---------------------------
                             // ADC A, A
                             // ---------------------------
                             R8ALUParam{
                                 .opcode         = Opcode::ADC_A_A,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x55,
                                 .src_value      = 0x55, // ignored
                                 .carry_in       = false,
                                 .expected_value = 0xAA,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_A_NoCarry",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::ADC_A_A,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x55,
                                 .src_value      = 0x55,
                                 .carry_in       = true,
                                 .expected_value = 0xAB, // 0x55 + 0x55 + 1
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_A_WithCarry",
                             },

                             // ---------------------------
                             // ADC A, B
                             // ---------------------------
                             R8ALUParam{
                                 .opcode         = Opcode::ADC_A_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x0F,
                                 .src_value      = 0x01,
                                 .carry_in       = false,
                                 .expected_value = 0x10,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = true,
                                 .expect_c       = false,
                                 .name           = "A_B_NoCarry",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::ADC_A_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x0F,
                                 .src_value      = 0x01,
                                 .carry_in       = true,
                                 .expected_value = 0x11,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = true,
                                 .expect_c       = false,
                                 .name           = "A_B_WithCarry",
                             },

                             // ---------------------------
                             // ADC A, C
                             // ---------------------------
                             R8ALUParam{
                                 .opcode         = Opcode::ADC_A_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x7F,
                                 .src_value      = 0x01,
                                 .carry_in       = false,
                                 .expected_value = 0x80,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = true,
                                 .expect_c       = false,
                                 .name           = "A_C_OverflowBit7_NoCarry",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::ADC_A_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x7F,
                                 .src_value      = 0x01,
                                 .carry_in       = true,
                                 .expected_value = 0x81, // +1 carry
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = true,
                                 .expect_c       = false,
                                 .name           = "A_C_OverflowBit7_WithCarry",
                             },

                             // ---------------------------
                             // ADC A, D
                             // ---------------------------
                             R8ALUParam{
                                 .opcode         = Opcode::ADC_A_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x00,
                                 .src_value      = 0x00,
                                 .carry_in       = false,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_D_Zero_NoCarry",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::ADC_A_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x00,
                                 .src_value      = 0x00,
                                 .carry_in       = true,
                                 .expected_value = 0x01,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_D_Zero_WithCarry",
                             },

                             // ---------------------------
                             // ADC A, E
                             // ---------------------------
                             R8ALUParam{
                                 .opcode         = Opcode::ADC_A_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xFF,
                                 .src_value      = 0x01,
                                 .carry_in       = false,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .expect_n       = false,
                                 .expect_h       = true,
                                 .expect_c       = true,
                                 .name           = "A_E_CarryOut_NoCarry",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::ADC_A_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xFF,
                                 .src_value      = 0x01,
                                 .carry_in       = true,
                                 .expected_value = 0x01,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = true,
                                 .expect_c       = true,
                                 .name           = "A_E_CarryOut_WithCarry",
                             },

                             // ---------------------------
                             // ADC A, H
                             // ---------------------------
                             R8ALUParam{
                                 .opcode         = Opcode::ADC_A_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x0F,
                                 .src_value      = 0x01,
                                 .carry_in       = false,
                                 .expected_value = 0x10,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = true,
                                 .expect_c       = false,
                                 .name           = "A_H_HalfCarry_NoCarry",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::ADC_A_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x0F,
                                 .src_value      = 0x01,
                                 .carry_in       = true,
                                 .expected_value = 0x11,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = true,
                                 .expect_c       = false,
                                 .name           = "A_H_HalfCarry_WithCarry",
                             },

                             // ---------------------------
                             // ADC A, L
                             // ---------------------------
                             R8ALUParam{
                                 .opcode         = Opcode::ADC_A_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x12,
                                 .src_value      = 0x34,
                                 .carry_in       = false,
                                 .expected_value = 0x46,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_L_Normal_NoCarry",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::ADC_A_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x12,
                                 .src_value      = 0x34,
                                 .carry_in       = true,
                                 .expected_value = 0x47,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_L_Normal_WithCarry",
                             }),
                         boyboy::test::cpu::param_name<R8ALUParam>);

INSTANTIATE_TEST_SUITE_P(SbcInstructions,
                         SBCR8Test,
                         ::testing::Values(
                             // ---------------------------
                             // SBC A, A
                             // ---------------------------
                             R8ALUParam{
                                 .opcode         = Opcode::SBC_A_A,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x55,
                                 .src_value      = 0x55,
                                 .carry_in       = false,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .expect_n       = true,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_A_NoCarry",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::SBC_A_A,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x55,
                                 .src_value      = 0x55,
                                 .carry_in       = true,
                                 .expected_value = 0xFF, // 0x55 - 0x55 - 1 = -1
                                 .expect_z       = false,
                                 .expect_n       = true,
                                 .expect_h       = true, // half-borrow from bit 4
                                 .expect_c       = true,
                                 .name           = "A_A_WithCarry",
                             },

                             // ---------------------------
                             // SBC A, B
                             // ---------------------------
                             R8ALUParam{
                                 .opcode         = Opcode::SBC_A_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x10,
                                 .src_value      = 0x01,
                                 .carry_in       = false,
                                 .expected_value = 0x0F,
                                 .expect_z       = false,
                                 .expect_n       = true,
                                 .expect_h       = true, // 0x10 < 0x01 borrow from bit 4?
                                 .expect_c       = false,
                                 .name           = "A_B_NoCarry",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::SBC_A_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x10,
                                 .src_value      = 0x01,
                                 .carry_in       = true,
                                 .expected_value = 0x0E, // 0x10 - 0x01 - 1
                                 .expect_z       = false,
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .expect_c       = false,
                                 .name           = "A_B_WithCarry",
                             },

                             // ---------------------------
                             // SBC A, C
                             // ---------------------------
                             R8ALUParam{
                                 .opcode         = Opcode::SBC_A_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x00,
                                 .src_value      = 0x01,
                                 .carry_in       = false,
                                 .expected_value = 0xFF, // underflow
                                 .expect_z       = false,
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .expect_c       = true,
                                 .name           = "A_C_Underflow_NoCarry",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::SBC_A_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x00,
                                 .src_value      = 0x01,
                                 .carry_in       = true,
                                 .expected_value = 0xFE, // -1 - carry
                                 .expect_z       = false,
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .expect_c       = true,
                                 .name           = "A_C_Underflow_WithCarry",
                             },

                             // ---------------------------
                             // SBC A, D
                             // ---------------------------
                             R8ALUParam{
                                 .opcode         = Opcode::SBC_A_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x01,
                                 .src_value      = 0x01,
                                 .carry_in       = false,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .expect_n       = true,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_D_Zero_NoCarry",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::SBC_A_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x01,
                                 .src_value      = 0x01,
                                 .carry_in       = true,
                                 .expected_value = 0xFF,
                                 .expect_z       = false,
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .expect_c       = true,
                                 .name           = "A_D_Zero_WithCarry",
                             },

                             // ---------------------------
                             // SBC A, E
                             // ---------------------------
                             R8ALUParam{
                                 .opcode         = Opcode::SBC_A_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xFF,
                                 .src_value      = 0xFF,
                                 .carry_in       = false,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .expect_n       = true,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_E_Zero_NoCarry",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::SBC_A_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xFF,
                                 .src_value      = 0xFF,
                                 .carry_in       = true,
                                 .expected_value = 0xFF, // subtract 1 more
                                 .expect_z       = false,
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .expect_c       = true,
                                 .name           = "A_E_Zero_WithCarry",
                             },

                             // ---------------------------
                             // SBC A, H
                             // ---------------------------
                             R8ALUParam{
                                 .opcode         = Opcode::SBC_A_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x10,
                                 .src_value      = 0x01,
                                 .carry_in       = false,
                                 .expected_value = 0x0F,
                                 .expect_z       = false,
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .expect_c       = false,
                                 .name           = "A_H_HalfBorrow_NoCarry",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::SBC_A_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x10,
                                 .src_value      = 0x01,
                                 .carry_in       = true,
                                 .expected_value = 0x0E,
                                 .expect_z       = false,
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .expect_c       = false,
                                 .name           = "A_H_HalfBorrow_WithCarry",
                             },

                             // ---------------------------
                             // SBC A, L
                             // ---------------------------
                             R8ALUParam{
                                 .opcode         = Opcode::SBC_A_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x20,
                                 .src_value      = 0x10,
                                 .carry_in       = false,
                                 .expected_value = 0x10,
                                 .expect_z       = false,
                                 .expect_n       = true,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_L_Normal_NoCarry",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::SBC_A_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x20,
                                 .src_value      = 0x10,
                                 .carry_in       = true,
                                 .expected_value = 0x0F,
                                 .expect_z       = false,
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .expect_c       = false,
                                 .name           = "A_L_Normal_WithCarry",
                             }),
                         boyboy::test::cpu::param_name<R8ALUParam>);
