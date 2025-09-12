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
#include "boyboy/common/utils.h"
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

        // Setup test data
        setup_source(param);
        setup_destination(param);
        setup_flags(param);

        // Execute opcode
        run(param.opcode);

        // Run asserts
        run_asserts(param);
    }

private:
    void setup_source(const R8Param& param)
    {
        // Set initial A register if provided
        if (param.initial_a) {
            cpu.set_register(boyboy::cpu::Reg8Name::A, *param.initial_a);
        }

        // Setup source register or memory depending on operand type
        switch (param.src_op_type) {
        case OperandType::Reg8:
            if (param.src) {
                if (!param.src->is_r8()) {
                    throw std::runtime_error(
                        "Source must be an 8-bit register for Reg8 operand type");
                }
                // Only write src_value if src != dst (in practice only A -> A)
                if (!param.dst || !param.src->overlaps(*param.dst)) {
                    cpu.set_register(param.src->get_r8(), param.src_value);
                }
            }
            else {
                throw std::runtime_error("Source register must be specified for Reg8 operand type");
            }
            break;
        case OperandType::Reg16:
            throw std::runtime_error("Reg16 operand type not supported in R8Test");
            break;
        case OperandType::Immediate:
            // Set next byte so that the CPU can read imm from
            cpu.write_byte(cpu.get_pc() + 1, param.src_value);
            break;
        case OperandType::Indirect:
            if (!param.src->is_r16()) {
                throw std::runtime_error(
                    "Source must be a 16-bit register for Indirect operand type");
            }
            if (!param.src_addr) {
                throw std::runtime_error(
                    "Source address must be specified for Indirect operand type");
            }

            cpu.set_register(param.src->get_r16(), *param.src_addr);
            cpu.write_byte(*param.src_addr, param.src_value);
            break;
        case OperandType::Memory: {
            // Set the imm 16-bit address and its value
            uint16_t addr = *param.src_addr;
            cpu.write_byte(cpu.get_pc() + 1, boyboy::utils::lsb(addr));
            cpu.write_byte(cpu.get_pc() + 2, boyboy::utils::msb(addr));
            cpu.write_byte(addr, param.src_value);
            break;
        }
        case OperandType::HighRAM: {
            uint8_t addr_lsb = boyboy::utils::lsb(*param.src_addr);
            if (param.src) {
                cpu.set_register(param.src->get_r8(), addr_lsb);
            }
            else {
                cpu.write_byte(cpu.get_pc() + 1, addr_lsb);
            }
            cpu.write_byte(*param.src_addr, param.src_value);
            break;
        }
        }
    }

    void setup_destination(const R8Param& param)
    {
        // Setup destination as needed
        if (param.dst_op_type) {
            switch (*param.dst_op_type) {
            case OperandType::Indirect:
                // We don't check anything. If it fails, it fails
                cpu.set_register(param.dst->get_r16(), *param.dst_addr);
                break;
            case OperandType::Memory: {
                // Set the imm 16-bit dst address
                uint16_t addr = *param.dst_addr;
                cpu.write_byte(cpu.get_pc() + 1, boyboy::utils::lsb(addr));
                cpu.write_byte(cpu.get_pc() + 2, boyboy::utils::msb(addr));
                break;
            }
            case OperandType::HighRAM: {
                uint8_t addr_lsb = boyboy::utils::lsb(*param.dst_addr);
                if (param.dst) {
                    cpu.set_register(param.dst->get_r8(), addr_lsb);
                }
                else {
                    cpu.write_byte(cpu.get_pc() + 1, addr_lsb);
                }
                break;
            }
            default:
                break;
            }
        }
    }

    void setup_flags(const R8Param& param)
    {
        // Set carry if needed for ADC and SBC
        if (param.carry_in) {
            cpu.set_flag(boyboy::cpu::Flag::Carry, *param.carry_in);
        }
    }

    void run_asserts(const R8Param& param)
    {
        if (param.skip_assert) {
            GTEST_LOG_(INFO) << param.name << " is a NOP, skipping value check";
            return;
        }

        OperandType op = param.target_operand();
        switch (op) {
        case OperandType::Reg8:
        case OperandType::Immediate:
            expect_r8(cpu, param);
            break;
        case OperandType::Reg16:
            throw std::runtime_error("Reg16 operand type not supported in R8Test");
            break;
        case OperandType::Indirect:
        case OperandType::Memory:
        case OperandType::HighRAM:
            expect_at_addr(cpu, param);
            break;
        }

        expect_flags(cpu, param);

        // Run custom asserts
        for (const auto& f : param.validators) {
            f(cpu, param);
        }
    }
};

} // namespace boyboy::test::cpu