/**
 * @file instructions.cpp
 * @brief CPU instructions for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "instructions.h"

#include "cpu.h"

namespace boyboy::cpu {

const std::array<Instruction, 256> InstructionTable::Opcodes = []() {
    std::array<Instruction, 256> table{};

#include "generated/opcodes.inc"

    return table;
}();
static_assert(InstructionTable::Opcodes.size() == 256, "Opcodes table must have 256 entries");

const std::array<Instruction, 256> InstructionTable::CBOpcodes = []() {
    std::array<Instruction, 256> table{};

#include "generated/cbopcodes.inc"

    return table;
}();
static_assert(InstructionTable::CBOpcodes.size() == 256, "CB Opcodes table must have 256 entries");

} // namespace boyboy::cpu