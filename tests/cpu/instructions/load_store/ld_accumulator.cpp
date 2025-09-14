/**
 * @file ld_accumulator.cpp
 * @brief Unit tests for load to/from accumulator instructions.
 *
 * LD A, [BC/DE/HL+/-] and LD [BC/DE/HL+/-], A
 * LD A, [n16] and LD [n16], A
 * LDH A, [C/n8] and LDH [C/n8], A
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

using boyboy::test::cpu::expect_hl_dec;
using boyboy::test::cpu::expect_hl_inc;
using boyboy::test::cpu::InstrParam;
using boyboy::test::cpu::InstrTest;
using boyboy::test::cpu::OperandType;

// -----------------------------
// Test types
// -----------------------------
using LdToAccTest     = InstrTest<InstrParam>;
using LdFromAccTest   = InstrTest<InstrParam>;
using LdAMemTest      = InstrTest<InstrParam>;
using LdMemATest      = InstrTest<InstrParam>;
using LdAHLIncDecTest = InstrTest<InstrParam>;
using LdHLIncADecTest = InstrTest<InstrParam>;
using LdhAMemTest     = InstrTest<InstrParam>;
using LdhMemATest     = InstrTest<InstrParam>;

// -----------------------------
// Test definitions
// -----------------------------
TEST_P(LdToAccTest, Works) { run_test(); }
TEST_P(LdFromAccTest, Works) { run_test(); }
TEST_P(LdAMemTest, Works) { run_test(); }
TEST_P(LdMemATest, Works) { run_test(); }
TEST_P(LdAHLIncDecTest, Works) { run_test(); }
TEST_P(LdHLIncADecTest, Works) { run_test(); }
TEST_P(LdhAMemTest, Works) { run_test(); }
TEST_P(LdhMemATest, Works) { run_test(); }

// -----------------------------
// Parameter instantiations
// -----------------------------
// LD A, [BC/DE]
INSTANTIATE_TEST_SUITE_P(LdInstructions,
                         LdToAccTest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::LD_A_AT_BC,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::BC,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = 0xABC0,
                                 .src_value      = uint8_t{0xAA},
                                 .expected_value = uint8_t{0xAA},
                                 .name           = "LD_A_AT_BC",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_A_AT_DE,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::DE,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = 0xADE0,
                                 .src_value      = uint8_t{0x55},
                                 .expected_value = uint8_t{0x55},
                                 .name           = "LD_A_AT_DE",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// LD [BC/DE], A
INSTANTIATE_TEST_SUITE_P(LdInstructions,
                         LdFromAccTest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::LD_AT_BC_A,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg16Name::BC,
                                 .dst_addr       = 0x0BCA,
                                 .src_value      = uint8_t{0xAA},
                                 .expected_value = uint8_t{0xAA},
                                 .name           = "LD_AT_BC_A",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_AT_DE_A,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg16Name::DE,
                                 .dst_addr       = 0x0DEA,
                                 .src_value      = uint8_t{0x55},
                                 .expected_value = uint8_t{0x55},
                                 .name           = "LD_AT_DE_A",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// LD a, [n16]
INSTANTIATE_TEST_SUITE_P(LdInstructions,
                         LdAMemTest,
                         ::testing::Values(InstrParam{
                             .opcode         = Opcode::LD_A_AT_A16,
                             .src_op_type    = OperandType::Memory,
                             .dst            = Reg8Name::A,
                             .src_addr       = 0xABC0,
                             .src_value      = uint8_t{0xAA},
                             .expected_value = uint8_t{0xAA},
                             .name           = "LD_A_AT_A16",
                         }),
                         boyboy::test::cpu::param_name<InstrParam>);

// LD [n16], A
INSTANTIATE_TEST_SUITE_P(LdInstructions,
                         LdMemATest,
                         ::testing::Values(InstrParam{
                             .opcode         = Opcode::LD_AT_A16_A,
                             .dst_op_type    = OperandType::Memory,
                             .src            = Reg8Name::A,
                             .dst_addr       = 0x0ABC,
                             .src_value      = uint8_t{0x55},
                             .expected_value = uint8_t{0x55},
                             .name           = "LD_AT_A16_A",
                         }),
                         boyboy::test::cpu::param_name<InstrParam>);

// LD A, [HL+/-]
INSTANTIATE_TEST_SUITE_P(LdInstructions,
                         LdAHLIncDecTest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::LD_A_AT_HL_INC,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = 0xABC0,
                                 .src_value      = uint8_t{0xAA},
                                 .expected_value = uint8_t{0xAA},
                                 .name           = "LD_A_AT_HL_INC",
                                 .validators     = {expect_hl_inc},
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_A_AT_HL_DEC,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::HL,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = 0x0ABC,
                                 .src_value      = uint8_t{0x55},
                                 .expected_value = uint8_t{0x55},
                                 .name           = "LD_A_AT_HL_DEC",
                                 .validators     = {expect_hl_dec},
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// LD [HL+/-], A
INSTANTIATE_TEST_SUITE_P(LdInstructions,
                         LdHLIncADecTest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::LD_AT_HL_INC_A,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg16Name::HL,
                                 .dst_addr       = 0xABC0,
                                 .src_value      = uint8_t{0xAA},
                                 .expected_value = uint8_t{0xAA},
                                 .name           = "LD_AT_HL_INC_A",
                                 .validators     = {expect_hl_inc},
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_AT_HL_DEC_A,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg16Name::HL,
                                 .dst_addr       = 0x0ABC,
                                 .src_value      = uint8_t{0x55},
                                 .expected_value = uint8_t{0x55},
                                 .name           = "LD_AT_HL_DEC_A",
                                 .validators     = {expect_hl_dec},
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// LDH A, [C/n8]
INSTANTIATE_TEST_SUITE_P(LdInstructions,
                         LdhAMemTest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::LDH_A_AT_C,
                                 .src_op_type    = OperandType::HighRAM,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = 0xFFAB,
                                 .src_value      = uint8_t{0xAA},
                                 .expected_value = uint8_t{0xAA},
                                 .name           = "LDH_A_AT_C",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LDH_A_AT_A8,
                                 .src_op_type    = OperandType::HighRAM,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = 0xFFBA,
                                 .src_value      = uint8_t{0x55},
                                 .expected_value = uint8_t{0x55},
                                 .name           = "LDH_A_AT_A8",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// LDH [C/n8], A
INSTANTIATE_TEST_SUITE_P(LdInstructions,
                         LdhMemATest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::LDH_AT_C_A,
                                 .dst_op_type    = OperandType::HighRAM,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg8Name::C,
                                 .dst_addr       = 0xFFAB,
                                 .src_value      = uint8_t{0xAA},
                                 .expected_value = uint8_t{0xAA},
                                 .name           = "LDH_AT_C_A",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LDH_AT_A8_A,
                                 .dst_op_type    = OperandType::HighRAM,
                                 .src            = Reg8Name::A,
                                 .dst_addr       = 0xFFBA,
                                 .src_value      = uint8_t{0x55},
                                 .expected_value = uint8_t{0x55},
                                 .name           = "LDH_AT_A8_A",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);