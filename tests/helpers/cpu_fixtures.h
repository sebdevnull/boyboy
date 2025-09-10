/**
 * @file cpu_fixtures.h
 * @brief Test fixtures for CPU instruction tests.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <gtest/gtest.h>

#include <cstdint>

#include "helpers/cpu_asserts.h"

// boyboy
#include "boyboy/cpu/cpu.h"

namespace boyboy::test::cpu {

struct CpuTest : public ::testing::Test {
    boyboy::cpu::Cpu cpu;

    void SetUp() override
    {
        // Reset CPU to a clean state before each test
        cpu.reset();
    }

    void run(boyboy::cpu::Opcode opcode) { cpu.execute(opcode); }

    void set_flags(bool z, bool n, bool h, bool c)
    {
        cpu.set_flag(boyboy::cpu::Flag::Zero, z);
        cpu.set_flag(boyboy::cpu::Flag::Substract, n);
        cpu.set_flag(boyboy::cpu::Flag::HalfCarry, h);
        cpu.set_flag(boyboy::cpu::Flag::Carry, c);
    }

    void clear_flags() { set_flags(false, false, false, false); }

    void load_program(std::initializer_list<uint8_t> bytes, uint16_t addr = 0x0100)
    {
        uint16_t pc = addr;
        for (uint8_t b : bytes) {
            cpu.write_byte(pc++, b);
        }
        cpu.set_register(boyboy::cpu::Reg16Name::PC, addr);
    }
};

template <typename ParamType>
class R8Test : public CpuTest, public ::testing::WithParamInterface<ParamType> {
public:
    void run_test()
    {
        auto param = this->GetParam();

        // Determine and setup initial value of the destination register
        uint8_t initial_dst = param.initial_a.value_or(cpu.get_register(param.target()));
        cpu.set_register(param.target(), initial_dst);

        // Determine source value
        uint8_t src_val =
            (param.dst.has_value() && param.src == *param.dst) ? initial_dst : param.src_value;

        // Set carry if needed
        if (param.carry_in.has_value()) {
            cpu.set_flag(boyboy::cpu::Flag::Carry, *param.carry_in);
        }

        cpu.set_register(param.src, src_val);
        cpu.execute(param.opcode);

        expect_r8(cpu, param);
        expect_flags(cpu, param);
    }
};

} // namespace boyboy::test::cpu