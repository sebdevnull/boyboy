/**
 * @file test_instructions_8bit.cpp
 * @brief Workbench file for developing and debugging 8-bit CPU tests.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

// Helpers
#include "helpers/cpu_fixtures.h"
#include "helpers/cpu_params.h"

using namespace boyboy::cpu;

using boyboy::test::cpu::R8Param;
using boyboy::test::cpu::R8Test;

// -----------------------------
// Test types
// -----------------------------
using DummyR8Test = R8Test<R8Param>;

// -----------------------------
// Test definitions
// -----------------------------

// -----------------------------
// Parameter instantiations
// -----------------------------
