/**
 * @file test_add_sub.cpp
 * @brief Unit tests for ADD and SUB CPU instructions.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include "boyboy/cpu/cpu.h"
#include "boyboy/cpu/opcodes.h"
#include "boyboy/cpu/registers.h"

// Helpers
#include "helpers/cpu_fixtures.h"
#include "helpers/cpu_params.h"

using boyboy::cpu::Opcode;
using boyboy::cpu::Reg8Name;

using boyboy::test::cpu::R8Param;
using boyboy::test::cpu::R8Test;

// -----------------------------
// Test types
// -----------------------------
using AddR8Test = R8Test<R8Param>;
using SubR8Test = R8Test<R8Param>;

// -----------------------------
// Test definitions
// -----------------------------
TEST_P(AddR8Test, Works)
{
    cpu.set_register(Reg8Name::A, 0x11); // Fixed initial value for A
    run_test();
}

TEST_P(SubR8Test, Works)
{
    cpu.set_register(Reg8Name::A, 0x11); // accumulator initial value
    run_test();
}

// -----------------------------
// Parameter instantiations
// -----------------------------
INSTANTIATE_TEST_SUITE_P(IncInstructions,
                         AddR8Test,
                         ::testing::Values(
                             // Edge-case tests
                             R8Param{
                                 .opcode         = Opcode::ADD_A_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::A,
                                 .src_value      = 0xFE,
                                 .expected_value = 0x0F,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = true,
                                 .name           = "B_CarryOverflow",
                             },
                             R8Param{
                                 .opcode         = Opcode::ADD_A_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::A,
                                 .src_value      = 0xEF,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .expect_n       = false,
                                 .expect_h       = true,
                                 .expect_c       = true,
                                 .name           = "C_ZeroFlagSet",
                             },
                             R8Param{
                                 .opcode         = Opcode::ADD_A_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::A,
                                 .src_value      = 0x0F,
                                 .expected_value = 0x20,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = true,
                                 .expect_c       = false,
                                 .name           = "D_HalfCarry",
                             },

                             // Normal tests
                             R8Param{
                                 .opcode         = Opcode::ADD_A_A,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg8Name::A,
                                 .src_value      = 0x08,
                                 .expected_value = 0x10,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = true,
                                 .expect_c       = false,
                                 .name           = "A_Normal",
                             },
                             R8Param{
                                 .opcode         = Opcode::ADD_A_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::A,
                                 .src_value      = 0x02,
                                 .expected_value = 0x13,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "E_Normal",
                             },
                             R8Param{
                                 .opcode         = Opcode::ADD_A_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::A,
                                 .src_value      = 0x01,
                                 .expected_value = 0x12,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "H_Normal",
                             },
                             R8Param{
                                 .opcode         = Opcode::ADD_A_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::A,
                                 .src_value      = 0x0A,
                                 .expected_value = 0x1B,
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "L_Normal",
                             }),
                         boyboy::test::cpu::param_name<R8Param>);

INSTANTIATE_TEST_SUITE_P(DecInstructions,
                         SubR8Test,
                         ::testing::Values(
                             // Edge-case tests
                             R8Param{
                                 .opcode         = Opcode::SUB_A_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::A,
                                 .src_value      = 0x12,
                                 .expected_value = 0xFF,
                                 .expect_z       = false,
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .expect_c       = true,
                                 .name           = "B_Underflow",
                             },
                             R8Param{
                                 .opcode         = Opcode::SUB_A_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::A,
                                 .src_value      = 0x02,
                                 .expected_value = 0x0F,
                                 .expect_z       = false,
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .expect_c       = false,
                                 .name           = "C_HalfCarry",
                             },
                             R8Param{
                                 .opcode         = Opcode::SUB_A_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::A,
                                 .src_value      = 0x11,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .expect_n       = true,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "D_Zero",
                             },

                             // Normal tests
                             R8Param{
                                 .opcode         = Opcode::SUB_A_A,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg8Name::A,
                                 .src_value      = 0x01,
                                 .expected_value = 0x00,
                                 .expect_z       = true,
                                 .expect_n       = true,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "A_Normal",
                             },
                             R8Param{
                                 .opcode         = Opcode::SUB_A_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::A,
                                 .src_value      = 0x01,
                                 .expected_value = 0x10,
                                 .expect_z       = false,
                                 .expect_n       = true,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "E_Normal",
                             },
                             R8Param{
                                 .opcode         = Opcode::SUB_A_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::A,
                                 .src_value      = 0x0A,
                                 .expected_value = 0x07,
                                 .expect_z       = false,
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .expect_c       = false,
                                 .name           = "H_Normal",
                             },
                             R8Param{
                                 .opcode         = Opcode::SUB_A_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::A,
                                 .src_value      = 0x0F,
                                 .expected_value = 0x02,
                                 .expect_z       = false,
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .expect_c       = false,
                                 .name           = "L_Normal",
                             }),
                         boyboy::test::cpu::param_name<R8Param>);