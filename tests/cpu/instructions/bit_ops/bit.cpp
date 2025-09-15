/**
 * @file bit.cpp
 * @brief Unit tests for CPU CB-prefixed bit manipulation instructions.
 *
 * BIT b, r8
 * BIT b, [HL]
 *
 * @license GPLv3 (see LICENSE file)
 */

// TODO: expected_value not needed for BIT tests, could be made optional

#include <gtest/gtest.h>

// Helpers
#include "helpers/cpu_fixtures.h"
#include "helpers/cpu_params.h"

using boyboy::cpu::CBOpcode;
using boyboy::cpu::Reg16Name;

using boyboy::test::cpu::InstrParam;
using boyboy::test::cpu::InstrTest;
using boyboy::test::cpu::OperandType;

// -----------------------------
// Test types
// -----------------------------
using BitR8Test = InstrTest<InstrParam>;
using BitHLTest = InstrTest<InstrParam>;

// -----------------------------
// Test definitions
// -----------------------------
TEST_P(BitR8Test, Works) { run_test(); }
TEST_P(BitHLTest, Works) { run_test(); }

// -----------------------------
// Parameter instantiations
// -----------------------------
// BIT b, r8
INSTANTIATE_TEST_SUITE_P(CBBitInstructions,
                         BitR8Test,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = CBOpcode::BIT_0_A,
                                 .src            = boyboy::cpu::Reg8Name::A,
                                 .dst            = boyboy::cpu::Reg8Name::A,
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b00000000},
                                 .expect_z       = true,
                                 .expect_h       = true,
                                 .name           = "BIT_0_A_Zero",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::BIT_1_B,
                                 .src            = boyboy::cpu::Reg8Name::B,
                                 .dst            = boyboy::cpu::Reg8Name::B,
                                 .src_value      = uint8_t{0b00000010},
                                 .expected_value = uint8_t{0b00000010},
                                 .expect_z       = false,
                                 .expect_h       = true,
                                 .name           = "BIT_1_B_BitSet",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::BIT_2_C,
                                 .src            = boyboy::cpu::Reg8Name::C,
                                 .dst            = boyboy::cpu::Reg8Name::C,
                                 .src_value      = uint8_t{0b00000100},
                                 .expected_value = uint8_t{0b00000100},
                                 .expect_z       = false,
                                 .expect_h       = true,
                                 .name           = "BIT_2_C_BitSet",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::BIT_3_D,
                                 .src            = boyboy::cpu::Reg8Name::D,
                                 .dst            = boyboy::cpu::Reg8Name::D,
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b00000000},
                                 .expect_z       = true,
                                 .expect_h       = true,
                                 .name           = "BIT_3_D_Zero",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::BIT_4_E,
                                 .src            = boyboy::cpu::Reg8Name::E,
                                 .dst            = boyboy::cpu::Reg8Name::E,
                                 .src_value      = uint8_t{0b00010000},
                                 .expected_value = uint8_t{0b00010000},
                                 .expect_z       = false,
                                 .expect_h       = true,
                                 .name           = "BIT_4_E_BitSet",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::BIT_5_H,
                                 .src            = boyboy::cpu::Reg8Name::H,
                                 .dst            = boyboy::cpu::Reg8Name::H,
                                 .src_value      = uint8_t{0b00100000},
                                 .expected_value = uint8_t{0b00100000},
                                 .expect_z       = false,
                                 .expect_h       = true,
                                 .name           = "BIT_5_H_BitSet",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::BIT_6_L,
                                 .src            = boyboy::cpu::Reg8Name::L,
                                 .dst            = boyboy::cpu::Reg8Name::L,
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b00000000},
                                 .expect_z       = true,
                                 .expect_h       = true,
                                 .name           = "BIT_6_L_Zero",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::BIT_7_A,
                                 .src            = boyboy::cpu::Reg8Name::A,
                                 .dst            = boyboy::cpu::Reg8Name::A,
                                 .src_value      = uint8_t{0b10000000},
                                 .expected_value = uint8_t{0b10000000},
                                 .expect_z       = false,
                                 .expect_h       = true,
                                 .name           = "BIT_7_A_BitSet",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// BIT b, [HL]
INSTANTIATE_TEST_SUITE_P(CBBitInstructions,
                         BitHLTest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = CBOpcode::BIT_0_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b00000000},
                                 .expect_z       = true,
                                 .expect_h       = true,
                                 .name           = "BIT_0_HL_Zero",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::BIT_1_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b00000010},
                                 .expected_value = uint8_t{0b00000010},
                                 .expect_z       = false,
                                 .expect_h       = true,
                                 .name           = "BIT_1_HL_BitSet",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::BIT_2_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b00000100},
                                 .expected_value = uint8_t{0b00000100},
                                 .expect_z       = false,
                                 .expect_h       = true,
                                 .name           = "BIT_2_HL_BitSet",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::BIT_3_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b00000000},
                                 .expect_z       = true,
                                 .expect_h       = true,
                                 .name           = "BIT_3_HL_Zero",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::BIT_4_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b00010000},
                                 .expected_value = uint8_t{0b00010000},
                                 .expect_z       = false,
                                 .expect_h       = true,
                                 .name           = "BIT_4_HL_BitSet",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::BIT_5_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b00100000},
                                 .expected_value = uint8_t{0b00100000},
                                 .expect_z       = false,
                                 .expect_h       = true,
                                 .name           = "BIT_5_HL_BitSet",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::BIT_6_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b00000000},
                                 .expect_z       = true,
                                 .expect_h       = true,
                                 .name           = "BIT_6_HL_Zero",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::BIT_7_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b10000000},
                                 .expected_value = uint8_t{0b10000000},
                                 .expect_z       = false,
                                 .expect_h       = true,
                                 .name           = "BIT_7_HL_BitSet",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);