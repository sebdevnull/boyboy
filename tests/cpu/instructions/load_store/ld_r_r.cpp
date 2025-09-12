/**
 * @file ld_r_r.cpp
 * @brief Unit tests for LD r8, r8' CPU instruction.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

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
using LdR8R8Test = R8Test<R8Param>;

// -----------------------------
// Test definitions
// -----------------------------
TEST_P(LdR8R8Test, Works) { run_test(); }

// -----------------------------
// Parameter instantiations
// -----------------------------
// LD A, r8
INSTANTIATE_TEST_SUITE_P(LdInstructions,
                         LdR8R8Test,
                         ::testing::Values(
                             R8Param{
                                 .opcode         = Opcode::LD_A_A,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg8Name::A,
                                 .src_value      = 0xAA,
                                 .expected_value = 0xAA,
                                 .name           = "LD_A_A",
                                 .skip_assert    = true,
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_A_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::A,
                                 .src_value      = 0xAB,
                                 .expected_value = 0xAB,
                                 .name           = "LD_A_B",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_A_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::A,
                                 .src_value      = 0xAC,
                                 .expected_value = 0xAC,
                                 .name           = "LD_A_C",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_A_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::A,
                                 .src_value      = 0xAD,
                                 .expected_value = 0xAD,
                                 .name           = "LD_A_D",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_A_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::A,
                                 .src_value      = 0xAE,
                                 .expected_value = 0xAE,
                                 .name           = "LD_A_E",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_A_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::A,
                                 .src_value      = 0xAF,
                                 .expected_value = 0xAF,
                                 .name           = "LD_A_H",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_A_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::A,
                                 .src_value      = 0xA0,
                                 .expected_value = 0xA0,
                                 .name           = "LD_A_L",
                             }),
                         boyboy::test::cpu::param_name<R8Param>);

// LD B, r8
INSTANTIATE_TEST_SUITE_P(LdBR8Instructions,
                         LdR8R8Test,
                         ::testing::Values(
                             R8Param{
                                 .opcode         = Opcode::LD_B_A,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg8Name::B,
                                 .src_value      = 0xBA,
                                 .expected_value = 0xBA,
                                 .name           = "LD_B_A",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_B_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::B,
                                 .src_value      = 0xBB,
                                 .expected_value = 0xBB,
                                 .name           = "LD_B_B",
                                 .skip_assert    = true,
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_B_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::B,
                                 .src_value      = 0xBC,
                                 .expected_value = 0xBC,
                                 .name           = "LD_B_C",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_B_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::B,
                                 .src_value      = 0xBD,
                                 .expected_value = 0xBD,
                                 .name           = "LD_B_D",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_B_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::B,
                                 .src_value      = 0xBE,
                                 .expected_value = 0xBE,
                                 .name           = "LD_B_E",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_B_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::B,
                                 .src_value      = 0xBF,
                                 .expected_value = 0xBF,
                                 .name           = "LD_B_H",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_B_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::B,
                                 .src_value      = 0xB0,
                                 .expected_value = 0xB0,
                                 .name           = "LD_B_L",
                             }),
                         boyboy::test::cpu::param_name<R8Param>);

// LD C, r8
INSTANTIATE_TEST_SUITE_P(LdCR8Instructions,
                         LdR8R8Test,
                         ::testing::Values(
                             R8Param{
                                 .opcode         = Opcode::LD_C_A,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg8Name::C,
                                 .src_value      = 0xCA,
                                 .expected_value = 0xCA,
                                 .name           = "LD_C_A",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_C_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::C,
                                 .src_value      = 0xCB,
                                 .expected_value = 0xCB,
                                 .name           = "LD_C_B",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_C_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::C,
                                 .src_value      = 0xCC,
                                 .expected_value = 0xCC,
                                 .name           = "LD_C_C",
                                 .skip_assert    = true,
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_C_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::C,
                                 .src_value      = 0xCD,
                                 .expected_value = 0xCD,
                                 .name           = "LD_C_D",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_C_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::C,
                                 .src_value      = 0xCE,
                                 .expected_value = 0xCE,
                                 .name           = "LD_C_E",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_C_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::C,
                                 .src_value      = 0xCF,
                                 .expected_value = 0xCF,
                                 .name           = "LD_C_H",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_C_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::C,
                                 .src_value      = 0xC0,
                                 .expected_value = 0xC0,
                                 .name           = "LD_C_L",
                             }),
                         boyboy::test::cpu::param_name<R8Param>);

// LD D, r8
INSTANTIATE_TEST_SUITE_P(LdDR8Instructions,
                         LdR8R8Test,
                         ::testing::Values(
                             R8Param{
                                 .opcode         = Opcode::LD_D_A,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg8Name::D,
                                 .src_value      = 0xDA,
                                 .expected_value = 0xDA,
                                 .name           = "LD_D_A",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_D_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::D,
                                 .src_value      = 0xDB,
                                 .expected_value = 0xDB,
                                 .name           = "LD_D_B",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_D_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::D,
                                 .src_value      = 0xDC,
                                 .expected_value = 0xDC,
                                 .name           = "LD_D_C",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_D_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::D,
                                 .src_value      = 0xDD,
                                 .expected_value = 0xDD,
                                 .name           = "LD_D_D",
                                 .skip_assert    = true,
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_D_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::D,
                                 .src_value      = 0xDE,
                                 .expected_value = 0xDE,
                                 .name           = "LD_D_E",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_D_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::D,
                                 .src_value      = 0xDF,
                                 .expected_value = 0xDF,
                                 .name           = "LD_D_H",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_D_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::D,
                                 .src_value      = 0xD0,
                                 .expected_value = 0xD0,
                                 .name           = "LD_D_L",
                             }),
                         boyboy::test::cpu::param_name<R8Param>);

// LD E, r8
INSTANTIATE_TEST_SUITE_P(LdER8Instructions,
                         LdR8R8Test,
                         ::testing::Values(
                             R8Param{
                                 .opcode         = Opcode::LD_E_A,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg8Name::E,
                                 .src_value      = 0xEA,
                                 .expected_value = 0xEA,
                                 .name           = "LD_E_A",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_E_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::E,
                                 .src_value      = 0xEB,
                                 .expected_value = 0xEB,
                                 .name           = "LD_E_B",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_E_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::E,
                                 .src_value      = 0xEC,
                                 .expected_value = 0xEC,
                                 .name           = "LD_E_C",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_E_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::E,
                                 .src_value      = 0xED,
                                 .expected_value = 0xED,
                                 .name           = "LD_E_D",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_E_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::E,
                                 .src_value      = 0xEE,
                                 .expected_value = 0xEE,
                                 .name           = "LD_E_E",
                                 .skip_assert    = true,
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_E_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::E,
                                 .src_value      = 0xEF,
                                 .expected_value = 0xEF,
                                 .name           = "LD_E_H",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_E_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::E,
                                 .src_value      = 0xE0,
                                 .expected_value = 0xE0,
                                 .name           = "LD_E_L",
                             }),
                         boyboy::test::cpu::param_name<R8Param>);

// LD H, r8
INSTANTIATE_TEST_SUITE_P(LdHR8Instructions,
                         LdR8R8Test,
                         ::testing::Values(
                             R8Param{
                                 .opcode         = Opcode::LD_H_A,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg8Name::H,
                                 .src_value      = 0xFA,
                                 .expected_value = 0xFA,
                                 .name           = "LD_H_A",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_H_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::H,
                                 .src_value      = 0xFB,
                                 .expected_value = 0xFB,
                                 .name           = "LD_H_B",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_H_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::H,
                                 .src_value      = 0xFC,
                                 .expected_value = 0xFC,
                                 .name           = "LD_H_C",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_H_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::H,
                                 .src_value      = 0xFD,
                                 .expected_value = 0xFD,
                                 .name           = "LD_H_D",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_H_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::H,
                                 .src_value      = 0xFE,
                                 .expected_value = 0xFE,
                                 .name           = "LD_H_E",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_H_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::H,
                                 .src_value      = 0xFF,
                                 .expected_value = 0xFF,
                                 .name           = "LD_H_H",
                                 .skip_assert    = true,
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_H_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::H,
                                 .src_value      = 0xF0,
                                 .expected_value = 0xF0,
                                 .name           = "LD_H_L",
                             }),
                         boyboy::test::cpu::param_name<R8Param>);

// LD L, r8
INSTANTIATE_TEST_SUITE_P(LdLR8Instructions,
                         LdR8R8Test,
                         ::testing::Values(
                             R8Param{
                                 .opcode         = Opcode::LD_L_A,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg8Name::L,
                                 .src_value      = 0xA0,
                                 .expected_value = 0xA0,
                                 .name           = "LD_L_A",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_L_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::L,
                                 .src_value      = 0xB0,
                                 .expected_value = 0xB0,
                                 .name           = "LD_L_B",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_L_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::L,
                                 .src_value      = 0xC0,
                                 .expected_value = 0xC0,
                                 .name           = "LD_L_C",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_L_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::L,
                                 .src_value      = 0xD0,
                                 .expected_value = 0xD0,
                                 .name           = "LD_L_D",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_L_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::L,
                                 .src_value      = 0xE0,
                                 .expected_value = 0xE0,
                                 .name           = "LD_L_E",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_L_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::L,
                                 .src_value      = 0xF0,
                                 .expected_value = 0xF0,
                                 .name           = "LD_L_H",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_L_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::L,
                                 .src_value      = 0x00,
                                 .expected_value = 0x00,
                                 .name           = "LD_L_L",
                                 .skip_assert    = true,
                             }),
                         boyboy::test::cpu::param_name<R8Param>);
