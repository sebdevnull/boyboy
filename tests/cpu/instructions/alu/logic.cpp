/**
 * @file logic.cpp
 * @brief Unit tests for AND, OR, XOR and CP CPU instructions.
 *
 * AND A, r8; AND A, n8; AND A, [HL]
 * OR A, r8; OR A, n8; OR A, [HL]
 * XOR A, r8; XOR A, n8; XOR A, [HL]
 * CP A, r8; CP A, n8; CP A, [HL]
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

// boyboy
#include "boyboy/mmu_constants.h"

// Helpers
#include "helpers/cpu_fixtures.h"
#include "helpers/cpu_params.h"

using boyboy::cpu::Opcode;
using boyboy::cpu::Reg16Name;
using boyboy::cpu::Reg8Name;
using boyboy::mmu::VRAMStart;

using boyboy::test::cpu::InstrParam;
using boyboy::test::cpu::InstrTest;
using boyboy::test::cpu::OperandType;

// -----------------------------
// Test types
// -----------------------------
using ANDR8Test  = InstrTest<InstrParam>;
using ANDHLTest  = InstrTest<InstrParam>;
using ANDImmTest = InstrTest<InstrParam>;
using ORR8Test   = InstrTest<InstrParam>;
using ORHLTest   = InstrTest<InstrParam>;
using ORImmTest  = InstrTest<InstrParam>;
using XORR8Test  = InstrTest<InstrParam>;
using XORHLTest  = InstrTest<InstrParam>;
using XORImmTest = InstrTest<InstrParam>;
using CPR8Test   = InstrTest<InstrParam>;
using CPHLTest   = InstrTest<InstrParam>;
using CPImmTest  = InstrTest<InstrParam>;

// -----------------------------
// Test definitions
// -----------------------------
TEST_P(ANDR8Test, Works) { run_test(); }
TEST_P(ANDHLTest, Works) { run_test(); }
TEST_P(ANDImmTest, Works) { run_test(); }
TEST_P(ORR8Test, Works) { run_test(); }
TEST_P(ORHLTest, Works) { run_test(); }
TEST_P(ORImmTest, Works) { run_test(); }
TEST_P(XORR8Test, Works) { run_test(); }
TEST_P(XORHLTest, Works) { run_test(); }
TEST_P(XORImmTest, Works) { run_test(); }
TEST_P(CPR8Test, Works) { run_test(); }
TEST_P(CPHLTest, Works) { run_test(); }
TEST_P(CPImmTest, Works) { run_test(); }

// -----------------------------
// Parameter instantiations
// -----------------------------
// AND A, r8
INSTANTIATE_TEST_SUITE_P(AndInstructions,
                         ANDR8Test,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::AND_A_A,
                                 .src            = Reg8Name::A,
                                 .src_value      = uint8_t{0xAA},
                                 .expected_value = uint8_t{0xAA},
                                 .expect_h       = true,
                                 .name           = "A_SameBits",
                             },
                             InstrParam{
                                 .opcode         = Opcode::AND_A_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = uint8_t{0x00},
                                 .expected_value = uint8_t{0x00},
                                 .expect_z       = true,
                                 .expect_h       = true,
                                 .name           = "B_Zero",
                             },
                             InstrParam{
                                 .opcode         = Opcode::AND_A_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = uint8_t{0x55},
                                 .expected_value = uint8_t{0x00},
                                 .expect_z       = true,
                                 .expect_h       = true,
                                 .name           = "C_NoCommonBits",
                             },

                             InstrParam{
                                 .opcode         = Opcode::AND_A_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = uint8_t{0xF0},
                                 .expected_value = uint8_t{0xA0},
                                 .expect_h       = true,
                                 .name           = "D_UpperNibble",
                             },
                             InstrParam{
                                 .opcode         = Opcode::AND_A_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = uint8_t{0x0F},
                                 .expected_value = uint8_t{0x0A},
                                 .expect_h       = true,
                                 .name           = "E_LowerNibble",
                             },
                             InstrParam{
                                 .opcode         = Opcode::AND_A_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = uint8_t{0xAA},
                                 .expected_value = uint8_t{0xAA},
                                 .expect_h       = true,
                                 .name           = "H_SameBits",
                             },
                             InstrParam{
                                 .opcode         = Opcode::AND_A_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = uint8_t{0xFF},
                                 .expected_value = uint8_t{0xAA},
                                 .expect_h       = true,
                                 .name           = "L_SameBits",
                             }

                             ),
                         boyboy::test::cpu::param_name<InstrParam>);

// AND A, [HL]
INSTANTIATE_TEST_SUITE_P(AndInstructions,
                         ANDHLTest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::AND_A_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = VRAMStart,
                                 .initial_a      = 0xF0,
                                 .src_value      = uint8_t{0xAA},
                                 .expected_value = uint8_t{0xA0},
                                 .expect_h       = true,
                                 .name           = "Normal",
                             },
                             InstrParam{
                                 .opcode         = Opcode::AND_A_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = VRAMStart + 1,
                                 .initial_a      = 0x0F,
                                 .src_value      = uint8_t{0xF0},
                                 .expected_value = uint8_t{0x00},
                                 .expect_z       = true,
                                 .expect_h       = true,
                                 .name           = "Zero",
                             },
                             InstrParam{
                                 .opcode         = Opcode::AND_A_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = VRAMStart + 2,
                                 .initial_a      = 0x55,
                                 .src_value      = uint8_t{0x55},
                                 .expected_value = uint8_t{0x55},
                                 .expect_h       = true,
                                 .name           = "SameValue",
                             },
                             InstrParam{
                                 .opcode         = Opcode::AND_A_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = VRAMStart + 3,
                                 .initial_a      = 0xAA,
                                 .src_value      = uint8_t{0xFF},
                                 .expected_value = uint8_t{0xAA},
                                 .expect_h       = true,
                                 .name           = "AllBits",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// AND A, n8
INSTANTIATE_TEST_SUITE_P(AndInstructions,
                         ANDImmTest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::AND_A_N8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xF0,
                                 .src_value      = uint8_t{0xAA},
                                 .expected_value = uint8_t{0xA0},
                                 .expect_h       = true,
                                 .name           = "Normal",
                             },
                             InstrParam{
                                 .opcode         = Opcode::AND_A_N8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x0F,
                                 .src_value      = uint8_t{0xF0},
                                 .expected_value = uint8_t{0x00},
                                 .expect_z       = true,
                                 .expect_h       = true,
                                 .name           = "Zero",
                             },
                             InstrParam{
                                 .opcode         = Opcode::AND_A_N8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x55,
                                 .src_value      = uint8_t{0x55},
                                 .expected_value = uint8_t{0x55},
                                 .expect_h       = true,
                                 .name           = "SameValue",
                             },
                             InstrParam{
                                 .opcode         = Opcode::AND_A_N8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = uint8_t{0xFF},
                                 .expected_value = uint8_t{0xAA},
                                 .expect_h       = true,
                                 .name           = "AllBits",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// OR A, r8
INSTANTIATE_TEST_SUITE_P(OrInstructions,
                         ORR8Test,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::OR_A_A,
                                 .src            = Reg8Name::A,
                                 .src_value      = uint8_t{0x55},
                                 .expected_value = uint8_t{0x55},
                                 .name           = "A_A_Normal",
                             },
                             InstrParam{
                                 .opcode         = Opcode::OR_A_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x55,
                                 .src_value      = uint8_t{0x0F},
                                 .expected_value = uint8_t{0x5F},
                                 .name           = "A_B_Normal",
                             },
                             InstrParam{
                                 .opcode         = Opcode::OR_A_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x00,
                                 .src_value      = uint8_t{0x00},
                                 .expected_value = uint8_t{0x00},
                                 .expect_z       = true,
                                 .name           = "A_C_Zero",
                             },
                             InstrParam{
                                 .opcode         = Opcode::OR_A_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xF0,
                                 .src_value      = uint8_t{0x07},
                                 .expected_value = uint8_t{0xF7},
                                 .name           = "A_D_Normal",
                             },
                             InstrParam{
                                 .opcode         = Opcode::OR_A_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x0F,
                                 .src_value      = uint8_t{0xF0},
                                 .expected_value = uint8_t{0xFF},
                                 .name           = "A_E_Normal",
                             },
                             InstrParam{
                                 .opcode         = Opcode::OR_A_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = uint8_t{0x55},
                                 .expected_value = uint8_t{0xFF},
                                 .name           = "A_H_Normal",
                             },
                             InstrParam{
                                 .opcode         = Opcode::OR_A_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x00,
                                 .src_value      = uint8_t{0xFF},
                                 .expected_value = uint8_t{0xFF},
                                 .name           = "A_L_Normal",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// OR A, [HL]
INSTANTIATE_TEST_SUITE_P(OrInstructions,
                         ORHLTest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::OR_A_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = VRAMStart,
                                 .initial_a      = 0xF0,
                                 .src_value      = uint8_t{0x0A},
                                 .expected_value = uint8_t{0xFA},
                                 .name           = "Normal",
                             },
                             InstrParam{
                                 .opcode         = Opcode::OR_A_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = VRAMStart + 1,
                                 .initial_a      = 0x00,
                                 .src_value      = uint8_t{0x00},
                                 .expected_value = uint8_t{0x00},
                                 .expect_z       = true,
                                 .name           = "Zero",
                             },
                             InstrParam{
                                 .opcode         = Opcode::OR_A_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = VRAMStart + 2,
                                 .initial_a      = 0x55,
                                 .src_value      = uint8_t{0x55},
                                 .expected_value = uint8_t{0x55},
                                 .name           = "SameValue",
                             },
                             InstrParam{
                                 .opcode         = Opcode::OR_A_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = VRAMStart + 3,
                                 .initial_a      = 0x5A,
                                 .src_value      = uint8_t{0xFF},
                                 .expected_value = uint8_t{0xFF},
                                 .name           = "AllBits",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// OR A, n8
INSTANTIATE_TEST_SUITE_P(OrInstructions,
                         ORImmTest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::OR_A_N8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xF0,
                                 .src_value      = uint8_t{0x0A},
                                 .expected_value = uint8_t{0xFA},
                                 .name           = "Normal",
                             },
                             InstrParam{
                                 .opcode         = Opcode::OR_A_N8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x00,
                                 .src_value      = uint8_t{0x00},
                                 .expected_value = uint8_t{0x00},
                                 .expect_z       = true,
                                 .name           = "Zero",
                             },
                             InstrParam{
                                 .opcode         = Opcode::OR_A_N8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x55,
                                 .src_value      = uint8_t{0x55},
                                 .expected_value = uint8_t{0x55},
                                 .name           = "SameValue",
                             },
                             InstrParam{
                                 .opcode         = Opcode::OR_A_N8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x5A,
                                 .src_value      = uint8_t{0xFF},
                                 .expected_value = uint8_t{0xFF},
                                 .name           = "AllBits",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// XOR A, r8
INSTANTIATE_TEST_SUITE_P(XorInstructions,
                         XORR8Test,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::XOR_A_A,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg8Name::A,
                                 .src_value      = uint8_t{0xFF},
                                 .expected_value = uint8_t{0x00},
                                 .expect_z       = true,
                                 .name           = "A_A_Zero",
                             },
                             InstrParam{
                                 .opcode         = Opcode::XOR_A_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xFF,
                                 .src_value      = uint8_t{0x0F},
                                 .expected_value = uint8_t{0xF0},
                                 .name           = "A_B_Normal",
                             },
                             InstrParam{
                                 .opcode         = Opcode::XOR_A_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x0F,
                                 .src_value      = uint8_t{0x0F},
                                 .expected_value = uint8_t{0x00},
                                 .expect_z       = true,
                                 .name           = "A_C_Zero",
                             },
                             InstrParam{
                                 .opcode         = Opcode::XOR_A_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xF0,
                                 .src_value      = uint8_t{0xFF},
                                 .expected_value = uint8_t{0x0F},
                                 .name           = "A_D_Normal",
                             },
                             InstrParam{
                                 .opcode         = Opcode::XOR_A_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xFF,
                                 .src_value      = uint8_t{0xFF},
                                 .expected_value = uint8_t{0x00},
                                 .expect_z       = true,
                                 .name           = "A_E_Zero",
                             },
                             InstrParam{
                                 .opcode         = Opcode::XOR_A_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = uint8_t{0x55},
                                 .expected_value = uint8_t{0xFF},
                                 .name           = "A_H_Normal",
                             },
                             InstrParam{
                                 .opcode         = Opcode::XOR_A_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xFF,
                                 .src_value      = uint8_t{0x00},
                                 .expected_value = uint8_t{0xFF},
                                 .name           = "A_L_Normal",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// XOR A, [HL]
INSTANTIATE_TEST_SUITE_P(XorInstructions,
                         XORHLTest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::XOR_A_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = VRAMStart,
                                 .initial_a      = 0x00,
                                 .src_value      = uint8_t{0x00},
                                 .expected_value = uint8_t{0x00},
                                 .expect_z       = true,
                                 .name           = "Zero",
                             },
                             InstrParam{
                                 .opcode         = Opcode::XOR_A_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = VRAMStart + 1,
                                 .initial_a      = 0x55,
                                 .src_value      = uint8_t{0x55},
                                 .expected_value = uint8_t{0x00},
                                 .expect_z       = true,
                                 .name           = "ZeroSameBits",
                             },
                             InstrParam{
                                 .opcode         = Opcode::XOR_A_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = VRAMStart + 2,
                                 .initial_a      = 0xAA,
                                 .src_value      = uint8_t{0x55},
                                 .expected_value = uint8_t{0xFF},
                                 .name           = "DifferentBits",
                             },
                             InstrParam{
                                 .opcode         = Opcode::XOR_A_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = VRAMStart + 3,
                                 .initial_a      = 0x5A,
                                 .src_value      = uint8_t{0xFF},
                                 .expected_value = uint8_t{0xA5},
                                 .name           = "AllBits",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// XOR A, n8
INSTANTIATE_TEST_SUITE_P(XorInstructions,
                         XORImmTest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::XOR_A_N8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x00,
                                 .src_value      = uint8_t{0x00},
                                 .expected_value = uint8_t{0x00},
                                 .expect_z       = true,
                                 .name           = "Zero",
                             },
                             InstrParam{
                                 .opcode         = Opcode::XOR_A_N8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x55,
                                 .src_value      = uint8_t{0x55},
                                 .expected_value = uint8_t{0x00},
                                 .expect_z       = true,
                                 .name           = "ZeroSameBits",
                             },
                             InstrParam{
                                 .opcode         = Opcode::XOR_A_N8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0xAA,
                                 .src_value      = uint8_t{0x55},
                                 .expected_value = uint8_t{0xFF},
                                 .name           = "DifferentBits",
                             },
                             InstrParam{
                                 .opcode         = Opcode::XOR_A_N8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x5A,
                                 .src_value      = uint8_t{0xFF},
                                 .expected_value = uint8_t{0xA5},
                                 .name           = "AllBits",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// CP A, r8
INSTANTIATE_TEST_SUITE_P(CpInstructions,
                         CPR8Test,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::CP_A_A,
                                 .src            = Reg8Name::A,
                                 .src_value      = uint8_t{0xFF},
                                 .expected_value = uint8_t{0xFF}, // A is unchanged
                                 .expect_z       = true,
                                 .expect_n       = true,
                                 .name           = "A_A_Normal",
                             },
                             InstrParam{
                                 .opcode         = Opcode::CP_A_B,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x0F,
                                 .src_value      = uint8_t{0x0F},
                                 .expected_value = uint8_t{0x0F}, // A is unchanged
                                 .expect_z       = true,
                                 .expect_n       = true,
                                 .name           = "A_B_Zero",
                             },
                             InstrParam{
                                 .opcode         = Opcode::CP_A_C,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x10,
                                 .src_value      = uint8_t{0x01},
                                 .expected_value = uint8_t{0x10}, // A is unchanged
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .name           = "A_C_HalfCarry",
                             },
                             InstrParam{
                                 .opcode         = Opcode::CP_A_D,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x00,
                                 .src_value      = uint8_t{0x01},
                                 .expected_value = uint8_t{0x00}, // A is unchanged
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .expect_c       = true,
                                 .name           = "A_D_Underflow",
                             },
                             InstrParam{
                                 .opcode         = Opcode::CP_A_E,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x80,
                                 .src_value      = uint8_t{0x70},
                                 .expected_value = uint8_t{0x80}, // A is unchanged
                                 .expect_n       = true,
                                 .name           = "A_E_Normal",
                             },
                             InstrParam{
                                 .opcode         = Opcode::CP_A_H,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x7F,
                                 .src_value      = uint8_t{0x80},
                                 .expected_value = uint8_t{0x7F}, // A is unchanged
                                 .expect_n       = true,
                                 .expect_c       = true,
                                 .name           = "A_H_Underflow",
                             },
                             InstrParam{
                                 .opcode         = Opcode::CP_A_L,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x01,
                                 .src_value      = uint8_t{0x01},
                                 .expected_value = uint8_t{0x01}, // A is unchanged
                                 .expect_z       = true,
                                 .expect_n       = true,
                                 .name           = "A_L_Zero",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// CP A, [HL]
INSTANTIATE_TEST_SUITE_P(CpInstructions,
                         CPHLTest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::CP_A_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = VRAMStart,
                                 .initial_a      = 0x80,
                                 .src_value      = uint8_t{0x70},
                                 .expected_value = uint8_t{0x80}, // A is unchanged
                                 .expect_n       = true,
                                 .name           = "Normal",
                             },
                             InstrParam{
                                 .opcode         = Opcode::CP_A_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = VRAMStart + 1,
                                 .initial_a      = 0x55,
                                 .src_value      = uint8_t{0x55},
                                 .expected_value = uint8_t{0x55}, // A is unchanged
                                 .expect_z       = true,
                                 .expect_n       = true,
                                 .name           = "Zero",
                             },
                             InstrParam{
                                 .opcode         = Opcode::CP_A_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = VRAMStart + 2,
                                 .initial_a      = 0x10,
                                 .src_value      = uint8_t{0x20},
                                 .expected_value = uint8_t{0x10}, // A is unchanged
                                 .expect_n       = true,
                                 .expect_c       = true,
                                 .name           = "Underflow",
                             },
                             InstrParam{
                                 .opcode         = Opcode::CP_A_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = VRAMStart + 3,
                                 .initial_a      = 0x10,
                                 .src_value      = uint8_t{0x01},
                                 .expected_value = uint8_t{0x10}, // A is unchanged
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .name           = "HalfCarry",
                             },
                             InstrParam{
                                 .opcode         = Opcode::CP_A_AT_HL,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = VRAMStart + 4,
                                 .initial_a      = 0x00,
                                 .src_value      = uint8_t{0x01},
                                 .expected_value = uint8_t{0x00}, // A is unchanged
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .expect_c       = true,
                                 .name           = "UnderflowHalfCarry",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// CP A, n8
INSTANTIATE_TEST_SUITE_P(CpInstructions,
                         CPImmTest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::CP_A_N8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x80,
                                 .src_value      = uint8_t{0x70},
                                 .expected_value = uint8_t{0x80}, // A is unchanged
                                 .expect_n       = true,
                                 .name           = "Normal",
                             },
                             InstrParam{
                                 .opcode         = Opcode::CP_A_N8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x55,
                                 .src_value      = uint8_t{0x55},
                                 .expected_value = uint8_t{0x55}, // A is unchanged
                                 .expect_z       = true,
                                 .expect_n       = true,
                                 .name           = "Zero",
                             },
                             InstrParam{
                                 .opcode         = Opcode::CP_A_N8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x10,
                                 .src_value      = uint8_t{0x20},
                                 .expected_value = uint8_t{0x10}, // A is unchanged
                                 .expect_n       = true,
                                 .expect_c       = true,
                                 .name           = "Underflow",
                             },
                             InstrParam{
                                 .opcode         = Opcode::CP_A_N8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x10,
                                 .src_value      = uint8_t{0x01},
                                 .expected_value = uint8_t{0x10}, // A is unchanged
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .name           = "HalfCarry",
                             },
                             InstrParam{
                                 .opcode         = Opcode::CP_A_N8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg8Name::A,
                                 .initial_a      = 0x00,
                                 .src_value      = uint8_t{0x01},
                                 .expected_value = uint8_t{0x00}, // A is unchanged
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .expect_c       = true,
                                 .name           = "UnderflowHalfCarry",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);