/**
 * @file test_roms.cpp
 * @brief Tests for blargg's CPU test ROMs.
 *
 * These tests run the ROMs and check their serial output for "Passed" or "Failed".
 *
 * ========= Tests timings =========
 * The times below are approximate and only for reference, they may vary depending on the machine.
 * Future reference for optimizations.
 * Run with: ctest --preset debug -R CpuROMTest -V
 *
 * 01-special.gb: ~1.05s
 * 02-interrupts.gb: X (not run, see comment below)
 * 03-op sp,hl.gb: ~1.15s
 * 04-op r,imm.gb: ~1.35s
 * 05-op rp.gb: ~1.85s
 * 06-ld r,r.gb: ~0.30s
 * 07-jr,jp,call,ret,rst.gb: ~0.25s
 * 08-misc instrs.gb: ~0.25s
 * 09-op r,r.gb: ~4.45s
 * 10-bit ops.gb: ~6.8s
 * 11-op a,(hl).gb: ~8.1s
 * cpu_instrs.gb: X (not run, see comment below)
 * ================================
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

// Validated until step 5 where it halts and waits for a timer interrupt, where it gets stuck
// indefinitely. Looking at the code of the test (02-interrupts.s), the timer interrupt vector
// (0x50) returns with RET instead of RETI, which means that IME is never re-enabled and the CPU
// remains halted forever. At some point we could modify and recompile the test ROM to use RETI, but
// for now we will just skip this test.
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