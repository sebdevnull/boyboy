/**
 * @file ld_r_r.cpp
 * @brief Unit tests for LD r8, r8' CPU instruction.
 *
 * LD r8, r8'
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

// Helpers
#include "helpers/cpu_fixtures.h"
#include "helpers/cpu_params.h"

using boyboy::core::cpu::Opcode;
using boyboy::core::cpu::Reg8Name;

using boyboy::test::cpu::InstrParam;
using boyboy::test::cpu::InstrTest;

// -----------------------------
// Test types
// -----------------------------
using LdR8R8Test = InstrTest<InstrParam>;

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
                             InstrParam{
                                 .opcode         = Opcode::LD_A_A,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg8Name::A,
                                 .src_value      = uint8_t{0xAA},
                                 .expected_value = uint8_t{0xAA},
                                 .name           = "LD_A_A",
                                 .skip_assert    = true,
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_A_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::A,
                                 .src_value      = uint8_t{0xAB},
                                 .expected_value = uint8_t{0xAB},
                                 .name           = "LD_A_B",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_A_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::A,
                                 .src_value      = uint8_t{0xAC},
                                 .expected_value = uint8_t{0xAC},
                                 .name           = "LD_A_C",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_A_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::A,
                                 .src_value      = uint8_t{0xAD},
                                 .expected_value = uint8_t{0xAD},
                                 .name           = "LD_A_D",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_A_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::A,
                                 .src_value      = uint8_t{0xAE},
                                 .expected_value = uint8_t{0xAE},
                                 .name           = "LD_A_E",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_A_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::A,
                                 .src_value      = uint8_t{0xAF},
                                 .expected_value = uint8_t{0xAF},
                                 .name           = "LD_A_H",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_A_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::A,
                                 .src_value      = uint8_t{0xA0},
                                 .expected_value = uint8_t{0xA0},
                                 .name           = "LD_A_L",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// LD B, r8
INSTANTIATE_TEST_SUITE_P(LdBR8Instructions,
                         LdR8R8Test,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::LD_B_A,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg8Name::B,
                                 .src_value      = uint8_t{0xBA},
                                 .expected_value = uint8_t{0xBA},
                                 .name           = "LD_B_A",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_B_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::B,
                                 .src_value      = uint8_t{0xBB},
                                 .expected_value = uint8_t{0xBB},
                                 .name           = "LD_B_B",
                                 .skip_assert    = true,
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_B_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::B,
                                 .src_value      = uint8_t{0xBC},
                                 .expected_value = uint8_t{0xBC},
                                 .name           = "LD_B_C",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_B_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::B,
                                 .src_value      = uint8_t{0xBD},
                                 .expected_value = uint8_t{0xBD},
                                 .name           = "LD_B_D",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_B_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::B,
                                 .src_value      = uint8_t{0xBE},
                                 .expected_value = uint8_t{0xBE},
                                 .name           = "LD_B_E",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_B_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::B,
                                 .src_value      = uint8_t{0xBF},
                                 .expected_value = uint8_t{0xBF},
                                 .name           = "LD_B_H",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_B_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::B,
                                 .src_value      = uint8_t{0xB0},
                                 .expected_value = uint8_t{0xB0},
                                 .name           = "LD_B_L",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// LD C, r8
INSTANTIATE_TEST_SUITE_P(LdCR8Instructions,
                         LdR8R8Test,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::LD_C_A,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg8Name::C,
                                 .src_value      = uint8_t{0xCA},
                                 .expected_value = uint8_t{0xCA},
                                 .name           = "LD_C_A",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_C_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::C,
                                 .src_value      = uint8_t{0xCB},
                                 .expected_value = uint8_t{0xCB},
                                 .name           = "LD_C_B",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_C_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::C,
                                 .src_value      = uint8_t{0xCC},
                                 .expected_value = uint8_t{0xCC},
                                 .name           = "LD_C_C",
                                 .skip_assert    = true,
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_C_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::C,
                                 .src_value      = uint8_t{0xCD},
                                 .expected_value = uint8_t{0xCD},
                                 .name           = "LD_C_D",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_C_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::C,
                                 .src_value      = uint8_t{0xCE},
                                 .expected_value = uint8_t{0xCE},
                                 .name           = "LD_C_E",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_C_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::C,
                                 .src_value      = uint8_t{0xCF},
                                 .expected_value = uint8_t{0xCF},
                                 .name           = "LD_C_H",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_C_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::C,
                                 .src_value      = uint8_t{0xC0},
                                 .expected_value = uint8_t{0xC0},
                                 .name           = "LD_C_L",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// LD D, r8
INSTANTIATE_TEST_SUITE_P(LdDR8Instructions,
                         LdR8R8Test,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::LD_D_A,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg8Name::D,
                                 .src_value      = uint8_t{0xDA},
                                 .expected_value = uint8_t{0xDA},
                                 .name           = "LD_D_A",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_D_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::D,
                                 .src_value      = uint8_t{0xDB},
                                 .expected_value = uint8_t{0xDB},
                                 .name           = "LD_D_B",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_D_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::D,
                                 .src_value      = uint8_t{0xDC},
                                 .expected_value = uint8_t{0xDC},
                                 .name           = "LD_D_C",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_D_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::D,
                                 .src_value      = uint8_t{0xDD},
                                 .expected_value = uint8_t{0xDD},
                                 .name           = "LD_D_D",
                                 .skip_assert    = true,
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_D_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::D,
                                 .src_value      = uint8_t{0xDE},
                                 .expected_value = uint8_t{0xDE},
                                 .name           = "LD_D_E",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_D_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::D,
                                 .src_value      = uint8_t{0xDF},
                                 .expected_value = uint8_t{0xDF},
                                 .name           = "LD_D_H",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_D_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::D,
                                 .src_value      = uint8_t{0xD0},
                                 .expected_value = uint8_t{0xD0},
                                 .name           = "LD_D_L",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// LD E, r8
INSTANTIATE_TEST_SUITE_P(LdER8Instructions,
                         LdR8R8Test,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::LD_E_A,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg8Name::E,
                                 .src_value      = uint8_t{0xEA},
                                 .expected_value = uint8_t{0xEA},
                                 .name           = "LD_E_A",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_E_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::E,
                                 .src_value      = uint8_t{0xEB},
                                 .expected_value = uint8_t{0xEB},
                                 .name           = "LD_E_B",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_E_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::E,
                                 .src_value      = uint8_t{0xEC},
                                 .expected_value = uint8_t{0xEC},
                                 .name           = "LD_E_C",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_E_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::E,
                                 .src_value      = uint8_t{0xED},
                                 .expected_value = uint8_t{0xED},
                                 .name           = "LD_E_D",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_E_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::E,
                                 .src_value      = uint8_t{0xEE},
                                 .expected_value = uint8_t{0xEE},
                                 .name           = "LD_E_E",
                                 .skip_assert    = true,
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_E_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::E,
                                 .src_value      = uint8_t{0xEF},
                                 .expected_value = uint8_t{0xEF},
                                 .name           = "LD_E_H",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_E_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::E,
                                 .src_value      = uint8_t{0xE0},
                                 .expected_value = uint8_t{0xE0},
                                 .name           = "LD_E_L",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// LD H, r8
INSTANTIATE_TEST_SUITE_P(LdHR8Instructions,
                         LdR8R8Test,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::LD_H_A,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg8Name::H,
                                 .src_value      = uint8_t{0xFA},
                                 .expected_value = uint8_t{0xFA},
                                 .name           = "LD_H_A",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_H_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::H,
                                 .src_value      = uint8_t{0xFB},
                                 .expected_value = uint8_t{0xFB},
                                 .name           = "LD_H_B",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_H_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::H,
                                 .src_value      = uint8_t{0xFC},
                                 .expected_value = uint8_t{0xFC},
                                 .name           = "LD_H_C",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_H_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::H,
                                 .src_value      = uint8_t{0xFD},
                                 .expected_value = uint8_t{0xFD},
                                 .name           = "LD_H_D",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_H_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::H,
                                 .src_value      = uint8_t{0xFE},
                                 .expected_value = uint8_t{0xFE},
                                 .name           = "LD_H_E",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_H_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::H,
                                 .src_value      = uint8_t{0xFF},
                                 .expected_value = uint8_t{0xFF},
                                 .name           = "LD_H_H",
                                 .skip_assert    = true,
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_H_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::H,
                                 .src_value      = uint8_t{0xF0},
                                 .expected_value = uint8_t{0xF0},
                                 .name           = "LD_H_L",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// LD L, r8
INSTANTIATE_TEST_SUITE_P(LdLR8Instructions,
                         LdR8R8Test,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::LD_L_A,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg8Name::L,
                                 .src_value      = uint8_t{0xA0},
                                 .expected_value = uint8_t{0xA0},
                                 .name           = "LD_L_A",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_L_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::L,
                                 .src_value      = uint8_t{0xB0},
                                 .expected_value = uint8_t{0xB0},
                                 .name           = "LD_L_B",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_L_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::L,
                                 .src_value      = uint8_t{0xC0},
                                 .expected_value = uint8_t{0xC0},
                                 .name           = "LD_L_C",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_L_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::L,
                                 .src_value      = uint8_t{0xD0},
                                 .expected_value = uint8_t{0xD0},
                                 .name           = "LD_L_D",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_L_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::L,
                                 .src_value      = uint8_t{0xE0},
                                 .expected_value = uint8_t{0xE0},
                                 .name           = "LD_L_E",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_L_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::L,
                                 .src_value      = uint8_t{0xF0},
                                 .expected_value = uint8_t{0xF0},
                                 .name           = "LD_L_H",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_L_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::L,
                                 .src_value      = uint8_t{0x00},
                                 .expected_value = uint8_t{0x00},
                                 .name           = "LD_L_L",
                                 .skip_assert    = true,
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);
