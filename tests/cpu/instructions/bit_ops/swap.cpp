/**
 * @file swap.cpp
 * @brief Unit tests for CPU CB-prefixed SWAP instruction.
 *
 * SWAP r8, SWAP [HL]
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
using SwapR8Test = InstrTest<InstrParam>;
using SwapHLTest = InstrTest<InstrParam>;

// -----------------------------
// Test definitions
// -----------------------------
TEST_P(SwapR8Test, Works) { run_test(); }
TEST_P(SwapHLTest, Works) { run_test(); }

// -----------------------------
// Parameter instantiations
// -----------------------------
// SWAP r8
INSTANTIATE_TEST_SUITE_P(CBShiftInstructions,
                         SwapR8Test,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = CBOpcode::SWAP_A,
                                 .src            = boyboy::core::cpu::Reg8Name::A,
                                 .dst            = boyboy::core::cpu::Reg8Name::A,
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b00000000},
                                 .expect_z       = true,
                                 .name           = "SWAP_A_Zero",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SWAP_B,
                                 .src            = boyboy::core::cpu::Reg8Name::B,
                                 .dst            = boyboy::core::cpu::Reg8Name::B,
                                 .src_value      = uint8_t{0b01001100},
                                 .expected_value = uint8_t{0b11000100},
                                 .expect_z       = false,
                                 .name           = "SWAP_B_NoZero",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SWAP_C,
                                 .src            = boyboy::core::cpu::Reg8Name::C,
                                 .dst            = boyboy::core::cpu::Reg8Name::C,
                                 .src_value      = uint8_t{0b10011001},
                                 .expected_value = uint8_t{0b10011001},
                                 .expect_z       = false,
                                 .name           = "SWAP_C_SameValue",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SWAP_D,
                                 .src            = boyboy::core::cpu::Reg8Name::D,
                                 .dst            = boyboy::core::cpu::Reg8Name::D,
                                 .src_value      = uint8_t{0b11111111},
                                 .expected_value = uint8_t{0b11111111},
                                 .expect_z       = false,
                                 .name           = "SWAP_D_AllBitsSet",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SWAP_E,
                                 .src            = boyboy::core::cpu::Reg8Name::E,
                                 .dst            = boyboy::core::cpu::Reg8Name::E,
                                 .src_value      = uint8_t{0b10000000},
                                 .expected_value = uint8_t{0b00001000},
                                 .expect_z       = false,
                                 .name           = "SWAP_E_SingleBit",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SWAP_H,
                                 .src            = boyboy::core::cpu::Reg8Name::H,
                                 .dst            = boyboy::core::cpu::Reg8Name::H,
                                 .src_value      = uint8_t{0b01111111},
                                 .expected_value = uint8_t{0b11110111},
                                 .expect_z       = false,
                                 .name           = "SWAP_H_Positive",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SWAP_L,
                                 .src            = boyboy::core::cpu::Reg8Name::L,
                                 .dst            = boyboy::core::cpu::Reg8Name::L,
                                 .src_value      = uint8_t{0b00010000},
                                 .expected_value = uint8_t{0b00000001},
                                 .expect_z       = false,
                                 .name           = "SWAP_L_SingleNibble",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// SWAP [HL]
INSTANTIATE_TEST_SUITE_P(CBShiftInstructions,
                         SwapHLTest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = CBOpcode::SWAP_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b00000000},
                                 .expect_z       = true,
                                 .name           = "SWAP_HL_Zero",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SWAP_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b01001100},
                                 .expected_value = uint8_t{0b11000100},
                                 .expect_z       = false,
                                 .name           = "SWAP_HL_NoZero",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SWAP_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b10011001},
                                 .expected_value = uint8_t{0b10011001},
                                 .expect_z       = false,
                                 .name           = "SWAP_HL_SameValue",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SWAP_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b11111111},
                                 .expected_value = uint8_t{0b11111111},
                                 .expect_z       = false,
                                 .name           = "SWAP_HL_AllBitsSet",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SWAP_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b10000000},
                                 .expected_value = uint8_t{0b00001000},
                                 .expect_z       = false,
                                 .name           = "SWAP_HL_SingleBit",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SWAP_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b01111111},
                                 .expected_value = uint8_t{0b11110111},
                                 .expect_z       = false,
                                 .name           = "SWAP_HL_Positive",
                             },
                             InstrParam{
                                 .opcode         = CBOpcode::SWAP_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg16Name::HL,
                                 .src_addr       = uint16_t{0xC000},
                                 .src_value      = uint8_t{0b00010000},
                                 .expected_value = uint8_t{0b00000001},
                                 .expect_z       = false,
                                 .name           = "SWAP_HL_SingleNibble",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);
