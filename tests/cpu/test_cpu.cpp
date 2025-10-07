/**
 * @file test_cpu.cpp
 * @brief Unit tests for the Cpu class in the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

// boyboy
#include "boyboy/cpu/cpu.h"
#include "boyboy/cpu/registers.h"

// Helpers
#include "helpers/cpu_fixtures.h"

using boyboy::cpu::Flag;
using boyboy::cpu::Reg16Name;
using boyboy::cpu::Reg8Name;

using boyboy::test::cpu::CpuTest;

// Parameterized tests for CPU register access
class CpuRegister8Test : public CpuTest,
                         public ::testing::WithParamInterface<std::pair<Reg8Name, uint8_t>> {};

TEST_P(CpuRegister8Test, SetAndGetRegister8)
{
    auto [reg, value] = GetParam();

    cpu.set_register(reg, value);
    EXPECT_EQ(cpu.get_register(reg), value);
}

INSTANTIATE_TEST_SUITE_P(
    Register8Tests,
    CpuRegister8Test,
    ::testing::Values(
        std::make_pair(Reg8Name::A, 0x12),
        std::make_pair(Reg8Name::B, 0x34),
        std::make_pair(Reg8Name::C, 0x56),
        std::make_pair(Reg8Name::D, 0x78),
        std::make_pair(Reg8Name::E, 0x9A),
        std::make_pair(Reg8Name::H, 0xBC),
        std::make_pair(Reg8Name::L, 0xDE),
        std::make_pair(Reg8Name::F, 0xF0)
    )
);

class CpuRegister16Test : public CpuTest,
                          public ::testing::WithParamInterface<std::pair<Reg16Name, uint16_t>> {};

TEST_P(CpuRegister16Test, SetAndGetRegister16)
{
    auto [reg, value] = GetParam();

    cpu.set_register(reg, value);
    EXPECT_EQ(cpu.get_register(reg), value);
}

INSTANTIATE_TEST_SUITE_P(
    Register16Tests,
    CpuRegister16Test,
    ::testing::Values(
        std::make_pair(Reg16Name::BC, 0x1234),
        std::make_pair(Reg16Name::DE, 0x5678),
        std::make_pair(Reg16Name::HL, 0x9ABC),
        std::make_pair(Reg16Name::AF, 0xDEF0),
        std::make_pair(Reg16Name::SP, 0xFFF0),
        std::make_pair(Reg16Name::PC, 0xABCD)
    )
);

// Flags tests
class CpuFlagTest : public CpuTest,
                    public ::testing::WithParamInterface<std::pair<uint8_t, bool>> {};

TEST_P(CpuFlagTest, SetAndGetFlag)
{
    auto [flag, value] = GetParam();

    cpu.set_flag(flag, value);
    EXPECT_EQ(cpu.get_flag(flag), value);
}

INSTANTIATE_TEST_SUITE_P(
    FlagTests,
    CpuFlagTest,
    ::testing::Values(
        std::make_pair(Flag::Zero, true),
        std::make_pair(Flag::Zero, false),
        std::make_pair(Flag::Carry, true),
        std::make_pair(Flag::Carry, false),
        std::make_pair(Flag::Substract, true),
        std::make_pair(Flag::Substract, false),
        std::make_pair(Flag::HalfCarry, true),
        std::make_pair(Flag::HalfCarry, false)
    )
);