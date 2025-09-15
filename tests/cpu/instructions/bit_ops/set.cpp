/**
 * @file set.cpp
 * @brief Unit tests for CPU CB-prefixed SET instruction.
 *
 * SET b, r8
 * SET b, [HL]
 *
 * @license GPLv3 (see LICENSE file)
 */

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
using SetR8Test = InstrTest<InstrParam>;
using SetHLTest = InstrTest<InstrParam>;

// -----------------------------
// Test definitions
// -----------------------------
TEST_P(SetR8Test, Works) { run_test(); }
TEST_P(SetHLTest, Works) { run_test(); }

// -----------------------------
// Parameter instantiations
// -----------------------------
// SET b, r8
INSTANTIATE_TEST_SUITE_P(CBBitInstructions,
                         SetR8Test,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = CBOpcode::SET_0_A,
                                 .src            = boyboy::cpu::Reg8Name::A,
                                 .dst            = boyboy::cpu::Reg8Name::A,
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b00000001},
                                 .name           = "SET_0_A_LSBClear",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SET_1_B,
                                 .src            = boyboy::cpu::Reg8Name::B,
                                 .dst            = boyboy::cpu::Reg8Name::B,
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b00000010},
                                 .name           = "SET_1_B_Bit1Clear",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SET_2_C,
                                 .src            = boyboy::cpu::Reg8Name::C,
                                 .dst            = boyboy::cpu::Reg8Name::C,
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b00000100},
                                 .name           = "SET_2_C_Bit2Clear",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SET_3_D,
                                 .src            = boyboy::cpu::Reg8Name::D,
                                 .dst            = boyboy::cpu::Reg8Name::D,
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b00001000},
                                 .name           = "SET_3_D_Bit3Clear",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SET_4_E,
                                 .src            = boyboy::cpu::Reg8Name::E,
                                 .dst            = boyboy::cpu::Reg8Name::E,
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b00010000},
                                 .name           = "SET_4_E_Bit4Clear",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SET_5_H,
                                 .src            = boyboy::cpu::Reg8Name::H,
                                 .dst            = boyboy::cpu::Reg8Name::H,
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b00100000},
                                 .name           = "SET_5_H_Bit5Clear",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SET_6_L,
                                 .src            = boyboy::cpu::Reg8Name::L,
                                 .dst            = boyboy::cpu::Reg8Name::L,
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b01000000},
                                 .name           = "SET_6_L_Bit6Clear",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SET_7_A,
                                 .src            = boyboy::cpu::Reg8Name::A,
                                 .dst            = boyboy::cpu::Reg8Name::A,
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b10000000},
                                 .name           = "SET_7_A_MSBClear",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// SET b, [HL]
INSTANTIATE_TEST_SUITE_P(CBBitInstructions,
                         SetHLTest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = CBOpcode::SET_0_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b00000001},
                                 .name           = "SET_0_AT_HL_LSBClear",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SET_1_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b00000010},
                                 .name           = "SET_1_AT_HL_Bit1Clear",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SET_2_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b00000100},
                                 .name           = "SET_2_AT_HL_Bit2Clear",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SET_3_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b00001000},
                                 .name           = "SET_3_AT_HL_Bit3Clear",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SET_4_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b00010000},
                                 .name           = "SET_4_AT_HL_Bit4Clear",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SET_5_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b00100000},
                                 .name           = "SET_5_AT_HL_Bit5Clear",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SET_6_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b01000000},
                                 .name           = "SET_6_AT_HL_Bit6Clear",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SET_7_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b10000000},
                                 .name           = "SET_7_AT_HL_MSBClear",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);