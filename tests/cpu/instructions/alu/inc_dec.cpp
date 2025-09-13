/**
 * @file inc_dec.cpp
 * @brief Unit tests for INC and DEC CPU instructions.
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

using boyboy::test::cpu::InstrParam;
using boyboy::test::cpu::InstrTest;
using boyboy::test::cpu::OperandType;

// -----------------------------
// Test types
// -----------------------------
using IncR8Test = InstrTest<InstrParam>;
using IncHLTest = InstrTest<InstrParam>;
using DecR8Test = InstrTest<InstrParam>;
using DecHLTest = InstrTest<InstrParam>;

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
                             InstrParam{
                                 .opcode         = Opcode::INC_A,
                                 .src            = Reg8Name::A,
                                 .src_value      = uint8_t{0x0F},
                                 .expected_value = uint8_t{0x10},
                                 .expect_h       = true,
                                 .name           = "A_HalfCarry",
                             },
                             InstrParam{
                                 .opcode         = Opcode::INC_B,
                                 .src            = Reg8Name::B,
                                 .src_value      = uint8_t{0xFF},
                                 .expected_value = uint8_t{0x00},
                                 .expect_z       = true,
                                 .expect_h       = true,
                                 .name           = "B_OverflowZero",
                             },
                             InstrParam{
                                 .opcode         = Opcode::INC_C,
                                 .src            = Reg8Name::C,
                                 .src_value      = uint8_t{0x7E},
                                 .expected_value = uint8_t{0x7F},
                                 .name           = "C_Normal",
                             },
                             InstrParam{
                                 .opcode         = Opcode::INC_D,
                                 .src            = Reg8Name::D,
                                 .src_value      = uint8_t{0x01},
                                 .expected_value = uint8_t{0x02},
                                 .name           = "D_Normal",
                             },
                             InstrParam{
                                 .opcode         = Opcode::INC_E,
                                 .src            = Reg8Name::E,
                                 .src_value      = uint8_t{0x7F},
                                 .expected_value = uint8_t{0x80},
                                 .expect_h       = true,
                                 .name           = "E_HalfCarry",
                             },
                             InstrParam{
                                 .opcode         = Opcode::INC_H,
                                 .src            = Reg8Name::H,
                                 .src_value      = uint8_t{0xFE},
                                 .expected_value = uint8_t{0xFF},
                                 .name           = "H_Normal",
                             },
                             InstrParam{
                                 .opcode         = Opcode::INC_L,
                                 .src            = Reg8Name::L,
                                 .src_value      = uint8_t{0xFE},
                                 .expected_value = uint8_t{0xFF},
                                 .name           = "L_Normal",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// INC [HL]
INSTANTIATE_TEST_SUITE_P(IncInstructions,
                         IncHLTest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::INC_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = std::nullopt,
                                 .src            = Reg16Name::HL,
                                 .src_addr       = 0x0000,
                                 .src_value      = uint8_t{0x00},
                                 .expected_value = uint8_t{0x01},
                                 .name           = "Normal",
                             },
                             InstrParam{
                                 .opcode         = Opcode::INC_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = std::nullopt,
                                 .src            = Reg16Name::HL,
                                 .src_addr       = 0x1234,
                                 .src_value      = uint8_t{0x0F},
                                 .expected_value = uint8_t{0x10},
                                 .expect_h       = true,
                                 .name           = "HalfCarry",
                             },
                             InstrParam{
                                 .opcode         = Opcode::INC_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = std::nullopt,
                                 .src            = Reg16Name::HL,
                                 .src_addr       = 0x5678,
                                 .src_value      = uint8_t{0xFF},
                                 .expected_value = uint8_t{0x00},
                                 .expect_z       = true,
                                 .expect_h       = true,
                                 .name           = "ZeroOverflow",
                             },
                             InstrParam{
                                 .opcode         = Opcode::INC_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = std::nullopt,
                                 .src            = Reg16Name::HL,
                                 .src_addr       = 0x9ABC,
                                 .src_value      = uint8_t{0x7F},
                                 .expected_value = uint8_t{0x80},
                                 .expect_h       = true,
                                 .name           = "SignChange",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// DEC r8
INSTANTIATE_TEST_SUITE_P(DecInstructions,
                         DecR8Test,
                         ::testing::Values(
                             // Edge-case tests
                             InstrParam{
                                 .opcode         = Opcode::DEC_A,
                                 .src            = Reg8Name::A,
                                 .src_value      = uint8_t{0x10},
                                 .expected_value = uint8_t{0x0F},
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .name           = "A_HalfCarry",
                             },
                             InstrParam{
                                 .opcode         = Opcode::DEC_B,
                                 .src            = Reg8Name::B,
                                 .src_value      = uint8_t{0x01},
                                 .expected_value = uint8_t{0x00},
                                 .expect_z       = true,
                                 .expect_n       = true,
                                 .name           = "B_ZeroFlagSet",
                             },
                             InstrParam{
                                 .opcode         = Opcode::DEC_C,
                                 .src            = Reg8Name::C,
                                 .src_value      = uint8_t{0x00},
                                 .expected_value = uint8_t{0xFF},
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .name           = "C_Underflow",
                             },

                             // Normal tests
                             InstrParam{
                                 .opcode         = Opcode::DEC_D,
                                 .src            = Reg8Name::D,
                                 .src_value      = uint8_t{0x02},
                                 .expected_value = uint8_t{0x01},
                                 .expect_n       = true,
                                 .name           = "D_Normal",
                             },
                             InstrParam{
                                 .opcode         = Opcode::DEC_E,
                                 .src            = Reg8Name::E,
                                 .src_value      = uint8_t{0x05},
                                 .expected_value = uint8_t{0x04},
                                 .expect_n       = true,
                                 .name           = "E_Normal",
                             },
                             InstrParam{
                                 .opcode         = Opcode::DEC_H,
                                 .src            = Reg8Name::H,
                                 .src_value      = uint8_t{0x80},
                                 .expected_value = uint8_t{0x7F},
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .name           = "H_HalfCarry",
                             },
                             InstrParam{
                                 .opcode         = Opcode::DEC_L,
                                 .src            = Reg8Name::L,
                                 .src_value      = uint8_t{0x0A},
                                 .expected_value = uint8_t{0x09},
                                 .expect_n       = true,
                                 .name           = "L_Normal",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// DEC [HL]
INSTANTIATE_TEST_SUITE_P(DecInstructions,
                         DecHLTest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::DEC_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = std::nullopt,
                                 .src            = Reg16Name::HL,
                                 .src_addr       = 0x0000,
                                 .src_value      = uint8_t{0xAB},
                                 .expected_value = uint8_t{0xAA},
                                 .expect_n       = true,
                                 .name           = "Normal",
                             },
                             InstrParam{
                                 .opcode         = Opcode::DEC_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = std::nullopt,
                                 .src            = Reg16Name::HL,
                                 .src_addr       = 0x1234,
                                 .src_value      = uint8_t{0x01},
                                 .expected_value = uint8_t{0x00},
                                 .expect_z       = true,
                                 .expect_n       = true,
                                 .name           = "Zero",
                             },
                             InstrParam{
                                 .opcode         = Opcode::DEC_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = std::nullopt,
                                 .src            = Reg16Name::HL,
                                 .src_addr       = 0x5678,
                                 .src_value      = uint8_t{0x10},
                                 .expected_value = uint8_t{0x0F},
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .name           = "HalfCarry",
                             },
                             InstrParam{
                                 .opcode         = Opcode::DEC_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = std::nullopt,
                                 .src            = Reg16Name::HL,
                                 .src_addr       = 0x9ABC,
                                 .src_value      = uint8_t{0x00},
                                 .expected_value = uint8_t{0xFF},
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .name           = "Underflow",
                             },
                             InstrParam{
                                 .opcode         = Opcode::DEC_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = std::nullopt,
                                 .src            = Reg16Name::HL,
                                 .src_addr       = 0xDEF0,
                                 .src_value      = uint8_t{0x80},
                                 .expected_value = uint8_t{0x7F},
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .name           = "SignChange",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);