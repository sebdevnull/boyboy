/**
 * @file ld_misc.cpp
 * @brief Unit tests for miscellaneous LD CPU instructions.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include <cstdint>

// Helpers
#include "helpers/cpu_fixtures.h"
#include "helpers/cpu_params.h"

using boyboy::cpu::Opcode;
using boyboy::cpu::Reg16Name;

using boyboy::test::cpu::InstrParam;
using boyboy::test::cpu::InstrTest;
using boyboy::test::cpu::OperandType;

// -----------------------------
// Test types
// -----------------------------
using LdR16N16Test  = InstrTest<InstrParam>;
using LdAtN16SPTest = InstrTest<InstrParam>;
using LdHLSPeTest   = InstrTest<InstrParam>;

// -----------------------------
// Test definitions
// -----------------------------
TEST_P(LdR16N16Test, Works) { run_test(); }
TEST_P(LdAtN16SPTest, Works) { run_test(); }
TEST_P(LdHLSPeTest, Works) { run_test(); }

// -----------------------------
// Parameter instantiations
// -----------------------------
// LD r16, n16
INSTANTIATE_TEST_SUITE_P(LdInstructions,
                         LdR16N16Test,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::LD_BC_N16,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg16Name::BC,
                                 .src_value      = uint16_t{0xBEEF},
                                 .expected_value = uint16_t{0xBEEF},
                                 .name           = "LD_BC_N16",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_DE_N16,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg16Name::DE,
                                 .src_value      = uint16_t{0xDEAD},
                                 .expected_value = uint16_t{0xDEAD},
                                 .name           = "LD_DE_N16",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_HL_N16,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg16Name::HL,
                                 .src_value      = uint16_t{0xC0DE},
                                 .expected_value = uint16_t{0xC0DE},
                                 .name           = "LD_HL_N16",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_SP_N16,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg16Name::SP,
                                 .src_value      = uint16_t{0x1234},
                                 .expected_value = uint16_t{0x1234},
                                 .name           = "LD_SP_N16",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// LD [n16], SP
INSTANTIATE_TEST_SUITE_P(LdInstructions,
                         LdAtN16SPTest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::LD_AT_A16_SP,
                                 .dst_op_type    = OperandType::Memory,
                                 .src            = Reg16Name::SP,
                                 .dst_addr       = 0xC000,
                                 .src_value      = uint16_t{0x1234},
                                 .expected_value = uint16_t{0x1234},
                                 .name           = "LD_AT_N16_SP1",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_AT_A16_SP,
                                 .dst_op_type    = OperandType::Memory,
                                 .src            = Reg16Name::SP,
                                 .dst_addr       = 0xFFFE,
                                 .src_value      = uint16_t{0xABCD},
                                 .expected_value = uint16_t{0xABCD},
                                 .name           = "LD_AT_N16_SP2",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// LD HL, SP+e8
INSTANTIATE_TEST_SUITE_P(LdInstructions,
                         LdHLSPeTest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::LD_HL_SP_INC_E8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg16Name::HL,
                                 .initial_sp     = uint16_t{0xFFF8},
                                 .src_value      = uint8_t{0x08},
                                 .expected_value = uint16_t{0x0000},
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = true,
                                 .expect_c       = true,
                                 .name           = "LD_HL_SP_E8_PositiveOverflow",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_HL_SP_INC_E8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg16Name::HL,
                                 .initial_sp     = uint16_t{0x0008},
                                 .src_value      = uint8_t{0x08},
                                 .expected_value = uint16_t{0x0010},
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = true,
                                 .expect_c       = false,
                                 .name           = "LD_HL_SP_E8_Positive",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_HL_SP_INC_E8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg16Name::HL,
                                 .initial_sp     = uint16_t{0x0008},
                                 .src_value      = uint8_t{0xF8}, // -8
                                 .expected_value = uint16_t{0x0000},
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = true,
                                 .expect_c       = true,
                                 .name           = "LD_HL_SP_E8_NegativeUnderflow",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_HL_SP_INC_E8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg16Name::HL,
                                 .initial_sp     = uint16_t{0x0010},
                                 .src_value      = uint8_t{0xF8}, // -8
                                 .expected_value = uint16_t{0x0008},
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = true,
                                 .name           = "LD_HL_SP_E8_Negative",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);