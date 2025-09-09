/**
 * @file test_instruction_stubs.cpp
 * @brief Unit tests for CPU instructions stubs.
 *
 * @license GPLv3 (see LICENSE file)
 */

#ifdef TEST_CPU_STUBS

#include <gtest/gtest.h>

#include <format>

#include "boyboy/common/errors.h"
#include "boyboy/cpu/cpu.h"
#include "cpu_stubs.h"

using namespace boyboy::cpu;
using namespace boyboy::errors;

// ----- Stubs tests -----
class InstructionStubParameterizedTest : public ::testing::TestWithParam<uint8_t> {};

TEST_P(InstructionStubParameterizedTest, ThrowsOnUnimplementedOpcode)
{
    Cpu cpu;
    uint8_t opcode = GetParam();

    EXPECT_THROW(cpu.execute(opcode), UnimplementedOpcode);
}

static std::string StubOpcodeNameGenerator(const ::testing::TestParamInfo<uint8_t>& info) // NOLINT
{
    return std::format("Opcode_0x{:02X}", info.param);
}

// We are testing not only that it throws but that stubs are correctly disabled
INSTANTIATE_TEST_SUITE_P(UnprefixedStubOpcodes,
                         InstructionStubParameterizedTest,
                         ::testing::ValuesIn(UnprefixedStubOpcodes),
                         StubOpcodeNameGenerator);

INSTANTIATE_TEST_SUITE_P(CBPrefixedStubOpcodes,
                         InstructionStubParameterizedTest,
                         ::testing::ValuesIn(CBPrefixedStubOpcodes),
                         StubOpcodeNameGenerator);
#endif