/**
 * @file call_ret.cpp
 * @brief Unit tests for call and return CPU instructions.
 *
 * CALL a16
 * CALL cc, a16
 * RET
 * RET cc
 * RETI
 * RST n
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include <cstdint>

// boyboy
#include "boyboy/mmu_constants.h"

// Helpers
#include "helpers/cpu_asserts.h"
#include "helpers/cpu_fixtures.h"
#include "helpers/cpu_params.h"

using boyboy::cpu::Opcode;
using boyboy::cpu::Reg16Name;
using boyboy::mmu::VRAMStart;
using boyboy::mmu::WRAM0Start;

using boyboy::test::cpu::expect_call;
using boyboy::test::cpu::expect_pop;
using boyboy::test::cpu::FlagsParam;
using boyboy::test::cpu::InstrParam;
using boyboy::test::cpu::InstrTest;
using boyboy::test::cpu::OperandType;

// -----------------------------
// Test types
// -----------------------------
using CallTest = InstrTest<InstrParam>;
using RetTest  = InstrTest<InstrParam>;
using RstTest  = InstrTest<InstrParam>;

// -----------------------------
// Test definitions
// -----------------------------
TEST_P(CallTest, Works) { run_test(); }
TEST_P(RetTest, Works) { run_test(); }
TEST_P(RstTest, Works) { run_test(); }

// -----------------------------
// Parameter instantiations
// -----------------------------
// CALL
INSTANTIATE_TEST_SUITE_P(CallInstructions,
                         CallTest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::CALL_A16,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg16Name::PC,
                                 .src_addr       = uint16_t{VRAMStart},
                                 .initial_pc     = uint16_t{WRAM0Start},
                                 .initial_sp     = uint16_t{0xFFFE},
                                 .src_value      = uint16_t{0x1234},
                                 .stack_expect   = uint16_t{WRAM0Start + 3}, // initial PC + 3
                                 .expected_value = uint16_t{0x1234},
                                 .name           = "CALL_A16",
                                 .validators     = {expect_call},
                             },
                             InstrParam{
                                 .opcode         = Opcode::CALL_Z_A16,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg16Name::PC,
                                 .src_addr       = uint16_t{VRAMStart},
                                 .initial_pc     = uint16_t{WRAM0Start},
                                 .initial_sp     = uint16_t{0xFFFE},
                                 .initial_flags  = FlagsParam{.z = true},
                                 .src_value      = uint16_t{0x1234},
                                 .stack_expect   = uint16_t{WRAM0Start + 3}, // initial PC + 3
                                 .expected_value = uint16_t{0x1234},
                                 .expect_z       = true,
                                 .name           = "CALL_Z_A16_True",
                                 .validators     = {expect_call},
                             },
                             InstrParam{
                                 .opcode         = Opcode::CALL_Z_A16,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg16Name::PC,
                                 .src_addr       = uint16_t{VRAMStart},
                                 .initial_pc     = uint16_t{WRAM0Start},
                                 .initial_sp     = uint16_t{0xFFFE},
                                 .initial_flags  = FlagsParam{.z = false},
                                 .src_value      = uint16_t{0x1234},
                                 .expected_value = uint16_t{WRAM0Start + 3}, // PC should just advance
                                 .expect_z       = false,
                                 .name           = "CALL_Z_A16_False",
                             },
                             InstrParam{
                                 .opcode         = Opcode::CALL_NZ_A16,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg16Name::PC,
                                 .src_addr       = uint16_t{VRAMStart},
                                 .initial_pc     = uint16_t{WRAM0Start},
                                 .initial_sp     = uint16_t{0xFFFE},
                                 .initial_flags  = FlagsParam{.z = false},
                                 .src_value      = uint16_t{0x1234},
                                 .stack_expect   = uint16_t{WRAM0Start + 3}, // initial PC + 3
                                 .expected_value = uint16_t{0x1234},
                                 .expect_z       = false,
                                 .name           = "CALL_NZ_A16_True",
                                 .validators     = {expect_call},
                             },
                             InstrParam{
                                 .opcode         = Opcode::CALL_NZ_A16,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg16Name::PC,
                                 .src_addr       = uint16_t{VRAMStart},
                                 .initial_pc     = uint16_t{WRAM0Start},
                                 .initial_sp     = uint16_t{0xFFFE},
                                 .initial_flags  = FlagsParam{.z = true},
                                 .src_value      = uint16_t{0x1234},
                                 .expected_value = uint16_t{WRAM0Start + 3}, // PC should just advance
                                 .expect_z       = true,
                                 .name           = "CALL_NZ_A16_False",
                             },
                             InstrParam{
                                 .opcode         = Opcode::CALL_C_A16,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg16Name::PC,
                                 .src_addr       = uint16_t{VRAMStart},
                                 .initial_pc     = uint16_t{WRAM0Start},
                                 .initial_sp     = uint16_t{0xFFFE},
                                 .initial_flags  = FlagsParam{.c = true},
                                 .src_value      = uint16_t{0x1234},
                                 .stack_expect   = uint16_t{WRAM0Start + 3}, // initial PC + 3
                                 .expected_value = uint16_t{0x1234},
                                 .expect_c       = true,
                                 .name           = "CALL_C_A16_True",
                                 .validators     = {expect_call},
                             },
                             InstrParam{
                                 .opcode         = Opcode::CALL_C_A16,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg16Name::PC,
                                 .src_addr       = uint16_t{VRAMStart},
                                 .initial_pc     = uint16_t{WRAM0Start},
                                 .initial_sp     = uint16_t{0xFFFE},
                                 .initial_flags  = FlagsParam{.c = false},
                                 .src_value      = uint16_t{0x1234},
                                 .expected_value = uint16_t{WRAM0Start + 3}, // PC should just advance
                                 .expect_c       = false,
                                 .name           = "CALL_C_A16_False",
                             },
                             InstrParam{
                                 .opcode         = Opcode::CALL_NC_A16,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg16Name::PC,
                                 .src_addr       = uint16_t{VRAMStart},
                                 .initial_pc     = uint16_t{WRAM0Start},
                                 .initial_sp     = uint16_t{0xFFFE},
                                 .initial_flags  = FlagsParam{.c = false},
                                 .src_value      = uint16_t{0x1234},
                                 .stack_expect   = uint16_t{WRAM0Start + 3}, // initial PC + 3
                                 .expected_value = uint16_t{0x1234},
                                 .expect_c       = false,
                                 .name           = "CALL_NC_A16_True",
                                 .validators     = {expect_call},
                             },
                             InstrParam{
                                 .opcode         = Opcode::CALL_NC_A16,
                                 .src_op_type    = OperandType::Immediate,
                                 .dst            = Reg16Name::PC,
                                 .src_addr       = uint16_t{VRAMStart},
                                 .initial_pc     = uint16_t{WRAM0Start},
                                 .initial_sp     = uint16_t{0xFFFE},
                                 .initial_flags  = FlagsParam{.c = true},
                                 .src_value      = uint16_t{0x1234},
                                 .expected_value = uint16_t{WRAM0Start + 3}, // PC should just advance
                                 .expect_c       = true,
                                 .name           = "CALL_NC_A16_False",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// RET
INSTANTIATE_TEST_SUITE_P(
    RetInstructions,
    RetTest,
    ::testing::Values(
        InstrParam{
            .opcode         = Opcode::RET,
            .dst            = Reg16Name::PC,
            .src_addr       = uint16_t{VRAMStart},
            .initial_pc     = uint16_t{WRAM0Start},
            .initial_sp     = uint16_t{0xFFFC},
            .stack_init     = uint16_t{0x1234},
            .expected_value = uint16_t{0x1234},
            .name           = "RET",
            .validators     = {expect_pop},
        },
        InstrParam{
            .opcode         = Opcode::RET_Z,
            .dst            = Reg16Name::PC,
            .src_addr       = uint16_t{VRAMStart},
            .initial_pc     = uint16_t{WRAM0Start},
            .initial_sp     = uint16_t{0xFFFC},
            .initial_flags  = FlagsParam{.z = true},
            .stack_init     = uint16_t{0x1234},
            .expected_value = uint16_t{0x1234},
            .expect_z       = true,
            .name           = "RET_Z_True",
            .validators     = {expect_pop},
        },
        InstrParam{
            .opcode         = Opcode::RET_Z,
            .dst            = Reg16Name::PC,
            .src_addr       = uint16_t{VRAMStart},
            .initial_pc     = uint16_t{WRAM0Start},
            .initial_sp     = uint16_t{0xFFFC},
            .initial_flags  = FlagsParam{.z = false},
            .stack_init     = uint16_t{0x1234},
            .expected_value = uint16_t{WRAM0Start + 1}, // PC should just advance
            .expect_z       = false,
            .name           = "RET_Z_False",
        },
        InstrParam{
            .opcode         = Opcode::RET_NZ,
            .dst            = Reg16Name::PC,
            .src_addr       = uint16_t{VRAMStart},
            .initial_pc     = uint16_t{WRAM0Start},
            .initial_sp     = uint16_t{0xFFFC},
            .initial_flags  = FlagsParam{.z = false},
            .stack_init     = uint16_t{0x1234},
            .expected_value = uint16_t{0x1234},
            .expect_z       = false,
            .name           = "RET_NZ_True",
            .validators     = {expect_pop},
        },
        InstrParam{
            .opcode         = Opcode::RET_NZ,
            .dst            = Reg16Name::PC,
            .src_addr       = uint16_t{VRAMStart},
            .initial_pc     = uint16_t{WRAM0Start},
            .initial_sp     = uint16_t{0xFFFC},
            .initial_flags  = FlagsParam{.z = true},
            .stack_init     = uint16_t{0x1234},
            .expected_value = uint16_t{WRAM0Start + 1}, // PC should just advance
            .expect_z       = true,
            .name           = "RET_NZ_False",
        },
        InstrParam{
            .opcode         = Opcode::RET_C,
            .dst            = Reg16Name::PC,
            .src_addr       = uint16_t{VRAMStart},
            .initial_pc     = uint16_t{WRAM0Start},
            .initial_sp     = uint16_t{0xFFFC},
            .initial_flags  = FlagsParam{.c = true},
            .stack_init     = uint16_t{0x1234},
            .expected_value = uint16_t{0x1234},
            .expect_c       = true,
            .name           = "RET_C_True",
            .validators     = {expect_pop},
        },
        InstrParam{
            .opcode         = Opcode::RET_C,
            .dst            = Reg16Name::PC,
            .src_addr       = uint16_t{VRAMStart},
            .initial_pc     = uint16_t{WRAM0Start},
            .initial_sp     = uint16_t{0xFFFC},
            .initial_flags  = FlagsParam{.c = false},
            .stack_init     = uint16_t{0x1234},
            .expected_value = uint16_t{WRAM0Start + 1}, // PC should just advance
            .expect_c       = false,
            .name           = "RET_C_False",
        },
        InstrParam{
            .opcode         = Opcode::RET_NC,
            .dst            = Reg16Name::PC,
            .src_addr       = uint16_t{VRAMStart},
            .initial_pc     = uint16_t{WRAM0Start},
            .initial_sp     = uint16_t{0xFFFC},
            .initial_flags  = FlagsParam{.c = false},
            .stack_init     = uint16_t{0x1234},
            .expected_value = uint16_t{0x1234},
            .expect_c       = false,
            .name           = "RET_NC_True",
            .validators     = {expect_pop},
        },
        InstrParam{
            .opcode         = Opcode::RET_NC,
            .dst            = Reg16Name::PC,
            .src_addr       = uint16_t{VRAMStart},
            .initial_pc     = uint16_t{WRAM0Start},
            .initial_sp     = uint16_t{0xFFFC},
            .initial_flags  = FlagsParam{.c = true},
            .stack_init     = uint16_t{0x1234},
            .expected_value = uint16_t{WRAM0Start + 1}, // PC should just advance
            .expect_c       = true,
            .name           = "RET_NC_False",
        },
        InstrParam{
            .opcode         = Opcode::RETI,
            .dst            = Reg16Name::PC,
            .src_addr       = uint16_t{VRAMStart},
            .initial_pc     = uint16_t{WRAM0Start},
            .initial_sp     = uint16_t{0xFFFC},
            .initial_flags  = FlagsParam{.c = true},
            .stack_init     = uint16_t{0x1234},
            .expected_value = uint16_t{0x1234},
            .expect_c       = true,
            .name           = "RETI",
            .validators     = {expect_pop,
                               [](const boyboy::cpu::Cpu& cpu, const InstrParam& p) {
                               EXPECT_TRUE(cpu.get_ime())
                                   << "IME flag not set after RETI: " << p.name;
                           }},
        }),
    boyboy::test::cpu::param_name<InstrParam>);

// RST
INSTANTIATE_TEST_SUITE_P(RstInstructions,
                         RstTest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::RST_00,
                                 .dst            = Reg16Name::PC,
                                 .initial_pc     = uint16_t{WRAM0Start},
                                 .initial_sp     = uint16_t{0xFFFE},
                                 .stack_expect   = uint16_t{WRAM0Start + 1}, // initial PC + 1
                                 .expected_value = uint16_t{0x0000},
                                 .name           = "RST_00",
                                 .validators     = {expect_call},
                             },
                             InstrParam{
                                 .opcode         = Opcode::RST_08,
                                 .dst            = Reg16Name::PC,
                                 .initial_pc     = uint16_t{WRAM0Start},
                                 .initial_sp     = uint16_t{0xFFFE},
                                 .stack_expect   = uint16_t{WRAM0Start + 1}, // initial PC + 1
                                 .expected_value = uint16_t{0x0008},
                                 .name           = "RST_08",
                                 .validators     = {expect_call},
                             },
                             InstrParam{
                                 .opcode         = Opcode::RST_10,
                                 .dst            = Reg16Name::PC,
                                 .initial_pc     = uint16_t{WRAM0Start},
                                 .initial_sp     = uint16_t{0xFFFE},
                                 .stack_expect   = uint16_t{WRAM0Start + 1}, // initial PC + 1
                                 .expected_value = uint16_t{0x0010},
                                 .name           = "RST_10",
                                 .validators     = {expect_call},
                             },
                             InstrParam{
                                 .opcode         = Opcode::RST_18,
                                 .dst            = Reg16Name::PC,
                                 .initial_pc     = uint16_t{WRAM0Start},
                                 .initial_sp     = uint16_t{0xFFFE},
                                 .stack_expect   = uint16_t{WRAM0Start + 1}, // initial PC + 1
                                 .expected_value = uint16_t{0x0018},
                                 .name           = "RST_18",
                                 .validators     = {expect_call},
                             },
                             InstrParam{
                                 .opcode         = Opcode::RST_20,
                                 .dst            = Reg16Name::PC,
                                 .initial_pc     = uint16_t{WRAM0Start},
                                 .initial_sp     = uint16_t{0xFFFE},
                                 .stack_expect   = uint16_t{WRAM0Start + 1}, // initial PC + 1
                                 .expected_value = uint16_t{0x0020},
                                 .name           = "RST_20",
                                 .validators     = {expect_call},
                             },
                             InstrParam{
                                 .opcode         = Opcode::RST_28,
                                 .dst            = Reg16Name::PC,
                                 .initial_pc     = uint16_t{WRAM0Start},
                                 .initial_sp     = uint16_t{0xFFFE},
                                 .stack_expect   = uint16_t{WRAM0Start + 1}, // initial PC + 1
                                 .expected_value = uint16_t{0x0028},
                                 .name           = "RST_28",
                                 .validators     = {expect_call},
                             },
                             InstrParam{
                                 .opcode         = Opcode::RST_30,
                                 .dst            = Reg16Name::PC,
                                 .initial_pc     = uint16_t{WRAM0Start},
                                 .initial_sp     = uint16_t{0xFFFE},
                                 .stack_expect   = uint16_t{WRAM0Start + 1}, // initial PC + 1
                                 .expected_value = uint16_t{0x0030},
                                 .name           = "RST_30",
                                 .validators     = {expect_call},
                             },
                             InstrParam{
                                 .opcode         = Opcode::RST_38,
                                 .dst            = Reg16Name::PC,
                                 .initial_pc     = uint16_t{WRAM0Start},
                                 .initial_sp     = uint16_t{0xFFFE},
                                 .stack_expect   = uint16_t{WRAM0Start + 1}, // initial PC + 1
                                 .expected_value = uint16_t{0x0038},
                                 .name           = "RST_38",
                                 .validators     = {expect_call},
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);
