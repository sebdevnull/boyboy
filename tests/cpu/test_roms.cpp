/**
 * @file test_roms.cpp
 * @brief Tests for blargg's CPU test ROMs.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include "common/roms.h"
#include "helpers/rom_fixtures.h"

using boyboy::test::common::CpuTest09Rom;
using boyboy::test::rom::ROMTest;

class CpuROMTest : public ROMTest {};

TEST_F(CpuROMTest, OpRegReg09)
{
    load(CpuTest09Rom);
    run();
    SUCCEED();
}