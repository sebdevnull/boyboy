/**
 * @file ld_hl_n.cpp
 * @brief Unit tests for LD [HL], n8 CPU instruction.
 *
 * LD [HL], n8
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

// boyboy
#include "boyboy/core/mmu/constants.h"

// Helpers
#include "helpers/cpu_fixtures.h"
#include "helpers/cpu_params.h"

using boyboy::core::cpu::Opcode;
using boyboy::core::cpu::Reg16Name;
using boyboy::core::mmu::VRAMStart;

using boyboy::test::cpu::InstrParam;
using boyboy::test::cpu::InstrTest;
using boyboy::test::cpu::OperandType;

// -----------------------------
// Test types
// -----------------------------
using LdHLN8Test = InstrTest<InstrParam>;

// -----------------------------
// Test definitions
// -----------------------------
TEST_P(LdHLN8Test, Works) { run_test(); }

// -----------------------------
// Parameter instantiations
// -----------------------------
// LD [HL], n8
INSTANTIATE_TEST_SUITE_P(LdInstructions,
                         LdHLN8Test,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::LD_AT_HL_N8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst_op_type    = OperandType::Indirect,
                                 .dst            = Reg16Name::HL,
                                 .dst_addr       = VRAMStart,
                                 .src_value      = uint8_t{0x00},
                                 .expected_value = uint8_t{0x00},
                                 .name           = "LD_AT_HL_N8_Zero",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_AT_HL_N8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst_op_type    = OperandType::Indirect,
                                 .dst            = Reg16Name::HL,
                                 .dst_addr       = VRAMStart + 1,
                                 .src_value      = uint8_t{0x7F},
                                 .expected_value = uint8_t{0x7F},
                                 .name           = "LD_AT_HL_N8_Positive",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_AT_HL_N8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst_op_type    = OperandType::Indirect,
                                 .dst            = Reg16Name::HL,
                                 .dst_addr       = VRAMStart + 2,
                                 .src_value      = uint8_t{0x80},
                                 .expected_value = uint8_t{0x80},
                                 .name           = "LD_AT_HL_N8_Negative",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_AT_HL_N8,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst_op_type    = OperandType::Indirect,
                                 .dst            = Reg16Name::HL,
                                 .dst_addr       = VRAMStart + 3,
                                 .src_value      = uint8_t{0xFF},
                                 .expected_value = uint8_t{0xFF},
                                 .name           = "LD_AT_HL_N8_Max",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);