/**
 * @file test_instruction_stubs.cpp
 * @brief Unit tests for CPU instructions stubs.
 *
 * @license GPLv3 (see LICENSE file)
 */

#ifdef TEST_CPU_STUBS

#include <gtest/gtest.h>

#include "cpu_stubs.h"

TEST(UnprefixedStubOpcodes, AllOpcodesCovered)
{
    EXPECT_EQ(UnprefixedStubOpcodes.size(), 0)
        << "There are unimplemented unprefixed opcodes not covered by stubs";
}

TEST(CBPrefixedStubOpcodes, AllOpcodesCovered)
{
    EXPECT_EQ(CBPrefixedStubOpcodes.size(), 0)
        << "There are unimplemented CB-prefixed opcodes not covered by stubs";
}

#endif