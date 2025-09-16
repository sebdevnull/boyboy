/**
 * @file test_roms.cpp
 * @brief Tests for blargg's CPU test ROMs.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include "common/roms.h"
#include "helpers/rom_fixtures.h"

using namespace boyboy::test::common;

using boyboy::test::rom::ROMTest;

class CpuROMTest : public ROMTest {};

TEST_F(CpuROMTest, Special01)
{
    load(CpuTest01Rom);
    run();
}

// TODO: need to implement interrupts
// TEST_F(CpuROMTest, Interrupts02)
// {
//     load(CpuTest02Rom);
//     run();
// }

TEST_F(CpuROMTest, OpSpHl03)
{
    load(CpuTest03Rom);
    run();
}

TEST_F(CpuROMTest, OpRImm04)
{
    load(CpuTest04Rom);
    run();
}

TEST_F(CpuROMTest, OpRp05)
{
    load(CpuTest05Rom);
    run();
}

TEST_F(CpuROMTest, LdRr06)
{
    load(CpuTest06Rom);
    run();
}

TEST_F(CpuROMTest, JrJpCallRetRst07)
{
    load(CpuTest07Rom);
    run();
}

TEST_F(CpuROMTest, MiscInstrs08)
{
    load(CpuTest08Rom);
    run();
}

TEST_F(CpuROMTest, OpRr09)
{
    load(CpuTest09Rom);
    run();
}

TEST_F(CpuROMTest, BitOps10)
{
    load(CpuTest10Rom);
    run();
}

TEST_F(CpuROMTest, OpAAtHl11)
{
    load(CpuTest11Rom);
    run();
}

// TODO: ROM checksum is wrong, so this test is ignored for now
// TEST_F(CpuROMTest, FullCpuInstrs)
// {
//     load(CpuTestFullRom);
//     run();
// }