/**
 * @file cpu_asserts.h
 * @brief Helper functions for assertions in CPU tests.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <gtest/gtest.h>

#include "boyboy/cpu/cpu.h"
#include "boyboy/cpu/registers.h"
#include "helpers/cpu_params.h"

namespace boyboy::test::cpu {

inline void expect_r8(const boyboy::cpu::Cpu& cpu, R8Param& p)
{
    auto tgt = p.target();
    if (!tgt.is_r8()) {
        throw std::runtime_error("Target register must be an 8-bit register");
    }

    EXPECT_EQ(cpu.get_register(tgt.get_r8()), p.expected_value) << "Register mismatch: " << p.name;
}

inline void expect_at_addr(const boyboy::cpu::Cpu& cpu, R8Param& p)
{
    uint16_t tgt_addr = p.target_addr();
    EXPECT_EQ(cpu.read_byte(tgt_addr), p.expected_value) << "Register mismatch: " << p.name;
}

inline void expect_flags(const boyboy::cpu::Cpu& cpu, R8Param& p)
{
    EXPECT_EQ(cpu.get_flag(boyboy::cpu::Flag::Zero), p.expect_z) << "Z flag mismatch: " << p.name;
    EXPECT_EQ(cpu.get_flag(boyboy::cpu::Flag::Substract), p.expect_n)
        << "N flag mismatch: " << p.name;
    EXPECT_EQ(cpu.get_flag(boyboy::cpu::Flag::HalfCarry), p.expect_h)
        << "H flag mismatch: " << p.name;
    EXPECT_EQ(cpu.get_flag(boyboy::cpu::Flag::Carry), p.expect_c) << "C flag mismatch: " << p.name;
}

} // namespace boyboy::test::cpu