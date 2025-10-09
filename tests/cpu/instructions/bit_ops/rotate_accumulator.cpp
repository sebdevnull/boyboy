/**
 * @file rotate_accumulator.cpp
 * @brief Unit tests for rotate accumulator CPU instructions.
 *
 * RLA
 * RRA
 * RLCA
 * RRCA
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

// Helpers
#include "helpers/cpu_fixtures.h"
#include "helpers/cpu_params.h"

using boyboy::core::cpu::Opcode;

using boyboy::test::cpu::FlagsParam;
using boyboy::test::cpu::InstrParam;
using boyboy::test::cpu::InstrTest;

// -----------------------------
// Test types
// -----------------------------
using RLATest  = InstrTest<InstrParam>;
using RRATest  = InstrTest<InstrParam>;
using RLCATest = InstrTest<InstrParam>;
using RRCATest = InstrTest<InstrParam>;

// -----------------------------
// Test definitions
// -----------------------------
TEST_P(RLATest, Works) { run_test(); }
TEST_P(RRATest, Works) { run_test(); }
TEST_P(RLCATest, Works) { run_test(); }
TEST_P(RRCATest, Works) { run_test(); }

// -----------------------------
// Parameter instantiations
// -----------------------------
// RLA
INSTANTIATE_TEST_SUITE_P(RotateInstructions,
                         RLATest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::RLA,
                                 .dst            = boyboy::core::cpu::Reg8Name::A,
                                 .initial_a      = uint8_t{0b00000000},
                                 .initial_flags  = FlagsParam{.c = false},
                                 .expected_value = uint8_t{0b00000000},
                                 .expect_c       = false,
                                 .name           = "RLA_Zero_NoCarry",
                             },
                             InstrParam{
                                 .opcode         = Opcode::RLA,
                                 .dst            = boyboy::core::cpu::Reg8Name::A,
                                 .initial_a      = uint8_t{0b00000000},
                                 .initial_flags  = FlagsParam{.c = true},
                                 .expected_value = uint8_t{0b00000001},
                                 .expect_c       = false,
                                 .name           = "RLA_Zero_Carry",
                             },
                             InstrParam{
                                 .opcode         = Opcode::RLA,
                                 .dst            = boyboy::core::cpu::Reg8Name::A,
                                 .initial_a      = uint8_t{0b10011001},
                                 .initial_flags  = FlagsParam{.c = false},
                                 .expected_value = uint8_t{0b00110010},
                                 .expect_c       = true,
                                 .name           = "RLA_NoCarry",
                             },
                             InstrParam{
                                 .opcode         = Opcode::RLA,
                                 .dst            = boyboy::core::cpu::Reg8Name::A,
                                 .initial_a      = uint8_t{0b10011001},
                                 .initial_flags  = FlagsParam{.c = true},
                                 .expected_value = uint8_t{0b00110011},
                                 .expect_c       = true,
                                 .name           = "RLA_Carry",
                             },
                             InstrParam{
                                 .opcode         = Opcode::RLA,
                                 .dst            = boyboy::core::cpu::Reg8Name::A,
                                 .initial_a      = uint8_t{0b01111111},
                                 .initial_flags  = FlagsParam{.c = false},
                                 .expected_value = uint8_t{0b11111110},
                                 .expect_c       = false,
                                 .name           = "RLA_Positive_NoCarry",
                             },
                             InstrParam{
                                 .opcode         = Opcode::RLA,
                                 .dst            = boyboy::core::cpu::Reg8Name::A,
                                 .initial_a      = uint8_t{0b01111111},
                                 .initial_flags  = FlagsParam{.c = true},
                                 .expected_value = uint8_t{0b11111111},
                                 .expect_c       = false,
                                 .name           = "RLA_Positive_Carry",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// RRA
INSTANTIATE_TEST_SUITE_P(RotateInstructions,
                         RRATest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::RRA,
                                 .dst            = boyboy::core::cpu::Reg8Name::A,
                                 .initial_a      = uint8_t{0b00000000},
                                 .initial_flags  = FlagsParam{.c = false},
                                 .expected_value = uint8_t{0b00000000},
                                 .expect_c       = false,
                                 .name           = "RRA_Zero_NoCarry",
                             },
                             InstrParam{
                                 .opcode         = Opcode::RRA,
                                 .dst            = boyboy::core::cpu::Reg8Name::A,
                                 .initial_a      = uint8_t{0b00000000},
                                 .initial_flags  = FlagsParam{.c = true},
                                 .expected_value = uint8_t{0b10000000},
                                 .expect_c       = false,
                                 .name           = "RRA_Zero_Carry",
                             },
                             InstrParam{
                                 .opcode         = Opcode::RRA,
                                 .dst            = boyboy::core::cpu::Reg8Name::A,
                                 .initial_a      = uint8_t{0b10011001},
                                 .initial_flags  = FlagsParam{.c = false},
                                 .expected_value = uint8_t{0b01001100},
                                 .expect_c       = true,
                                 .name           = "RRA_NoCarry",
                             },
                             InstrParam{
                                 .opcode         = Opcode::RRA,
                                 .dst            = boyboy::core::cpu::Reg8Name::A,
                                 .initial_a      = uint8_t{0b10011001},
                                 .initial_flags  = FlagsParam{.c = true},
                                 .expected_value = uint8_t{0b11001100},
                                 .expect_c       = true,
                                 .name           = "RRA_Carry",
                             },
                             InstrParam{
                                 .opcode         = Opcode::RRA,
                                 .dst            = boyboy::core::cpu::Reg8Name::A,
                                 .initial_a      = uint8_t{0b11111110},
                                 .initial_flags  = FlagsParam{.c = false},
                                 .expected_value = uint8_t{0b01111111},
                                 .expect_c       = false,
                                 .name           = "RRA_Positive_NoCarry",
                             },
                             InstrParam{
                                 .opcode         = Opcode::RRA,
                                 .dst            = boyboy::core::cpu::Reg8Name::A,
                                 .initial_a      = uint8_t{0b11111110},
                                 .initial_flags  = FlagsParam{.c = true},
                                 .expected_value = uint8_t{0b11111111},
                                 .expect_c       = false,
                                 .name           = "RRA_Positive_Carry",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// RLCA
INSTANTIATE_TEST_SUITE_P(RotateInstructions,
                         RLCATest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::RLCA,
                                 .dst            = boyboy::core::cpu::Reg8Name::A,
                                 .initial_a      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b00000000},
                                 .expect_c       = false,
                                 .name           = "RLCA_Zero",
                             },
                             InstrParam{
                                 .opcode         = Opcode::RLCA,
                                 .dst            = boyboy::core::cpu::Reg8Name::A,
                                 .initial_a      = uint8_t{0b10011001},
                                 .expected_value = uint8_t{0b00110011},
                                 .expect_c       = true,
                                 .name           = "RLCA",
                             },
                             InstrParam{
                                 .opcode         = Opcode::RLCA,
                                 .dst            = boyboy::core::cpu::Reg8Name::A,
                                 .initial_a      = uint8_t{0b01111111},
                                 .expected_value = uint8_t{0b11111110},
                                 .expect_c       = false,
                                 .name           = "RLCA_Positive",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);

// RRCA
INSTANTIATE_TEST_SUITE_P(RotateInstructions,
                         RRCATest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::RRCA,
                                 .dst            = boyboy::core::cpu::Reg8Name::A,
                                 .initial_a      = uint8_t{0b00000000},
                                 .expected_value = uint8_t{0b00000000},
                                 .expect_c       = false,
                                 .name           = "RRCA_Zero",
                             },
                             InstrParam{
                                 .opcode         = Opcode::RRCA,
                                 .dst            = boyboy::core::cpu::Reg8Name::A,
                                 .initial_a      = uint8_t{0b10011001},
                                 .expected_value = uint8_t{0b11001100},
                                 .expect_c       = true,
                                 .name           = "RRCA",
                             },
                             InstrParam{
                                 .opcode         = Opcode::RRCA,
                                 .dst            = boyboy::core::cpu::Reg8Name::A,
                                 .initial_a      = uint8_t{0b11111110},
                                 .expected_value = uint8_t{0b01111111},
                                 .expect_c       = false,
                                 .name           = "RRCA_Positive",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);