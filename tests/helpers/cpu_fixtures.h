/**
 * @file cpu_fixtures.h
 * @brief Test fixtures for CPU instruction tests.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <type_traits>
#include <variant>
#include <vector>

// boyboy
#include "boyboy/common/utils.h"
#include "boyboy/core/cpu/cpu.h"
#include "boyboy/core/cpu/cpu_constants.h"
#include "boyboy/core/cpu/cycles.h"
#include "boyboy/core/cpu/registers.h"
#include "boyboy/core/io/io.h"
#include "boyboy/core/mmu/constants.h"
#include "boyboy/core/mmu/mmu.h"

// Helpers
#include "helpers/cpu_asserts.h"
#include "helpers/cpu_params.h"

namespace boyboy::test::cpu {

struct CpuTest : public ::testing::Test {
    std::shared_ptr<boyboy::core::io::Io> io; // needed for interrupts
    std::shared_ptr<boyboy::core::mmu::Mmu> mmu;
    std::unique_ptr<boyboy::core::cpu::Cpu> cpu;

    void SetUp() override
    {
        io  = std::make_shared<boyboy::core::io::Io>();
        mmu = std::make_shared<boyboy::core::mmu::Mmu>(io);
        cpu = std::make_unique<boyboy::core::cpu::Cpu>(mmu);

        mmu->init();
        cpu->init();

        // Set flags to known state
        clear_flags();

        // Set PC to a writable area (WRAM) to avoid issues with read-only memory
        cpu->set_pc(boyboy::core::mmu::WRAM0Start);

        // Set tick mode to Instruction
        cpu->set_tick_mode(core::cpu::TickMode::Instruction);
    }

    void run(boyboy::core::cpu::Opcode opcode) const
    {
        cpu->fetch(); // simulate opcode fetch
        cpu->execute(opcode);
    }

    void run(boyboy::core::cpu::CBOpcode opcode) const
    {
        cpu->fetch(); // simulate 0xCB prefix fetch
        cpu->fetch(); // simulate opcode fetch
        cpu->execute(opcode);
    }

    void set_next_instruction(boyboy::core::cpu::Opcode opcode) const
    {
        uint16_t pc = cpu->get_pc();
        cpu->write_byte(pc, static_cast<uint8_t>(opcode));
    }

    void set_next_instruction(boyboy::core::cpu::CBOpcode opcode) const
    {
        uint16_t pc = cpu->get_pc();
        cpu->write_byte(pc, boyboy::core::cpu::CBInstructionPrefix);
        cpu->write_byte(pc + 1, static_cast<uint8_t>(opcode));
    }

    void set_next_bytes(const std::vector<uint8_t>& instrs) const
    {
        auto pc = cpu->get_pc();
        for (const auto& instr : instrs) {
            cpu->write_byte(pc++, instr);
        }
    }

    void set_flags(bool z, bool n, bool h, bool c) const
    {
        cpu->set_flag(boyboy::core::cpu::Flag::Zero, z);
        cpu->set_flag(boyboy::core::cpu::Flag::Substract, n);
        cpu->set_flag(boyboy::core::cpu::Flag::HalfCarry, h);
        cpu->set_flag(boyboy::core::cpu::Flag::Carry, c);
    }

    void clear_flags() const { set_flags(false, false, false, false); }

    void load_program(std::initializer_list<uint8_t> bytes, uint16_t addr = 0x0100) const
    {
        uint16_t pc = addr;
        for (uint8_t b : bytes) {
            cpu->write_byte(pc++, b);
        }
        cpu->set_pc(addr);
    }
};

template <typename ParamType>
class InstrTest : public CpuTest, public ::testing::WithParamInterface<ParamType> {
public:
    void run_test()
    {
        auto param = this->GetParam();

        // Setup test data
        setup_source(param);
        setup_destination(param);
        setup_flags(param);

        // Execute opcode
        std::visit([&](auto&& op) { run(op); }, param.opcode);

        // Run asserts
        run_asserts(param);
    }

    void set_flags(const FlagsParam& flags)
    {
        CpuTest::set_flags(flags.z, flags.n, flags.h, flags.c);
    }

private:
    void setup_source(const InstrParam& param)
    {
        // Set initial A register if provided
        if (param.initial_a) {
            cpu->set_register(boyboy::core::cpu::Reg8Name::A, *param.initial_a);
        }
        // Set initial HL if provided
        if (param.initial_hl) {
            cpu->set_register(boyboy::core::cpu::Reg16Name::HL, *param.initial_hl);
        }
        // Set initial PC if provided
        if (param.initial_pc) {
            cpu->set_pc(*param.initial_pc);
        }
        // Set SP and preload stack if needed
        if (param.initial_sp) {
            uint16_t sp = *param.initial_sp;
            cpu->set_sp(sp);
            if (param.stack_init) {
                cpu->write_byte(sp, boyboy::common::utils::lsb(*param.stack_init));
                cpu->write_byte(sp + 1, boyboy::common::utils::msb(*param.stack_init));
            }
        }

        // Setup source depending on operand type
        if (param.src_op_type && param.src_value) {
            switch (*param.src_op_type) {
                case OperandType::Register:
                    setup_register_source(param);
                    break;
                case OperandType::Immediate:
                    setup_immediate_source(param);
                    break;
                case OperandType::Indirect:
                    setup_indirect_source(param);
                    break;
                case OperandType::Memory:
                    setup_memory_source(param);
                    break;
                case OperandType::HighRAM:
                    setup_high_ram_source(param);
                    break;
            }
        }
    }

    void setup_register_source(const InstrParam& param)
    {
        if (!param.src) {
            throw std::runtime_error("Source register must be specified");
        }
        if (param.src->is_r8()) {
            cpu->set_register(param.src->get_r8(), param.src_value8());
        }
        else if (param.src->is_r16()) {
            cpu->set_register(param.src->get_r16(), param.src_value16());
        }
        else {
            throw std::runtime_error("Invalid register type");
        }
    }

    void setup_immediate_source(const InstrParam& param)
    {
        uint16_t pc = cpu->get_pc();
        // Set next byte(s) so that the CPU can read imm from
        std::visit(
            [&](auto&& val) {
                using T = std::decay_t<decltype(val)>;
                if constexpr (std::is_same_v<T, uint8_t>) {
                    cpu->write_byte(pc + 1, val);
                }
                else if (std::is_same_v<T, uint16_t>) {
                    cpu->write_byte(pc + 1, boyboy::common::utils::lsb(val));
                    cpu->write_byte(pc + 2, boyboy::common::utils::msb(val));
                }
            },
            *param.src_value
        );
    }

    void setup_indirect_source(const InstrParam& param)
    {
        if (!param.src || !param.src->is_r16()) {
            throw std::runtime_error("Indirect source must be 16-bit register");
        }
        if (!param.src_addr) {
            throw std::runtime_error("Source address must be specified for Indirect operand type");
        }
        cpu->set_register(param.src->get_r16(), *param.src_addr);
        cpu->write_byte(*param.src_addr, param.src_value8()); // indirect always 8-bit}
    }

    void setup_memory_source(const InstrParam& param)
    {
        if (!param.src_addr) {
            throw std::runtime_error("Memory address must be specified");
        }
        // Set the imm 16-bit address and its value
        uint16_t addr = *param.src_addr;
        uint16_t pc   = cpu->get_pc();
        cpu->write_byte(pc + 1, boyboy::common::utils::lsb(addr));
        cpu->write_byte(pc + 2, boyboy::common::utils::msb(addr));

        // Write value to the memory address
        std::visit(
            [&](auto&& val) {
                using T = std::decay_t<decltype(val)>;
                if constexpr (std::is_same_v<T, uint8_t>) {
                    cpu->write_byte(*param.src_addr, val);
                }
                else if constexpr (std::is_same_v<T, uint16_t>) {
                    cpu->write_byte(*param.src_addr, boyboy::common::utils::lsb(val));
                    cpu->write_byte(*param.src_addr + 1, boyboy::common::utils::msb(val));
                }
            },
            *param.src_value
        );
    }

    void setup_high_ram_source(const InstrParam& param)
    {
        if (!param.src_addr) {
            throw std::runtime_error("HighRAM address must be specified");
        }
        uint8_t addr_lsb = boyboy::common::utils::lsb(*param.src_addr);
        if (param.src && param.src->is_r8()) {
            cpu->set_register(param.src->get_r8(), addr_lsb);
        }
        else {
            cpu->write_byte(cpu->get_pc() + 1, addr_lsb);
        }
        cpu->write_byte(*param.src_addr, param.src_value8());
    }

    void setup_destination(const InstrParam& param)
    {
        // Setup destination as needed
        if (param.dst_op_type) {
            switch (*param.dst_op_type) {
                case OperandType::Indirect:
                    // Setup only if we have a dst address
                    if (param.dst && param.dst->is_r16() && param.dst_addr) {
                        cpu->set_register(param.dst->get_r16(), *param.dst_addr);
                    }
                    break;
                case OperandType::Memory: {
                    // Set the imm 16-bit dst address
                    uint16_t addr = *param.dst_addr;
                    cpu->write_byte(cpu->get_pc() + 1, boyboy::common::utils::lsb(addr));
                    cpu->write_byte(cpu->get_pc() + 2, boyboy::common::utils::msb(addr));
                    break;
                }
                case OperandType::HighRAM: {
                    uint8_t addr_lsb = boyboy::common::utils::lsb(*param.dst_addr);
                    if (param.dst) {
                        cpu->set_register(param.dst->get_r8(), addr_lsb);
                    }
                    else {
                        cpu->write_byte(cpu->get_pc() + 1, addr_lsb);
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }

    void setup_flags(const InstrParam& param)
    {
        if (param.initial_flags) {
            set_flags(*param.initial_flags);
        }
    }

    void run_asserts(const InstrParam& param)
    {
        if (param.skip_assert) {
            GTEST_LOG_(INFO) << param.name << " is a NOP, skipping value check";
            return;
        }

        OperandType op = param.target_operand();
        switch (op) {
            case OperandType::Register:
            case OperandType::Immediate:
                expect_r(*cpu, param);
                break;
            case OperandType::Indirect:
            case OperandType::Memory:
            case OperandType::HighRAM:
                expect_at_addr(*cpu, param);
                break;
        }

        expect_flags(*cpu, param);

        // Run custom asserts
        for (const auto& f : param.validators) {
            f(*cpu, param);
        }
    }
};

} // namespace boyboy::test::cpu