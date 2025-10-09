/**
 * @file ld_hl_r.cpp
 * @brief Unit tests for LD [HL], r8 CPU instruction.
 *
 * LD [HL], r8
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
using boyboy::core::cpu::Reg8Name;
using boyboy::core::mmu::VRAMStart;

using boyboy::test::cpu::InstrParam;
using boyboy::test::cpu::InstrTest;
using boyboy::test::cpu::OperandType;

// -----------------------------
// Test types
// -----------------------------
using LdHLR8Test = InstrTest<InstrParam>;

// -----------------------------
// Test definitions
// -----------------------------
TEST_P(LdHLR8Test, Works) { run_test(); }

// -----------------------------
// Parameter instantiations
// -----------------------------
// LD [HL], r8
INSTANTIATE_TEST_SUITE_P(LdInstructions,
                         LdHLR8Test,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::LD_AT_HL_A,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg8Name::A,
                                 .dst            = Reg16Name::HL,
                                 .dst_addr       = VRAMStart,
                                 .src_value      = uint8_t{0xAA},
                                 .expected_value = uint8_t{0xAA},
                                 .name           = "LD_AT_HL_A",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_AT_HL_B,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg8Name::B,
                                 .dst            = Reg16Name::HL,
                                 .dst_addr       = VRAMStart + 1,
                                 .src_value      = uint8_t{0xBB},
                                 .expected_value = uint8_t{0xBB},
                                 .name           = "LD_AT_HL_B",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_AT_HL_C,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg8Name::C,
                                 .dst            = Reg16Name::HL,
                                 .dst_addr       = VRAMStart + 2,
                                 .src_value      = uint8_t{0xCC},
                                 .expected_value = uint8_t{0xCC},
                                 .name           = "LD_AT_HL_C",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_AT_HL_D,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg8Name::D,
                                 .dst            = Reg16Name::HL,
                                 .dst_addr       = VRAMStart + 3,
                                 .src_value      = uint8_t{0xDD},
                                 .expected_value = uint8_t{0xDD},
                                 .name           = "LD_AT_HL_D",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_AT_HL_E,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg8Name::E,
                                 .dst            = Reg16Name::HL,
                                 .dst_addr       = VRAMStart + 4,
                                 .src_value      = uint8_t{0xEE},
                                 .expected_value = uint8_t{0xEE},
                                 .name           = "LD_AT_HL_E",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_AT_HL_H,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg8Name::H,
                                 .dst            = Reg16Name::HL,
                                 .dst_addr       = VRAMStart + 5,
                                 .src_value      = uint8_t{0x80},
                                 .expected_value = uint8_t{0x80},
                                 .name           = "LD_AT_HL_H",
                             },
                             InstrParam{
                                 .opcode         = Opcode::LD_AT_HL_L,
                                 .dst_op_type    = OperandType::Indirect,
                                 .src            = Reg8Name::L,
                                 .dst            = Reg16Name::HL,
                                 .dst_addr       = VRAMStart + 6,
                                 .src_value      = uint8_t{0x06},
                                 .expected_value = uint8_t{0x06},
                                 .name           = "LD_AT_HL_L",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);