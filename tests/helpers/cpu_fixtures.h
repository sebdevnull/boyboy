/**
 * @file cpu_fixtures.h
 * @brief Test fixtures for CPU instruction tests.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <gtest/gtest.h>

#include <cstdint>

// boyboy
#include "boyboy/cpu/cpu.h"
#include "boyboy/cpu/registers.h"

// Helpers
#include "helpers/cpu_asserts.h"
#include "helpers/cpu_params.h"

namespace boyboy::test::cpu {

struct CpuTest : public ::testing::Test {
    boyboy::cpu::Cpu cpu;

    void SetUp() override
    {
        // Reset CPU to a clean state before each test
        cpu.reset();
    }

    void run(boyboy::cpu::Opcode opcode)
    {
        cpu.fetch(); // simulate opcode fetch
        cpu.execute(opcode);
    }

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
        // if (!(param.src.has_value() || param.dst.has_value())) {
        //     throw std::runtime_error("Either src or dst register must be specified");
        // }

        // Set initial A register if provided
        if (param.initial_a.has_value()) {
            cpu.set_register(boyboy::cpu::Reg8Name::A, *param.initial_a);
        }

        // Setup source register or memory depending on operand type
        switch (param.operand_type) {
        case ALUOperandType::Reg8:
            if (param.src.has_value()) {
                // Only write src_value if src != dst (in practice only A -> A)
                if (!param.dst.has_value() || param.src.value() != param.dst.value()) {
                    cpu.set_register(*param.src, param.src_value);
                }
            }
            else {
                throw std::runtime_error("Source register must be specified for Reg8 operand type");
            }
            break;
        case ALUOperandType::Immediate:
            // Set next byte so that the CPU can read imm from
            cpu.write_byte(cpu.get_pc() + 1, param.src_value);
            break;
        case ALUOperandType::IndirectHL:
            if (!param.src_addr.has_value()) {
                throw std::runtime_error(
                    "Source address must be specified for IndirectHL operand type");
            }
            cpu.set_register(boyboy::cpu::Reg16Name::HL, *param.src_addr);
            cpu.write_byte(*param.src_addr, param.src_value);
            break;
        }

        // Set carry if needed for ADC and SBC
        if (param.carry_in.has_value()) {
            cpu.set_flag(boyboy::cpu::Flag::Carry, *param.carry_in);
        }

        // Execute opcode
        run(param.opcode);

        // Run asserts
        if (!param.skip_assert) {
            switch (param.operand_type) {
            case ALUOperandType::Reg8:
            case ALUOperandType::Immediate:
                expect_r8(cpu, param);
                break;
            case ALUOperandType::IndirectHL:
                if (param.dst.has_value()) {
                    expect_r8(cpu, param);
                }
                else {
                    expect_at_addr(cpu, param);
                }
                break;
            }
            expect_flags(cpu, param);
        }
        else {
            GTEST_LOG_(INFO) << param.name << " is a NOP, skipping value check";
        }
    }
};

} // namespace boyboy::test::cpu