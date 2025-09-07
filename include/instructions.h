/**
 * @file instructions.h
 * @brief CPU instructions for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <array>
#include <cstdint>
#include <string_view>

#include "cpu.h"

namespace boyboy::cpu {

class Cpu;

using OpcodeHandler = void (Cpu::*)();

struct Instruction {
    std::string_view mnemonic;
    uint8_t length;
    uint8_t cycles;
    OpcodeHandler execute;
};

class InstructionTable {
public:
    static void execute(Cpu& cpu, OpcodeHandler exec_fn) { (cpu.*exec_fn)(); }

    static const std::array<Instruction, 256> Opcodes;
    static const std::array<Instruction, 256> CBOpcodes;
};

} // namespace boyboy::cpu