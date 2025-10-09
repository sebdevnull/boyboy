/**
 * @file res.cpp
 * @brief Unit tests for CPU CB-prefixed RES instruction.
 *
 * RES b, r8
 * RES b, [HL]
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
using ResR8Test = InstrTest<InstrParam>;
using ResHLTest = InstrTest<InstrParam>;

// -----------------------------
// Test definitions
// -----------------------------
TEST_P(ResR8Test, Works) { run_test(); }
TEST_P(ResHLTest, Works) { run_test(); }

// -----------------------------
// Parameter instantiations
// -----------------------------
// RES b, r8
INSTANTIATE_TEST_SUITE_P(CBBitInstructions,
                         ResR8Test,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = CBOpcode::RES_0_A,
                                 .src            = boyboy::core::cpu::Reg8Name::A,
                                 .dst            = boyboy::core::cpu::Reg8Name::A,
                                 .src_value      = uint8_t{0b00000001},
                                 .expected_value = uint8_t{0b00000000},
                                 .name           = "RES_0_A_LSBSet",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::RES_1_B,
                                 .src            = boyboy::core::cpu::Reg8Name::B,
                                 .dst            = boyboy::core::cpu::Reg8Name::B,
                                 .src_value      = uint8_t{0b00000010},
                                 .expected_value = uint8_t{0b00000000},
                                 .name           = "RES_1_B_Bit1Set",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::RES_2_C,
                                 .src            = boyboy::core::cpu::Reg8Name::C,
                                 .dst            = boyboy::core::cpu::Reg8Name::C,
                                 .src_value      = uint8_t{0b00000100},
                                 .expected_value = uint8_t{0b00000000},
                                 .name           = "RES_2_C_Bit2Set",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::RES_3_D,
                                 .src            = boyboy::core::cpu::Reg8Name::D,
                                 .dst            = boyboy::core::cpu::Reg8Name::D,
                                 .src_value      = uint8_t{0b00001000},
                                 .expected_value = uint8_t{0b00000000},
                                 .name           = "RES_3_D_Bit3Set",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::RES_4_E,
                                 .src            = boyboy::core::cpu::Reg8Name::E,
                                 .dst            = boyboy::core::cpu::Reg8Name::E,
                                 .src_value      = uint8_t{0b00010000},
                                 .expected_value = uint8_t{0b00000000},
                                 .name           = "RES_4_E_Bit4Set",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::RES_5_H,
                                 .src            = boyboy::core::cpu::Reg8Name::H,
                                 .dst            = boyboy::core::cpu::Reg8Name::H,
                                 .src_value      = uint8_t{0b00100000},
                                 .expected_value = uint8_t{0b00000000},
                                 .name           = "RES_5_H_Bit5Set",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::RES_6_L,
                                 .src            = boyboy::core::cpu::Reg8Name::L,
                                 .dst            = boyboy::core::cpu::Reg8Name::L,
                                 .src_value      = uint8_t{0b01000000},
                                 .expected_value = uint8_t{0b00000000},
                                 .name           = "RES_6_L_Bit6Set",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::RES_7_A,
                                 .src            = boyboy::core::cpu::Reg8Name::A,
                                 .dst            = boyboy::core::cpu::Reg8Name::A,
                                 .src_value      = uint8_t{0b10000000},
                                 .expected_value = uint8_t{0b00000000},
                                 .name           = "RES_7_A_MSBSet",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// RES b, [HL]
INSTANTIATE_TEST_SUITE_P(CBBitInstructions,
                         ResHLTest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = CBOpcode::RES_0_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b00000001},
                                 .expected_value = uint8_t{0b00000000},
                                 .name           = "RES_0_AT_HL_LSBSet",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::RES_1_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b00000010},
                                 .expected_value = uint8_t{0b00000000},
                                 .name           = "RES_1_AT_HL_Bit1Set",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::RES_2_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b00000100},
                                 .expected_value = uint8_t{0b00000000},
                                 .name           = "RES_2_AT_HL_Bit2Set",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::RES_3_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b00001000},
                                 .expected_value = uint8_t{0b00000000},
                                 .name           = "RES_3_AT_HL_Bit3Set",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::RES_4_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b00010000},
                                 .expected_value = uint8_t{0b00000000},
                                 .name           = "RES_4_AT_HL_Bit4Set",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::RES_5_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b00100000},
                                 .expected_value = uint8_t{0b00000000},
                                 .name           = "RES_5_AT_HL_Bit5Set",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::RES_6_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b01000000},
                                 .expected_value = uint8_t{0b00000000},
                                 .name           = "RES_6_AT_HL_Bit6Set",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::RES_7_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b10000000},
                                 .expected_value = uint8_t{0b00000000},
                                 .name           = "RES_7_AT_HL_MSBSet",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);