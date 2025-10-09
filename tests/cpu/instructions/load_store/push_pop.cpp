/**
 * @file push_pop.cpp
 * @brief Unit tests for PUSH and POP CPU instructions.
 *
 * PUSH r16
 * POP r16
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include <cstdint>

// Helpers
#include "helpers/cpu_asserts.h"
#include "helpers/cpu_fixtures.h"
#include "helpers/cpu_params.h"

using boyboy::core::cpu::Opcode;
using boyboy::core::cpu::Reg16Name;

using boyboy::test::cpu::expect_pop;
using boyboy::test::cpu::expect_push;
using boyboy::test::cpu::InstrParam;
using boyboy::test::cpu::InstrTest;

// -----------------------------
// Test types
// -----------------------------
using PushR16Test = InstrTest<InstrParam>;
using PushAFTest  = InstrTest<InstrParam>;
using PopR16Test  = InstrTest<InstrParam>;
using PopAFTest   = InstrTest<InstrParam>;

// -----------------------------
// Test definitions
// -----------------------------
TEST_P(PushR16Test, Works) { run_test(); }
TEST_P(PushAFTest, Works) { run_test(); }
TEST_P(PopR16Test, Works) { run_test(); }
TEST_P(PopAFTest, Works) { run_test(); }

// -----------------------------
// Parameter instantiations
// -----------------------------
// PUSH r16
INSTANTIATE_TEST_SUITE_P(LdInstructions,
                         PushR16Test,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::PUSH_BC,
                                 .src            = Reg16Name::BC,
                                 .initial_sp     = uint16_t{0xFFFE},
                                 .src_value      = uint16_t{0x1234},
                                 .expected_value = uint16_t{0x1234},
                                 .name           = "PUSH_BC",
                                 .validators     = {expect_push},
                             },
                             InstrParam{
                                 .opcode         = Opcode::PUSH_DE,
                                 .src            = Reg16Name::DE,
                                 .initial_sp     = uint16_t{0xFFFE},
                                 .src_value      = uint16_t{0x5678},
                                 .expected_value = uint16_t{0x5678},
                                 .name           = "PUSH_DE",
                                 .validators     = {expect_push},
                             },
                             InstrParam{
                                 .opcode         = Opcode::PUSH_HL,
                                 .src            = Reg16Name::HL,
                                 .initial_sp     = uint16_t{0xFFFE},
                                 .src_value      = uint16_t{0x9ABC},
                                 .expected_value = uint16_t{0x9ABC},
                                 .name           = "PUSH_HL",
                                 .validators     = {expect_push},
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);
// PUSH AF
INSTANTIATE_TEST_SUITE_P(LdInstructions,
                         PushAFTest,
                         ::testing::Values(
                             InstrParam{
                                 // lsb 0x00 = {z=0, n=0, h=0, c=0}
                                 .opcode         = Opcode::PUSH_AF,
                                 .src            = Reg16Name::AF,
                                 .initial_sp     = uint16_t{0xFFFE},
                                 .src_value      = uint16_t{0x1200},
                                 .expected_value = uint16_t{0x1200},
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "PUSH_AF_Flags_Clear",
                                 .validators     = {expect_push},
                             },
                             InstrParam{
                                 // lsb 0x30 = {z=0, n=0, h=1, c=1}
                                 .opcode         = Opcode::PUSH_AF,
                                 .src            = Reg16Name::AF,
                                 .initial_sp     = uint16_t{0xFFFE},
                                 .src_value      = uint16_t{0x3430},
                                 .expected_value = uint16_t{0x3430},
                                 .expect_z       = false,
                                 .expect_n       = false,
                                 .expect_h       = true,
                                 .expect_c       = true,
                                 .name           = "PUSH_AF_Flags_Low",
                                 .validators     = {expect_push},
                             },
                             InstrParam{
                                 // lsb 0xC0 = {z=1, n=1, h=0, c=0}
                                 .opcode         = Opcode::PUSH_AF,
                                 .src            = Reg16Name::AF,
                                 .initial_sp     = uint16_t{0xFFFE},
                                 .src_value      = uint16_t{0x34C0},
                                 .expected_value = uint16_t{0x34C0},
                                 .expect_z       = true,
                                 .expect_n       = true,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "PUSH_AF_Flags_High",
                                 .validators     = {expect_push},
                             },
                             InstrParam{
                                 // lsb 0xF0 = {z=1, n=1, h=1, c=1}
                                 .opcode         = Opcode::PUSH_AF,
                                 .src            = Reg16Name::AF,
                                 .initial_sp     = uint16_t{0xFFFE},
                                 .src_value      = uint16_t{0xABF0},
                                 .expected_value = uint16_t{0xABF0},
                                 .expect_z       = true,
                                 .expect_n       = true,
                                 .expect_h       = true,
                                 .expect_c       = true,
                                 .name           = "PUSH_AF_Flags_Set",
                                 .validators     = {expect_push},
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);
// POP r16
INSTANTIATE_TEST_SUITE_P(LdInstructions,
                         PopR16Test,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::POP_BC,
                                 .dst            = Reg16Name::BC,
                                 .initial_sp     = uint16_t{0xFFFC},
                                 .stack_init     = uint16_t{0x1234},
                                 .expected_value = uint16_t{0x1234},
                                 .name           = "POP_BC",
                                 .validators     = {expect_pop},
                             },
                             InstrParam{
                                 .opcode         = Opcode::POP_DE,
                                 .dst            = Reg16Name::DE,
                                 .initial_sp     = uint16_t{0xFFFC},
                                 .stack_init     = uint16_t{0x5678},
                                 .expected_value = uint16_t{0x5678},
                                 .name           = "POP_DE",
                                 .validators     = {expect_pop},
                             },
                             InstrParam{
                                 .opcode         = Opcode::POP_HL,
                                 .dst            = Reg16Name::HL,
                                 .initial_sp     = uint16_t{0xFFFC},
                                 .stack_init     = uint16_t{0x9ABC},
                                 .expected_value = uint16_t{0x9ABC},
                                 .name           = "POP_HL",
                                 .validators     = {expect_pop},
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// POP AF
INSTANTIATE_TEST_SUITE_P(
    LdInstructions,
    PopAFTest,
    ::testing::Values(
        InstrParam{
            // lsb 0x00 = {z=0, n=0, h=0, c=0}
            .opcode         = Opcode::POP_AF,
            .dst            = Reg16Name::AF,
            .initial_sp     = uint16_t{0xFFFC},
            .stack_init     = uint16_t{0x1200},
            .expected_value = uint16_t{0x1200},
            .expect_z       = false,
            .expect_n       = false,
            .expect_h       = false,
            .expect_c       = false,
            .name           = "POP_AF_Flags_Clear",
            .validators     = {expect_pop},
        },
        InstrParam{
            // lsb 0x30 = {z=0, n=0, h=1, c=1}
            .opcode         = Opcode::POP_AF,
            .dst            = Reg16Name::AF,
            .initial_sp     = uint16_t{0xFFFC},
            .stack_init     = uint16_t{0x3430},
            .expected_value = uint16_t{0x3430},
            .expect_z       = false,
            .expect_n       = false,
            .expect_h       = true,
            .expect_c       = true,
            .name           = "POP_AF_Flags_Low",
            .validators     = {expect_pop},
        },
        InstrParam{
            // lsb 0xC0 = {z=1, n=1, h=0, c=0}
            .opcode         = Opcode::POP_AF,
            .dst            = Reg16Name::AF,
            .initial_sp     = uint16_t{0xFFFC},
            .stack_init     = uint16_t{0x56C0},
            .expected_value = uint16_t{0x56C0},
            .expect_z       = true,
            .expect_n       = true,
            .expect_h       = false,
            .expect_c       = false,
            .name           = "POP_AF_Flags_High",
            .validators     = {expect_pop},
        },
        InstrParam{
            // lsb 0xF0 = {z=1, n=1, h=1, c=1}
            .opcode         = Opcode::POP_AF,
            .dst            = Reg16Name::AF,
            .initial_sp     = uint16_t{0xFFFC},
            .stack_init     = uint16_t{0xABF0},
            .expected_value = uint16_t{0xABF0},
            .expect_z       = true,
            .expect_n       = true,
            .expect_h       = true,
            .expect_c       = true,
            .name           = "POP_AF_Flags_Set",
            .validators     = {expect_pop},
        },
        InstrParam{
            // lsb 0xF0 = {z=1, n=1, h=1, c=1}
            .opcode         = Opcode::POP_AF,
            .dst            = Reg16Name::AF,
            .initial_sp     = uint16_t{0xFFFC},
            .stack_init     = uint16_t{0xABFF},
            .expected_value = uint16_t{0xABF0}, // lower nibble should always be zero
            .expect_z       = true,
            .expect_n       = true,
            .expect_h       = true,
            .expect_c       = true,
            .name           = "POP_AF_Flags_Invalid",
            .validators     = {expect_pop},
        }),
    boyboy::test::cpu::param_name<InstrParam>);