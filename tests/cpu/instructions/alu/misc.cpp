/**
 * @file misc.cpp
 * @brief Unit tests for miscellaneous ALU CPU instructions.
 *
 * CPL
 * DAA
 * SCF
 * CCF
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

// Helpers
#include "helpers/cpu_fixtures.h"
#include "helpers/cpu_params.h"

using boyboy::cpu::Opcode;

using boyboy::test::cpu::FlagsParam;
using boyboy::test::cpu::InstrParam;
using boyboy::test::cpu::InstrTest;

// -----------------------------
// Test types
// -----------------------------
using DAATest = InstrTest<InstrParam>;
class CPLTest : public InstrTest<InstrParam> {};
class CCFTest : public InstrTest<InstrParam> {};
class SCFTest : public InstrTest<InstrParam> {};

// -----------------------------
// Test definitions
// -----------------------------
TEST_P(DAATest, Works) { run_test(); }

TEST_F(CPLTest, Works)
{
    cpu.set_register(boyboy::cpu::Reg8Name::A, 0xAA);
    CpuTest::set_flags(false, false, false, false);

    run(Opcode::CPL);
    EXPECT_EQ(cpu.get_register(boyboy::cpu::Reg8Name::A), 0x55)
        << "A register not complemented after CPL";
    EXPECT_FALSE(cpu.get_flag(boyboy::cpu::Flag::Zero)) << "Z flag changed after CPL";
    EXPECT_TRUE(cpu.get_flag(boyboy::cpu::Flag::Substract)) << "N flag not set after CPL";
    EXPECT_TRUE(cpu.get_flag(boyboy::cpu::Flag::HalfCarry)) << "H flag not set after CPL";
    EXPECT_FALSE(cpu.get_flag(boyboy::cpu::Flag::Carry)) << "C flag changed after CPL";

    CpuTest::set_flags(false, false, false, false);

    run(Opcode::CPL);
    EXPECT_EQ(cpu.get_register(boyboy::cpu::Reg8Name::A), 0xAA)
        << "A register not complemented after CPL";
    EXPECT_FALSE(cpu.get_flag(boyboy::cpu::Flag::Zero)) << "Z flag changed after CPL";
    EXPECT_TRUE(cpu.get_flag(boyboy::cpu::Flag::Substract)) << "N flag not set after CPL";
    EXPECT_TRUE(cpu.get_flag(boyboy::cpu::Flag::HalfCarry)) << "H flag not set after CPL";
    EXPECT_FALSE(cpu.get_flag(boyboy::cpu::Flag::Carry)) << "C flag changed after CPL";
}

TEST_F(CCFTest, Works)
{
    CpuTest::set_flags(false, false, false, false);

    run(Opcode::CCF);
    EXPECT_FALSE(cpu.get_flag(boyboy::cpu::Flag::Zero)) << "Z flag changed after CCF";
    EXPECT_FALSE(cpu.get_flag(boyboy::cpu::Flag::Substract)) << "N flag changed after CCF";
    EXPECT_FALSE(cpu.get_flag(boyboy::cpu::Flag::HalfCarry)) << "H flag changed after CCF";
    EXPECT_TRUE(cpu.get_flag(boyboy::cpu::Flag::Carry)) << "C flag not set after CCF";

    run(Opcode::CCF);
    EXPECT_FALSE(cpu.get_flag(boyboy::cpu::Flag::Zero)) << "Z flag changed after CCF";
    EXPECT_FALSE(cpu.get_flag(boyboy::cpu::Flag::Substract)) << "N flag changed after CCF";
    EXPECT_FALSE(cpu.get_flag(boyboy::cpu::Flag::HalfCarry)) << "H flag changed after CCF";
    EXPECT_FALSE(cpu.get_flag(boyboy::cpu::Flag::Carry)) << "C flag not reset after CCF";
}

TEST_F(SCFTest, Works)
{
    CpuTest::set_flags(false, false, false, false);

    run(Opcode::SCF);
    EXPECT_FALSE(cpu.get_flag(boyboy::cpu::Flag::Zero)) << "Z flag changed after SCF";
    EXPECT_FALSE(cpu.get_flag(boyboy::cpu::Flag::Substract)) << "N flag changed after SCF";
    EXPECT_FALSE(cpu.get_flag(boyboy::cpu::Flag::HalfCarry)) << "H flag changed after SCF";
    EXPECT_TRUE(cpu.get_flag(boyboy::cpu::Flag::Carry)) << "C flag not set after SCF";

    run(Opcode::SCF);
    EXPECT_FALSE(cpu.get_flag(boyboy::cpu::Flag::Zero)) << "Z flag changed after SCF";
    EXPECT_FALSE(cpu.get_flag(boyboy::cpu::Flag::Substract)) << "N flag changed after SCF";
    EXPECT_FALSE(cpu.get_flag(boyboy::cpu::Flag::HalfCarry)) << "H flag changed after SCF";
    EXPECT_TRUE(cpu.get_flag(boyboy::cpu::Flag::Carry)) << "C flag not set after SCF";
}

// -----------------------------
// Parameter instantiations
// -----------------------------
// DAA
INSTANTIATE_TEST_SUITE_P(DaaInstruction,
                         DAATest,
                         ::testing::Values(
                             InstrParam{
                                 .opcode         = Opcode::DAA,
                                 .dst            = boyboy::cpu::Reg8Name::A,
                                 .initial_a      = uint8_t{0x45},
                                 .initial_flags  = FlagsParam{.n = false, .h = false, .c = false},
                                 .expected_value = uint8_t{0x45},
                                 .expect_z       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "DAA_45_NoAdjust",
                             },
                             InstrParam{
                                 .opcode         = Opcode::DAA,
                                 .dst            = boyboy::cpu::Reg8Name::A,
                                 .initial_a      = uint8_t{0x9A},
                                 .initial_flags  = FlagsParam{.n = false, .h = false, .c = false},
                                 .expected_value = uint8_t{0x00},
                                 .expect_z       = true,
                                 .expect_h       = false,
                                 .expect_c       = true,
                                 .name           = "DAA_9A_Adjust_C",
                             },
                             InstrParam{
                                 .opcode         = Opcode::DAA,
                                 .dst            = boyboy::cpu::Reg8Name::A,
                                 .initial_a      = uint8_t{0x15},
                                 .initial_flags  = FlagsParam{.n = false, .h = true, .c = false},
                                 .expected_value = uint8_t{0x1B},
                                 .expect_z       = false,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "DAA_15_Adjust_H",
                             },
                             InstrParam{
                                 .opcode         = Opcode::DAA,
                                 .dst            = boyboy::cpu::Reg8Name::A,
                                 .initial_a      = uint8_t{0x9A},
                                 .initial_flags  = FlagsParam{.n = false, .h = true, .c = false},
                                 .expected_value = uint8_t{0x00},
                                 .expect_z       = true,
                                 .expect_h       = false,
                                 .expect_c       = true,
                                 .name           = "DAA_9A_Adjust_H_C",
                             },
                             InstrParam{
                                 .opcode         = Opcode::DAA,
                                 .dst            = boyboy::cpu::Reg8Name::A,
                                 .initial_a      = uint8_t{0x45},
                                 .initial_flags  = FlagsParam{.n = true, .h = false, .c = false},
                                 .expected_value = uint8_t{0x45},
                                 .expect_z       = false,
                                 .expect_n       = true,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "DAA_45_Subtract_NoAdjust",
                             },
                             InstrParam{
                                 .opcode         = Opcode::DAA,
                                 .dst            = boyboy::cpu::Reg8Name::A,
                                 .initial_a      = uint8_t{0x15},
                                 .initial_flags  = FlagsParam{.n = true, .h = true, .c = false},
                                 .expected_value = uint8_t{0x0F},
                                 .expect_z       = false,
                                 .expect_n       = true,
                                 .expect_h       = false,
                                 .expect_c       = false,
                                 .name           = "DAA_15_Subtract_Adjust_H",
                             },
                             InstrParam{
                                 .opcode         = Opcode::DAA,
                                 .dst            = boyboy::cpu::Reg8Name::A,
                                 .initial_a      = uint8_t{0x45},
                                 .initial_flags  = FlagsParam{.n = true, .h = false, .c = true},
                                 .expected_value = uint8_t{0xE5},
                                 .expect_z       = false,
                                 .expect_n       = true,
                                 .expect_h       = false,
                                 .expect_c       = true,
                                 .name           = "DAA_45_Subtract_Adjust_C",
                             },
                             InstrParam{
                                 .opcode         = Opcode::DAA,
                                 .dst            = boyboy::cpu::Reg8Name::A,
                                 .initial_a      = uint8_t{0x15},
                                 .initial_flags  = FlagsParam{.n = true, .h = true, .c = true},
                                 .expected_value = uint8_t{0xAF},
                                 .expect_z       = false,
                                 .expect_n       = true,
                                 .expect_h       = false,
                                 .expect_c       = true,
                                 .name           = "DAA_15_Subtract_Adjust_H_C",
                             }),
                         boyboy::test::cpu::param_name<InstrParam>);
