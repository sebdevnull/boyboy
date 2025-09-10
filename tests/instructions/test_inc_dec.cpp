/**
 * @file test_inc_dec.cpp
 * @brief Unit tests for INC and DEC CPU instructions.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include <optional>

#include "helpers/cpu_fixtures.h"
#include "helpers/cpu_params.h"

using namespace boyboy::cpu;

using boyboy::test::cpu::R8ALUParam;
using boyboy::test::cpu::R8Test;

// -----------------------------
// Test types
// -----------------------------
using IncR8Test = R8Test<R8ALUParam>;
using DecR8Test = R8Test<R8ALUParam>;

// -----------------------------
// Test definitions
// -----------------------------
TEST_P(IncR8Test, Works) { run_test(); }
TEST_P(DecR8Test, Works) { run_test(); }

// -----------------------------
// Parameter instantiations
// -----------------------------

// INC r8
INSTANTIATE_TEST_SUITE_P(IncInstructions,
                         IncR8Test,
                         ::testing::Values(
                             // Edge-case tests
                             R8ALUParam{
                                 .opcode         = Opcode::INC_A,
                                 .src            = Reg8Name::A,
                                 .dst            = std::nullopt,
                                 .initial_a      = std::nullopt,
                                 .src_value      = 0x0F,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x10,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = true,
                                 .expect_c       = false,
                                 .name           = "A_HalfCarry",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::INC_B,
                                 .src            = Reg8Name::B,
                                 .dst            = std::nullopt,
                                 .initial_a      = std::nullopt,
                                 .src_value      = 0xFF,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .expect_n       = false,
                                 .expect_h       = true,
                                 .expect_c       = false,
                                 .name           = "B_ZeroFlagSet",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::INC_C,
                                 .src            = Reg8Name::C,
                                 .dst            = std::nullopt,
                                 .initial_a      = std::nullopt,
                                 .src_value      = 0x7E,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x7F,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "C_Normal",
                             },

                             // Normal tests
                             R8ALUParam{
                                 .opcode         = Opcode::INC_D,
                                 .src            = Reg8Name::D,
                                 .dst            = std::nullopt,
                                 .initial_a      = std::nullopt,
                                 .src_value      = 0x01,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x02,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "D_Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::INC_E,
                                 .src            = Reg8Name::E,
                                 .dst            = std::nullopt,
                                 .initial_a      = std::nullopt,
                                 .src_value      = 0x01,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x02,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "E_Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::INC_H,
                                 .src            = Reg8Name::H,
                                 .dst            = std::nullopt,
                                 .initial_a      = std::nullopt,
                                 .src_value      = 0x01,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x02,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "H_Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::INC_L,
                                 .src            = Reg8Name::L,
                                 .dst            = std::nullopt,
                                 .initial_a      = std::nullopt,
                                 .src_value      = 0x01,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x02,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "L_Normal",
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
                                 .dst            = std::nullopt,
                                 .initial_a      = std::nullopt,
                                 .src_value      = 0x10,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x0F,
                                 .expect_z       = false,
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .expect_c       = false,
                                 .name           = "A_HalfCarry",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::DEC_B,
                                 .src            = Reg8Name::B,
                                 .dst            = std::nullopt,
                                 .initial_a      = std::nullopt,
                                 .src_value      = 0x01,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .expect_n       = true,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "B_ZeroFlagSet",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::DEC_C,
                                 .src            = Reg8Name::C,
                                 .dst            = std::nullopt,
                                 .initial_a      = std::nullopt,
                                 .src_value      = 0x00,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0xFF,
                                 .expect_z       = false,
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .expect_c       = false,
                                 .name           = "C_Underflow",
                             },

                             // Normal tests
                             R8ALUParam{
                                 .opcode         = Opcode::DEC_D,
                                 .src            = Reg8Name::D,
                                 .dst            = std::nullopt,
                                 .initial_a      = std::nullopt,
                                 .src_value      = 0x02,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x01,
                                 .expect_z       = false,
                                 .expect_n       = true,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "D_Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::DEC_E,
                                 .src            = Reg8Name::E,
                                 .dst            = std::nullopt,
                                 .initial_a      = std::nullopt,
                                 .src_value      = 0x05,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x04,
                                 .expect_z       = false,
                                 .expect_n       = true,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "E_Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::DEC_H,
                                 .src            = Reg8Name::H,
                                 .dst            = std::nullopt,
                                 .initial_a      = std::nullopt,
                                 .src_value      = 0x01,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .expect_n       = true,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "H_Normal",
                             },
                             R8ALUParam{
                                 .opcode         = Opcode::DEC_L,
                                 .src            = Reg8Name::L,
                                 .dst            = std::nullopt,
                                 .initial_a      = std::nullopt,
                                 .src_value      = 0x0A,
                                 .carry_in       = std::nullopt,
                                 .expected_value = 0x09,
                                 .expect_z       = false,
                                 .expect_n       = true,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "L_Normal",
                             }),
                         boyboy::test::cpu::param_name<R8ALUParam>);