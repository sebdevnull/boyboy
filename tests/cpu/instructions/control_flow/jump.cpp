/**
 * @file jump.cpp
 * @brief Unit tests for jump CPU instructions.
 *
 * JP n16
 * JP HL
 * JP cc, n16
 * JR e8
 * JR cc, e8
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
using boyboy::mmu::WRAM0Start;

using boyboy::test::cpu::FlagsParam;
using boyboy::test::cpu::InstrParam;
using boyboy::test::cpu::InstrTest;
using boyboy::test::cpu::OperandType;

// -----------------------------
// Test types
// -----------------------------
using JpN16Test = InstrTest<InstrParam>;
using JpHLTest  = InstrTest<InstrParam>;
using JpE8Test  = InstrTest<InstrParam>;

// -----------------------------
// Test definitions
// -----------------------------
TEST_P(JpN16Test, Works) { run_test(); }
TEST_P(JpHLTest, Works) { run_test(); }
TEST_P(JpE8Test, Works) { run_test(); }

// -----------------------------
// Parameter instantiations
// -----------------------------
// JP n16
INSTANTIATE_TEST_SUITE_P(
    JpInstructions,
    JpN16Test,
    ::testing::Values(
        InstrParam{
            .opcode         = Opcode::JP_A16,
            .src_op_type    = OperandType::Immediate,
            .dst            = Reg16Name::PC,
            .src_value      = uint16_t{0x1234},
            .expected_value = uint16_t{0x1234},
            .name           = "JP_A16",
        },
        InstrParam{
            .opcode         = Opcode::JP_Z_A16,
            .src_op_type    = OperandType::Immediate,
            .dst            = Reg16Name::PC,
            .initial_pc     = uint16_t{WRAM0Start},
            .initial_flags  = FlagsParam{.z = true},
            .src_value      = uint16_t{0x1234},
            .expected_value = uint16_t{0x1234},
            .expect_z       = true,
            .name           = "JP_Z_A16_Taken",
        },
        InstrParam{
            .opcode         = Opcode::JP_Z_A16,
            .src_op_type    = OperandType::Immediate,
            .dst            = Reg16Name::PC,
            .initial_pc     = uint16_t{WRAM0Start},
            .initial_flags  = FlagsParam{.z = false},
            .src_value      = uint16_t{0x1234},
            .expected_value = uint16_t{WRAM0Start + 3}, // Not taken, should just advance PC
            .expect_z       = false,
            .name           = "JP_Z_A16_NotTaken",
        },
        InstrParam{
            .opcode         = Opcode::JP_NZ_A16,
            .src_op_type    = OperandType::Immediate,
            .dst            = Reg16Name::PC,
            .initial_pc     = uint16_t{WRAM0Start},
            .initial_flags  = FlagsParam{.z = false},
            .src_value      = uint16_t{0x1234},
            .expected_value = uint16_t{0x1234},
            .expect_z       = false,
            .name           = "JP_NZ_A16_Taken",
        },
        InstrParam{
            .opcode         = Opcode::JP_NZ_A16,
            .src_op_type    = OperandType::Immediate,
            .dst            = Reg16Name::PC,
            .initial_pc     = uint16_t{WRAM0Start},
            .initial_flags  = FlagsParam{.z = true},
            .src_value      = uint16_t{0x1234},
            .expected_value = uint16_t{WRAM0Start + 3}, // Not taken, should just advance PC
            .expect_z       = true,
            .name           = "JP_NZ_A16_NotTaken",
        },
        InstrParam{
            .opcode         = Opcode::JP_C_A16,
            .src_op_type    = OperandType::Immediate,
            .dst            = Reg16Name::PC,
            .initial_pc     = uint16_t{WRAM0Start},
            .initial_flags  = FlagsParam{.c = true},
            .src_value      = uint16_t{0x1234},
            .expected_value = uint16_t{0x1234},
            .expect_c       = true,
            .name           = "JP_C_A16_Taken",
        },
        InstrParam{
            .opcode         = Opcode::JP_C_A16,
            .src_op_type    = OperandType::Immediate,
            .dst            = Reg16Name::PC,
            .initial_pc     = uint16_t{WRAM0Start},
            .initial_flags  = FlagsParam{.c = false},
            .src_value      = uint16_t{0x1234},
            .expected_value = uint16_t{WRAM0Start + 3}, // Not taken, should just advance PC
            .expect_c       = false,
            .name           = "JP_C_A16_NotTaken",
        },
        InstrParam{
            .opcode         = Opcode::JP_NC_A16,
            .src_op_type    = OperandType::Immediate,
            .dst            = Reg16Name::PC,
            .initial_pc     = uint16_t{WRAM0Start},
            .initial_flags  = FlagsParam{.c = false},
            .src_value      = uint16_t{0x1234},
            .expected_value = uint16_t{0x1234},
            .expect_c       = false,
            .name           = "JP_NC_A16_Taken",
        },
        InstrParam{
            .opcode         = Opcode::JP_NC_A16,
            .src_op_type    = OperandType::Immediate,
            .dst            = Reg16Name::PC,
            .initial_pc     = uint16_t{WRAM0Start},
            .initial_flags  = FlagsParam{.c = true},
            .src_value      = uint16_t{0x1234},
            .expected_value = uint16_t{WRAM0Start + 3}, // Not taken, should just advance PC
            .expect_c       = true,
            .name           = "JP_NC_A16_NotTaken",
        }),
    boyboy::test::cpu::param_name<InstrParam>);

// JP HL
INSTANTIATE_TEST_SUITE_P(JpInstructions,
                         JpHLTest,
                         ::testing::Values(InstrParam{
                             .opcode         = Opcode::JP_HL,
                             .src_op_type    = OperandType::Register,
                             .src            = Reg16Name::HL,
                             .dst            = Reg16Name::PC,
                             .src_value      = uint16_t{0x1234},
                             .expected_value = uint16_t{0x1234},
                             .name           = "JP_HL",
                         }),
                         boyboy::test::cpu::param_name<InstrParam>);

// JR e8
INSTANTIATE_TEST_SUITE_P(
    JrInstructions,
    JpE8Test,
    ::testing::Values(
        InstrParam{
            .opcode         = Opcode::JR_E8,
            .src_op_type    = OperandType::Immediate,
            .dst            = Reg16Name::PC,
            .initial_pc     = uint16_t{WRAM0Start},
            .src_value      = uint8_t{0x7F}, // +127
            .expected_value = uint16_t{WRAM0Start + 2 + 0x7F},
            .name           = "JR_E8_Positive",
        },
        InstrParam{
            .opcode         = Opcode::JR_E8,
            .src_op_type    = OperandType::Immediate,
            .dst            = Reg16Name::PC,
            .initial_pc     = uint16_t{WRAM0Start + 0x80},
            .src_value      = uint8_t{0x80}, // -128
            .expected_value = uint16_t{WRAM0Start + 2},
            .name           = "JR_E8_Negative",
        },
        InstrParam{
            .opcode         = Opcode::JR_Z_E8,
            .src_op_type    = OperandType::Immediate,
            .dst            = Reg16Name::PC,
            .initial_pc     = uint16_t{WRAM0Start},
            .initial_flags  = FlagsParam{.z = true},
            .src_value      = uint8_t{0x7F}, // +127
            .expected_value = uint16_t{WRAM0Start + 2 + 0x7F},
            .expect_z       = true,
            .name           = "JR_Z_E8_Taken",
        },
        InstrParam{
            .opcode         = Opcode::JR_Z_E8,
            .src_op_type    = OperandType::Immediate,
            .dst            = Reg16Name::PC,
            .initial_pc     = uint16_t{WRAM0Start},
            .initial_flags  = FlagsParam{.z = false},
            .src_value      = uint8_t{0x7F},            // +127
            .expected_value = uint16_t{WRAM0Start + 2}, // Not taken, should just advance PC
            .expect_z       = false,
            .name           = "JR_Z_E8_NotTaken",
        },
        InstrParam{
            .opcode         = Opcode::JR_NZ_E8,
            .src_op_type    = OperandType::Immediate,
            .dst            = Reg16Name::PC,
            .initial_pc     = uint16_t{WRAM0Start},
            .initial_flags  = FlagsParam{.z = false},
            .src_value      = uint8_t{0x7F}, // +127
            .expected_value = uint16_t{WRAM0Start + 2 + 0x7F},
            .expect_z       = false,
            .name           = "JR_NZ_E8_Taken",
        },
        InstrParam{
            .opcode         = Opcode::JR_NZ_E8,
            .src_op_type    = OperandType::Immediate,
            .dst            = Reg16Name::PC,
            .initial_pc     = uint16_t{WRAM0Start},
            .initial_flags  = FlagsParam{.z = true},
            .src_value      = uint8_t{0x7F},            // +127
            .expected_value = uint16_t{WRAM0Start + 2}, // Not taken, should just advance PC
            .expect_z       = true,
            .name           = "JR_NZ_E8_NotTaken",
        },
        InstrParam{
            .opcode         = Opcode::JR_C_E8,
            .src_op_type    = OperandType::Immediate,
            .dst            = Reg16Name::PC,
            .initial_pc     = uint16_t{WRAM0Start},
            .initial_flags  = FlagsParam{.c = true},
            .src_value      = uint8_t{0x7F}, // +127
            .expected_value = uint16_t{WRAM0Start + 2 + 0x7F},
            .expect_c       = true,
            .name           = "JR_C_E8_Taken",
        },
        InstrParam{
            .opcode         = Opcode::JR_C_E8,
            .src_op_type    = OperandType::Immediate,
            .dst            = Reg16Name::PC,
            .initial_pc     = uint16_t{WRAM0Start},
            .initial_flags  = FlagsParam{.c = false},
            .src_value      = uint8_t{0x7F},            // +127
            .expected_value = uint16_t{WRAM0Start + 2}, // Not taken, should just advance PC
            .expect_c       = false,
            .name           = "JR_C_E8_NotTaken",
        },
        InstrParam{
            .opcode         = Opcode::JR_NC_E8,
            .src_op_type    = OperandType::Immediate,
            .dst            = Reg16Name::PC,
            .initial_pc     = uint16_t{WRAM0Start},
            .initial_flags  = FlagsParam{.c = false},
            .src_value      = uint8_t{0x7F}, // +127
            .expected_value = uint16_t{WRAM0Start + 2 + 0x7F},
            .expect_c       = false,
            .name           = "JR_NC_E8_Taken",
        },
        InstrParam{
            .opcode         = Opcode::JR_NC_E8,
            .src_op_type    = OperandType::Immediate,
            .dst            = Reg16Name::PC,
            .initial_pc     = uint16_t{WRAM0Start},
            .initial_flags  = FlagsParam{.c = true},
            .src_value      = uint8_t{0x7F},            // +127
            .expected_value = uint16_t{WRAM0Start + 2}, // Not taken, should just advance PC
            .expect_c       = true,
            .name           = "JR_NC_E8_NotTaken",
        }),
    boyboy::test::cpu::param_name<InstrParam>);