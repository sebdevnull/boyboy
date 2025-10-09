/**
 * @file instructions_table.h
 * @brief CPU instructions table for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <array>

#include "boyboy/cpu/cpu.h"
#include "boyboy/cpu/instructions.h"

namespace boyboy::cpu {

class InstructionTable {
public:
    static void execute(Cpu& cpu, OpcodeHandler exec_fn) { (cpu.*exec_fn)(); }
    static const std::array<Instruction, 256>& get_instruction_table(InstructionType type)
    {
        return (type == InstructionType::Unprefixed) ? Opcodes : CBOpcodes;
    }
    static const Instruction& get_instruction(InstructionType type, uint8_t opcode)
    {
        const auto& table = get_instruction_table(type);
        return table.at(opcode);
    }

    static const std::array<Instruction, 256> Opcodes;
    static const std::array<Instruction, 256> CBOpcodes;
};

} // namespace boyboy::cpu