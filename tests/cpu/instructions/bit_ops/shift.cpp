/**
 * @file shift.cpp
 * @brief Unit tests for CPU CB-prefixed shift instructions.
 *
 * SLA r8, SRA r8, SRL r8
 * SLA [HL], SRA [HL], SRL [HL]
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

// Helpers
#include "helpers/cpu_fixtures.h"
#include "helpers/cpu_params.h"

using boyboy::core::cpu::CBOpcode;
using boyboy::core::cpu::Reg16Name;

using boyboy::test::cpu::InstrParam;
using boyboy::test::cpu::InstrTest;
using boyboy::test::cpu::OperandType;

// -----------------------------
// Test types
// -----------------------------
using SlaR8Test = InstrTest<InstrParam>;
using SraR8Test = InstrTest<InstrParam>;
using SrlR8Test = InstrTest<InstrParam>;
using SlaHLTest = InstrTest<InstrParam>;
using SraHLTest = InstrTest<InstrParam>;
using SrlHLTest = InstrTest<InstrParam>;

// -----------------------------
// Test definitions
// -----------------------------
TEST_P(SlaR8Test, Works) { run_test(); }
TEST_P(SraR8Test, Works) { run_test(); }
TEST_P(SrlR8Test, Works) { run_test(); }
TEST_P(SlaHLTest, Works) { run_test(); }
TEST_P(SraHLTest, Works) { run_test(); }
TEST_P(SrlHLTest, Works) { run_test(); }

// -----------------------------
// Parameter instantiations
// -----------------------------
// SLA r8
INSTANTIATE_TEST_SUITE_P(CBShiftInstructions,
                         SlaR8Test,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = CBOpcode::SLA_A,
                                 .src            = boyboy::core::cpu::Reg8Name::A,
                                 .dst            = boyboy::core::cpu::Reg8Name::A,
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b00000000},
                                 .expect_z       = true,
                                 .expect_c       = false,
                                 .name           = "SLA_A_Zero",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SLA_B,
                                 .src            = boyboy::core::cpu::Reg8Name::B,
                                 .dst            = boyboy::core::cpu::Reg8Name::B,
                                 .src_value      = uint8_t{0b01001100},
                                 .expected_value = uint8_t{0b10011000},
                                 .expect_z       = false,
                                 .expect_c       = false,
                                 .name           = "SLA_B_NoZero",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SLA_C,
                                 .src            = boyboy::core::cpu::Reg8Name::C,
                                 .dst            = boyboy::core::cpu::Reg8Name::C,
                                 .src_value      = uint8_t{0b10011001},
                                 .expected_value = uint8_t{0b00110010},
                                 .expect_z       = false,
                                 .expect_c       = true,
                                 .name           = "SLA_C_Carry",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SLA_D,
                                 .src            = boyboy::core::cpu::Reg8Name::D,
                                 .dst            = boyboy::core::cpu::Reg8Name::D,
                                 .src_value      = uint8_t{0b11111111},
                                 .expected_value = uint8_t{0b11111110},
                                 .expect_z       = false,
                                 .expect_c       = true,
                                 .name           = "SLA_D_AllBitsSet",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SLA_E,
                                 .src            = boyboy::core::cpu::Reg8Name::E,
                                 .dst            = boyboy::core::cpu::Reg8Name::E,
                                 .src_value      = uint8_t{0b01111111},
                                 .expected_value = uint8_t{0b11111110},
                                 .expect_z       = false,
                                 .expect_c       = false,
                                 .name           = "SLA_E_Positive",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SLA_H,
                                 .src            = boyboy::core::cpu::Reg8Name::H,
                                 .dst            = boyboy::core::cpu::Reg8Name::H,
                                 .src_value      = uint8_t{0b10000000},
                                 .expected_value = uint8_t{0b00000000},
                                 .expect_z       = true,
                                 .expect_c       = true,
                                 .name           = "SLA_H_MSBSet",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SLA_L,
                                 .src            = boyboy::core::cpu::Reg8Name::L,
                                 .dst            = boyboy::core::cpu::Reg8Name::L,
                                 .src_value      = uint8_t{0b00000001},
                                 .expected_value = uint8_t{0b00000010},
                                 .expect_z       = false,
                                 .expect_c       = false,
                                 .name           = "SLA_L_LSBSet",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// SRA r8
INSTANTIATE_TEST_SUITE_P(CBShiftInstructions,
                         SraR8Test,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = CBOpcode::SRA_A,
                                 .src            = boyboy::core::cpu::Reg8Name::A,
                                 .dst            = boyboy::core::cpu::Reg8Name::A,
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b00000000},
                                 .expect_z       = true,
                                 .expect_c       = false,
                                 .name           = "SRA_A_Zero",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SRA_B,
                                 .src            = boyboy::core::cpu::Reg8Name::B,
                                 .dst            = boyboy::core::cpu::Reg8Name::B,
                                 .src_value      = uint8_t{0b01001100},
                                 .expected_value = uint8_t{0b00100110},
                                 .expect_z       = false,
                                 .expect_c       = false,
                                 .name           = "SRA_B_NoZero",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SRA_C,
                                 .src            = boyboy::core::cpu::Reg8Name::C,
                                 .dst            = boyboy::core::cpu::Reg8Name::C,
                                 .src_value      = uint8_t{0b10011001},
                                 .expected_value = uint8_t{0b11001100},
                                 .expect_z       = false,
                                 .expect_c       = true,
                                 .name           = "SRA_C_Carry",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SRA_D,
                                 .src            = boyboy::core::cpu::Reg8Name::D,
                                 .dst            = boyboy::core::cpu::Reg8Name::D,
                                 .src_value      = uint8_t{0b11111111},
                                 .expected_value = uint8_t{0b11111111},
                                 .expect_z       = false,
                                 .expect_c       = true,
                                 .name           = "SRA_D_AllBitsSet",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SRA_E,
                                 .src            = boyboy::core::cpu::Reg8Name::E,
                                 .dst            = boyboy::core::cpu::Reg8Name::E,
                                 .src_value      = uint8_t{0b01111111},
                                 .expected_value = uint8_t{0b00111111},
                                 .expect_z       = false,
                                 .expect_c       = true,
                                 .name           = "SRA_E_Positive",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SRA_H,
                                 .src            = boyboy::core::cpu::Reg8Name::H,
                                 .dst            = boyboy::core::cpu::Reg8Name::H,
                                 .src_value      = uint8_t{0b10000000},
                                 .expected_value = uint8_t{0b11000000},
                                 .expect_z       = false,
                                 .expect_c       = false,
                                 .name           = "SRA_H_MSBSet",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SRA_L,
                                 .src            = boyboy::core::cpu::Reg8Name::L,
                                 .dst            = boyboy::core::cpu::Reg8Name::L,
                                 .src_value      = uint8_t{0b00000001},
                                 .expected_value = uint8_t{0b00000000},
                                 .expect_z       = true,
                                 .expect_c       = true,
                                 .name           = "SRA_L_LSBSet",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// SRL r8
INSTANTIATE_TEST_SUITE_P(CBShiftInstructions,
                         SrlR8Test,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = CBOpcode::SRL_A,
                                 .src            = boyboy::core::cpu::Reg8Name::A,
                                 .dst            = boyboy::core::cpu::Reg8Name::A,
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b00000000},
                                 .expect_z       = true,
                                 .expect_c       = false,
                                 .name           = "SRL_A_Zero",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SRL_B,
                                 .src            = boyboy::core::cpu::Reg8Name::B,
                                 .dst            = boyboy::core::cpu::Reg8Name::B,
                                 .src_value      = uint8_t{0b01001100},
                                 .expected_value = uint8_t{0b00100110},
                                 .expect_z       = false,
                                 .expect_c       = false,
                                 .name           = "SRL_B_NoZero",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SRL_C,
                                 .src            = boyboy::core::cpu::Reg8Name::C,
                                 .dst            = boyboy::core::cpu::Reg8Name::C,
                                 .src_value      = uint8_t{0b10011001},
                                 .expected_value = uint8_t{0b01001100},
                                 .expect_z       = false,
                                 .expect_c       = true,
                                 .name           = "SRL_C_Carry",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SRL_D,
                                 .src            = boyboy::core::cpu::Reg8Name::D,
                                 .dst            = boyboy::core::cpu::Reg8Name::D,
                                 .src_value      = uint8_t{0b11111111},
                                 .expected_value = uint8_t{0b01111111},
                                 .expect_z       = false,
                                 .expect_c       = true,
                                 .name           = "SRL_D_AllBitsSet",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SRL_E,
                                 .src            = boyboy::core::cpu::Reg8Name::E,
                                 .dst            = boyboy::core::cpu::Reg8Name::E,
                                 .src_value      = uint8_t{0b01111111},
                                 .expected_value = uint8_t{0b00111111},
                                 .expect_z       = false,
                                 .expect_c       = true,
                                 .name           = "SRL_E_Positive",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SRL_H,
                                 .src            = boyboy::core::cpu::Reg8Name::H,
                                 .dst            = boyboy::core::cpu::Reg8Name::H,
                                 .src_value      = uint8_t{0b10000000},
                                 .expected_value = uint8_t{0b01000000},
                                 .expect_z       = false,
                                 .expect_c       = false,
                                 .name           = "SRL_H_MSBSet",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SRL_L,
                                 .src            = boyboy::core::cpu::Reg8Name::L,
                                 .dst            = boyboy::core::cpu::Reg8Name::L,
                                 .src_value      = uint8_t{0b00000001},
                                 .expected_value = uint8_t{0b00000000},
                                 .expect_z       = true,
                                 .expect_c       = true,
                                 .name           = "SRL_L_LSBSet",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// SLA [HL]
INSTANTIATE_TEST_SUITE_P(CBShiftInstructions,
                         SlaHLTest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = CBOpcode::SLA_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b00000000},
                                 .expect_z       = true,
                                 .expect_c       = false,
                                 .name           = "SLA_HL_Zero",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SLA_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b01001100},
                                 .expected_value = uint8_t{0b10011000},
                                 .expect_z       = false,
                                 .expect_c       = false,
                                 .name           = "SLA_HL_Positive",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SLA_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b10011001},
                                 .expected_value = uint8_t{0b00110010},
                                 .expect_z       = false,
                                 .expect_c       = true,
                                 .name           = "SLA_HL_Carry",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SLA_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b11111111},
                                 .expected_value = uint8_t{0b11111110},
                                 .expect_z       = false,
                                 .expect_c       = true,
                                 .name           = "SLA_HL_AllBitsSet",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// SRA [HL]
INSTANTIATE_TEST_SUITE_P(CBShiftInstructions,
                         SraHLTest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = CBOpcode::SRA_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b00000000},
                                 .expect_z       = true,
                                 .expect_c       = false,
                                 .name           = "SRA_HL_Zero",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SRA_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b01001100},
                                 .expected_value = uint8_t{0b00100110},
                                 .expect_z       = false,
                                 .expect_c       = false,
                                 .name           = "SRA_HL_Positive",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SRA_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b10011001},
                                 .expected_value = uint8_t{0b11001100},
                                 .expect_z       = false,
                                 .expect_c       = true,
                                 .name           = "SRA_HL_Carry",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SRA_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b11111111},
                                 .expected_value = uint8_t{0b11111111},
                                 .expect_z       = false,
                                 .expect_c       = true,
                                 .name           = "SRA_HL_AllBitsSet",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// SRL [HL]
INSTANTIATE_TEST_SUITE_P(CBShiftInstructions,
                         SrlHLTest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = CBOpcode::SRL_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b00000000},
                                 .expect_z       = true,
                                 .expect_c       = false,
                                 .name           = "SRL_HL_Zero",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SRL_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b01001100},
                                 .expected_value = uint8_t{0b00100110},
                                 .expect_z       = false,
                                 .expect_c       = false,
                                 .name           = "SRL_HL_Positive_NoCarry",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SRL_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b10011001},
                                 .expected_value = uint8_t{0b01001100},
                                 .expect_z       = false,
                                 .expect_c       = true,
                                 .name           = "SRL_HL_Negative_Carry",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SRL_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b11111111},
                                 .expected_value = uint8_t{0b01111111},
                                 .expect_z       = false,
                                 .expect_c       = true,
                                 .name           = "SRL_HL_AllBitsSet",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);