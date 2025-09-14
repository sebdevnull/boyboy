/**
 * @file test_instruction_stubs.cpp
 * @brief Unit tests for CPU instructions stubs.
 *
 * @license GPLv3 (see LICENSE file)
 */

#ifdef TEST_CPU_STUBS

#include <gtest/gtest.h>

#include <format>

// boyboy
#include "boyboy/common/errors.h"
#include "boyboy/cpu/cpu.h"
#include "boyboy/cpu/opcodes.h"

// helpers
#include "cpu_stubs.h"
#include "helpers/cpu_fixtures.h"

using boyboy::cpu::CBOpcode;
using boyboy::errors::UnimplementedOpcode;

using boyboy::test::cpu::CpuTest;

class CBInstructionStubTest : public CpuTest, public ::testing::WithParamInterface<uint8_t> {};

TEST(UnprefixedStubOpcodes, AllOpcodesCovered)
{
    EXPECT_EQ(UnprefixedStubOpcodes.size(), 0)
        << "There are unimplemented unprefixed opcodes not covered by stubs";
}

TEST_P(CBInstructionStubTest, ThrowsOnUnimplementedOpcode)
{
    auto opcode = static_cast<CBOpcode>(GetParam());
    EXPECT_THROW(cpu.execute(opcode), UnimplementedOpcode);
}

namespace {
std::string opcode_name(const ::testing::TestParamInfo<uint8_t>& info)
{
    return std::format("Opcode0x{:02X}", info.param);
}
} // namespace

INSTANTIATE_TEST_SUITE_P(CBPrefixedStubOpcodes,
                         CBInstructionStubTest,
                         ::testing::ValuesIn(CBPrefixedStubOpcodes),
                         opcode_name);

#endif