/**
 * @file cpu_asserts.h
 * @brief Helper functions for assertions in CPU tests.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <gtest/gtest.h>

#include <cstdint>
#include <type_traits>
#include <variant>

// boyboy
#include "boyboy/common/utils.h"
#include "boyboy/cpu/cpu.h"
#include "boyboy/cpu/registers.h"

// helpers
#include "helpers/cpu_params.h"

namespace boyboy::test::cpu {

inline void expect_r(const boyboy::cpu::Cpu& cpu, const InstrParam& p)
{
    auto tgt = p.target();
    if (tgt.is_r8()) {
        EXPECT_EQ(cpu.get_register(tgt.get_r8()), p.expected_value8())
            << "Register mismatch: " << p.name;
    }
    else if (tgt.is_r16()) {
        EXPECT_EQ(cpu.get_register(tgt.get_r16()), p.expected_value16())
            << "Register mismatch: " << p.name;
    }
    else {
        throw std::runtime_error("Invalid register type");
    }
}

inline void expect_at_addr(const boyboy::cpu::Cpu& cpu, const InstrParam& p)
{
    uint16_t tgt_addr = p.target_addr();

    std::visit(
        [&](auto&& val) {
            using T = std::decay_t<decltype(val)>;
            if constexpr (std::is_same_v<T, uint8_t>) {
                EXPECT_EQ(cpu.read_byte(tgt_addr), val) << "Register mismatch: " << p.name;
            }
            else if constexpr (std::is_same_v<T, uint16_t>) {
                uint16_t mem_val = utils::to_u16(
                    cpu.read_byte(tgt_addr + 1), cpu.read_byte(tgt_addr)
                );
                EXPECT_EQ(mem_val, val) << "Register mismatch: " << p.name;
            }
        },
        p.expected_value
    );
}

inline void expect_flags(const boyboy::cpu::Cpu& cpu, const InstrParam& p)
{
    EXPECT_EQ(cpu.get_flag(boyboy::cpu::Flag::Zero), p.expect_z) << "Z flag mismatch: " << p.name;
    EXPECT_EQ(cpu.get_flag(boyboy::cpu::Flag::Substract), p.expect_n)
        << "N flag mismatch: " << p.name;
    EXPECT_EQ(cpu.get_flag(boyboy::cpu::Flag::HalfCarry), p.expect_h)
        << "H flag mismatch: " << p.name;
    EXPECT_EQ(cpu.get_flag(boyboy::cpu::Flag::Carry), p.expect_c) << "C flag mismatch: " << p.name;
}

inline void expect_hl_inc(const boyboy::cpu::Cpu& cpu, const InstrParam& p)
{
    EXPECT_EQ(cpu.get_register(boyboy::cpu::Reg16Name::HL), p.target_addr() + 1)
        << "HL increment mismatch: " << p.name;
}

inline void expect_hl_dec(const boyboy::cpu::Cpu& cpu, const InstrParam& p)
{
    EXPECT_EQ(cpu.get_register(boyboy::cpu::Reg16Name::HL), p.target_addr() - 1)
        << "HL increment mismatch: " << p.name;
}

inline void expect_push(const boyboy::cpu::Cpu& cpu, const InstrParam& p)
{
    uint16_t expected_sp = *p.initial_sp - 2;
    uint16_t mem_val = utils::to_u16(cpu.read_byte(expected_sp + 1), cpu.read_byte(expected_sp));

    EXPECT_EQ(mem_val, p.src_value16()) << "Stack memory mismatch: " << p.name;
    EXPECT_EQ(cpu.get_sp(), expected_sp) << "SP push mismatch: " << p.name;
}

inline void expect_pop(const boyboy::cpu::Cpu& cpu, const InstrParam& p)
{
    uint16_t reg = cpu.get_register(p.dst->get_r16());
    EXPECT_EQ(reg, p.expected_value16()) << "Target register mismatch: " << p.name;
    EXPECT_EQ(cpu.get_sp(), *p.initial_sp + 2) << "SP mismatch after: " << p.name;
}

inline void expect_call(const boyboy::cpu::Cpu& cpu, const InstrParam& p)
{
    uint16_t sp   = cpu.get_sp();
    uint16_t addr = boyboy::utils::to_u16(cpu.read_byte(sp + 1), cpu.read_byte(sp));

    EXPECT_EQ(cpu.get_pc(), p.expected_value16()) << "PC mismatch after: " << p.name;
    EXPECT_EQ(sp, *p.initial_sp - 2) << "SP mismatch after: " << p.name;
    EXPECT_EQ(addr, *p.stack_expect) << "Return address mismatch on stack: " << p.name;
}

} // namespace boyboy::test::cpu