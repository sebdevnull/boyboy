/**
 * @file test_registers.cpp
 * @brief Unit tests for CPU registers.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include "boyboy/cpu/registers.h"

using namespace boyboy::cpu;

// ===== Parameterized test for 16-bit registers using high/low =====
struct Reg16TestParam {
    uint16_t val;
    uint8_t high;
    uint8_t low;
};

class Register16Test : public ::testing::TestWithParam<Reg16TestParam> {};

TEST_P(Register16Test, HighLowAccess)
{
    Reg16TestParam p = GetParam();
    Register16 reg(p.val);

    EXPECT_EQ(reg.high(), p.high);
    EXPECT_EQ(reg.low(), p.low);

    // Modify high
    auto new_high = static_cast<uint8_t>(p.high + 1); // wrap if overflow
    reg.high(new_high);
    EXPECT_EQ(reg.high(), new_high);

    // Modify low
    auto new_low = static_cast<uint8_t>(p.low + 1); // wrap if overflow
    reg.low(new_low);
    EXPECT_EQ(reg.low(), new_low);
}

INSTANTIATE_TEST_SUITE_P(
    Registers,
    Register16Test,
    ::testing::Values(
        Reg16TestParam{0x1234, 0x12, 0x34},
        Reg16TestParam{0xABCD, 0xAB, 0xCD},
        Reg16TestParam{0x0000, 0x00, 0x00},
        Reg16TestParam{0xFFFF, 0xFF, 0xFF}
    )
);

// ===== Prefix and postfix 16-bit register operations test =====
TEST(Register16Test, PrefixPostfixOperations)
{
    Register16 reg(0x0000);

    // Postfix increment
    Register16 old = reg++;
    EXPECT_EQ(old, 0x0000);
    EXPECT_EQ(reg, 0x0001);

    // Postfix decrement
    old = reg--;
    EXPECT_EQ(old, 0x0001);
    EXPECT_EQ(reg, 0x0000);

    // Prefix increment
    Register16& new_ref = ++reg;
    EXPECT_EQ(reg, 0x0001);
    EXPECT_EQ(&new_ref, &reg); // same object

    // Prefix decrement
    new_ref = --reg;
    EXPECT_EQ(reg, 0x0000);
    EXPECT_EQ(&new_ref, &reg); // same object
}

// ===== Parameterized test for AF register with flag handling =====
class AFRegisterFlagTest : public ::testing::TestWithParam<uint8_t> {};

TEST_P(AFRegisterFlagTest, FlagSetAndClear)
{
    uint8_t flag = GetParam();
    AFRegister af;

    // Initially cleared
    af.low(0);
    EXPECT_FALSE((af.low() & flag));

    // Set flag
    switch (flag) {
        case Flag::Zero:
            af.zero_flag(true);
            break;
        case Flag::Carry:
            af.carry_flag(true);
            break;
        case Flag::Substract:
            af.substract_flag(true);
            break;
        case Flag::HalfCarry:
            af.half_carry_flag(true);
            break;
        default:
            FAIL() << "Unknown flag";
    }

    EXPECT_TRUE((af.low() & flag));

    // Clear flag
    switch (flag) {
        case Flag::Zero:
            af.zero_flag(false);
            break;
        case Flag::Carry:
            af.carry_flag(false);
            break;
        case Flag::Substract:
            af.substract_flag(false);
            break;
        case Flag::HalfCarry:
            af.half_carry_flag(false);
            break;
        default:
            FAIL() << "Unknown flag";
    }

    EXPECT_FALSE((af.low() & flag));
}

static std::string FlagNameGenerator(const ::testing::TestParamInfo<uint8_t>& info) // NOLINT
{
    auto name = Flag::to_string(info.param);
    for (auto& c : name) {
        if (std::isalnum(c) == 0) {
            c = '_';
        }
    }
    return name;
}

INSTANTIATE_TEST_SUITE_P(
    Registers,
    AFRegisterFlagTest,
    ::testing::Values(Flag::Zero, Flag::Carry, Flag::Substract, Flag::HalfCarry),
    FlagNameGenerator
);

// ===== Parameterized test for convenience accessors =====
struct RegTestParam {
    std::string name;
    std::function<void(Registers&, uint8_t)> setter;
    std::function<uint8_t(const Registers&)> getter;
    uint8_t value;
};

// overload for cleaner output
static inline std::ostream& operator<<(std::ostream& os, const RegTestParam& p) // NOLINT
{
    os << "[" << p.name << " value=0x" << std::hex << int(p.value) << "]";
    return os;
}

class RegistersAccessorTest : public ::testing::TestWithParam<RegTestParam> {};

TEST_P(RegistersAccessorTest, SetterGetterConsistency)
{
    Registers regs;
    auto p = GetParam();
    p.setter(regs, p.value);
    EXPECT_EQ(p.getter(regs), p.value);
}

INSTANTIATE_TEST_SUITE_P(
    All8BitRegisters,
    RegistersAccessorTest,
    ::testing::Values(
        RegTestParam{
            "A",
            [](Registers& r, uint8_t v) { r.a(v); },
            [](const Registers& r) { return r.a(); },
            0x12
        },
        RegTestParam{
            "B",
            [](Registers& r, uint8_t v) { r.b(v); },
            [](const Registers& r) { return r.b(); },
            0x34
        },
        RegTestParam{
            "C",
            [](Registers& r, uint8_t v) { r.c(v); },
            [](const Registers& r) { return r.c(); },
            0x56
        },
        RegTestParam{
            "D",
            [](Registers& r, uint8_t v) { r.d(v); },
            [](const Registers& r) { return r.d(); },
            0x78
        },
        RegTestParam{
            "E",
            [](Registers& r, uint8_t v) { r.e(v); },
            [](const Registers& r) { return r.e(); },
            0x9A
        },
        RegTestParam{
            "H",
            [](Registers& r, uint8_t v) { r.h(v); },
            [](const Registers& r) { return r.h(); },
            0xBC
        },
        RegTestParam{
            "L",
            [](Registers& r, uint8_t v) { r.l(v); },
            [](const Registers& r) { return r.l(); },
            0xDE
        },
        // F register — only upper nibble is valid, lower 4 bits always masked
        RegTestParam{
            "F",
            [](Registers& r, uint8_t v) { r.f(v); },
            [](const Registers& r) { return r.f(); },
            0xF0
        }
    )
);
