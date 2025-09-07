/**
 * @file test_instruction_table.cpp
 * @brief Unit tests for the InstructionTable class.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <set>
#include <string_view>

#include "gtest/gtest.h"
#include "instructions.h"

using namespace boyboy::cpu;

namespace {

void check_unique_function_names(const std::array<Instruction, 256>& table,
                                 const std::string& table_name)
{
    std::set<std::string_view> func_names;
    for (const auto& instr : table) {
        func_names.insert(instr.mnemonic); // Or instr.func_name if available
    }
    EXPECT_EQ(func_names.size(), table.size())
        << "Duplicate function names detected in " << table_name;
}

void check_mnemonic_contains(const Instruction& instr, const std::string& substr)
{
    EXPECT_NE(instr.mnemonic.find(substr), std::string::npos) << instr.mnemonic << ", " << substr;
}

} // namespace

TEST(InstructionTableTest, OpcodesSize)
{
    EXPECT_EQ(InstructionTable::Opcodes.size(), 256);
}

TEST(InstructionTableTest, CBOpcodesSize)
{
    EXPECT_EQ(InstructionTable::CBOpcodes.size(), 256);
}

TEST(InstructionTableTest, UniqueFunctionNames)
{
    check_unique_function_names(InstructionTable::Opcodes, "unprefixed opcodes");
    check_unique_function_names(InstructionTable::CBOpcodes, "CB-prefixed opcodes");
}

TEST(InstructionTableTest, MnemonicFormatting)
{
    // Example: unprefixed
    check_mnemonic_contains(InstructionTable::Opcodes[0x00], "NOP");
    check_mnemonic_contains(InstructionTable::Opcodes[0x34], "[HL]");

    // Example: CB-prefixed
    check_mnemonic_contains(InstructionTable::CBOpcodes[0x11], "C");
    check_mnemonic_contains(InstructionTable::CBOpcodes[0x11], "RL");
}

TEST(InstructionTableTest, IllegalOpcodes)
{
    // Ensure illegal opcodes point to a handler (non-null execute)
    const auto& illegal_opcodes = {0xFC, 0xFD, 0xFE, 0xFF};
    for (auto code : illegal_opcodes) {
        EXPECT_NE(InstructionTable::Opcodes.at(code).execute, nullptr)
            << "Illegal opcode 0x" << std::hex << static_cast<int>(code) << " has no handler";
    }
}

TEST(InstructionTableTest, OperandFormatting)
{
    // Check that memory-pointer operands are wrapped in parentheses
    const auto& ptr_opcodes = {0x34, 0x36}; // Example opcodes that use (HL)
    for (auto code : ptr_opcodes) {
        const auto& instr = InstructionTable::Opcodes.at(code);
        EXPECT_NE(instr.mnemonic.find("[HL]"), std::string::npos)
            << "Opcode 0x" << std::hex << static_cast<int>(code)
            << " should have [HL] in mnemonic: " << instr.mnemonic;
    }

    // Check that increment/decrement operators include + or -
    const auto& inc_opcodes = {0x22, 0x2A}; // Example INC opcodes
    for (auto code : inc_opcodes) {
        const auto& instr = InstructionTable::Opcodes.at(code);
        EXPECT_NE(instr.mnemonic.find('+'), std::string::npos)
            << "Opcode 0x" << std::hex << static_cast<int>(code)
            << " should have '+' in mnemonic: " << instr.mnemonic;
    }

    const auto& dec_opcodes = {0x32, 0x3A}; // Example DEC opcodes
    for (auto code : dec_opcodes) {
        const auto& instr = InstructionTable::Opcodes.at(code);
        EXPECT_NE(instr.mnemonic.find('-'), std::string::npos)
            << "Opcode 0x" << std::hex << static_cast<int>(code)
            << " should have '-' in mnemonic: " << instr.mnemonic;
    }
}
