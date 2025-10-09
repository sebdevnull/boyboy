/**
 * @file instructions.h
 * @brief CPU instructions for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstdint>
#include <string_view>

namespace boyboy::core::cpu {

class Cpu;

using OpcodeHandler = void (Cpu::*)();

struct Instruction {
    std::string_view mnemonic;
    uint8_t length;
    uint8_t cycles;
    OpcodeHandler execute;
};

enum class InstructionType : uint8_t { Unprefixed, CBPrefixed };

} // namespace boyboy::core::cpu