/**
 * @file test_roms.h
 * @brief Test ROM file paths for unit tests.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <string>

namespace boyboy::test::common {

// Test ROMs paths
const std::string ROMsRoot       = "roms/";
const std::string BlarggROMs     = ROMsRoot + "blargg/";
const std::string CustomROMs     = ROMsRoot + "custom/";
const std::string GameBoyLifeROM = ROMsRoot + "gameboylife/life.gb";

const std::string ValidROM   = GameBoyLifeROM;
const std::string InvalidROM = CustomROMs + "invalid.gb";

// CPU test ROM files
const std::string CpuTestFullRom = BlarggROMs + "cpu/cpu_instrs.gb";
const std::string CpuTest01Rom   = BlarggROMs + "cpu/individual/01-special.gb";
const std::string CpuTest02Rom   = BlarggROMs + "cpu/individual/02-interrupts.gb";
const std::string CpuTest03Rom   = BlarggROMs + "cpu/individual/03-op sp,hl.gb";
const std::string CpuTest04Rom   = BlarggROMs + "cpu/individual/04-op r,imm.gb";
const std::string CpuTest05Rom   = BlarggROMs + "cpu/individual/05-op rp.gb";
const std::string CpuTest06Rom   = BlarggROMs + "cpu/individual/06-ld r,r.gb";
const std::string CpuTest07Rom   = BlarggROMs + "cpu/individual/07-jr,jp,call,ret,rst.gb";
const std::string CpuTest08Rom   = BlarggROMs + "cpu/individual/08-misc instrs.gb";
const std::string CpuTest09Rom   = BlarggROMs + "cpu/individual/09-op r,r.gb";
const std::string CpuTest10Rom   = BlarggROMs + "cpu/individual/10-bit ops.gb";
const std::string CpuTest11Rom   = BlarggROMs + "cpu/individual/11-op a,(hl).gb";

} // namespace boyboy::test::common