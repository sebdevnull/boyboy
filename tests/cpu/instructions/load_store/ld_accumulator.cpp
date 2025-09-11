/**
 * @file ld_accumulator.cpp
 * @brief Unit tests for load to/from accumulator instructions.
 *
 * LD A, [BC/DE/HL+/-] and LD [BC/DE/HL+/-], A
 * LD A, [n16] and LD [n16], A
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

// Helpers
#include "helpers/cpu_fixtures.h"
#include "helpers/cpu_params.h"

using namespace boyboy::cpu;

using boyboy::test::cpu::OperandType;
using boyboy::test::cpu::R8Param;
using boyboy::test::cpu::R8Test;

// -----------------------------
// Test types
// -----------------------------
using LdToAccTest   = R8Test<R8Param>;
using LdFromAccTest = R8Test<R8Param>;
using LdAMemTest    = R8Test<R8Param>;
using LdMemATest    = R8Test<R8Param>;

// -----------------------------
// Test definitions
// -----------------------------
TEST_P(LdToAccTest, Works) { run_test(); }
TEST_P(LdFromAccTest, Works) { run_test(); }
TEST_P(LdAMemTest, Works) { run_test(); }
TEST_P(LdMemATest, Works) { run_test(); }

// -----------------------------
// Parameter instantiations
// -----------------------------
// LD A, [BC/DE]
INSTANTIATE_TEST_SUITE_P(LdInstructions,
                         LdToAccTest,
                         ::testing::Values(
                             R8Param{
                                 .opcode         = Opcode::LD_A_AT_BC,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::BC,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = 0xABC0,
                                 .src_value      = 0xAA,
                                 .expected_value = 0xAA,
                                 .name           = "LD_A_AT_BC",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_A_AT_DE,
                                 .src_op_type    = OperandType::Indirect,
                                 .src            = Reg16Name::DE,
                                 .dst            = Reg8Name::A,
                                 .src_addr       = 0xADE0,
                                 .src_value      = 0x55,
                                 .expected_value = 0x55,
                                 .name           = "LD_A_AT_DE",
                             }),
                         boyboy::test::cpu::param_name<R8Param>);

// LD [BC/DE], A
INSTANTIATE_TEST_SUITE_P(LdInstructions,
                         LdFromAccTest,
                         ::testing::Values(
                             R8Param{
                                 .opcode         = Opcode::LD_AT_BC_A,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg16Name::BC,
                                 .dst_addr       = 0x0BCA,
                                 .src_value      = 0xAA,
                                 .expected_value = 0xAA,
                                 .name           = "LD_AT_BC_A",
                             },
                             R8Param{
                                 .opcode         = Opcode::LD_AT_DE_A,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg16Name::DE,
                                 .dst_addr       = 0x0DEA,
                                 .src_value      = 0x55,
                                 .expected_value = 0x55,
                                 .name           = "LD_AT_DE_A",
                             }),
                         boyboy::test::cpu::param_name<R8Param>);

// LD a, [n16]
INSTANTIATE_TEST_SUITE_P(LdInstructions,
                         LdAMemTest,
                         ::testing::Values(R8Param{
                             .opcode         = Opcode::LD_A_AT_A16,
                             .src_op_type    = OperandType::Memory,
                             .dst            = Reg8Name::A,
                             .src_addr       = 0xABC0,
                             .src_value      = 0xAA,
                             .expected_value = 0xAA,
                             .name           = "LD_A_AT_A16",
                         }),
                         boyboy::test::cpu::param_name<R8Param>);

// LD [n16], A
INSTANTIATE_TEST_SUITE_P(LdInstructions,
                         LdMemATest,
                         ::testing::Values(R8Param{
                             .opcode         = Opcode::LD_AT_A16_A,
                             .dst_op_type    = OperandType::Memory,
                             .src            = Reg8Name::A,
                             .dst_addr       = 0x0ABC,
                             .src_value      = 0x55,
                             .expected_value = 0x55,
                             .name           = "LD_AT_A16_A",
                         }),
                         boyboy::test::cpu::param_name<R8Param>);