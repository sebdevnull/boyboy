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
const std::string RomsRoot       = "roms/";
const std::string CpuRoms        = RomsRoot + "cpu/";
const std::string MiscRoms       = RomsRoot + "misc/";
const std::string BlarggCpuRoms  = CpuRoms + "blargg/";
const std::string CustomRoms     = MiscRoms + "custom/";
const std::string GameBoyLifeRom = MiscRoms + "gameboylife/life.gb";

const std::string ValidROM   = GameBoyLifeRom;
const std::string InvalidROM = CustomRoms + "invalid.gb";

// CPU test ROM files
const std::string CpuTestFullRom = BlarggCpuRoms + "cpu_instrs.gb";
const std::string CpuTest01Rom   = BlarggCpuRoms + "individual/01-special.gb";
const std::string CpuTest02Rom   = BlarggCpuRoms + "individual/02-interrupts.gb";
const std::string CpuTest03Rom   = BlarggCpuRoms + "individual/03-op sp,hl.gb";
const std::string CpuTest04Rom   = BlarggCpuRoms + "individual/04-op r,imm.gb";
const std::string CpuTest05Rom   = BlarggCpuRoms + "individual/05-op rp.gb";
const std::string CpuTest06Rom   = BlarggCpuRoms + "individual/06-ld r,r.gb";
const std::string CpuTest07Rom   = BlarggCpuRoms + "individual/07-jr,jp,call,ret,rst.gb";
const std::string CpuTest08Rom   = BlarggCpuRoms + "individual/08-misc instrs.gb";
const std::string CpuTest09Rom   = BlarggCpuRoms + "individual/09-op r,r.gb";
const std::string CpuTest10Rom   = BlarggCpuRoms + "individual/10-bit ops.gb";
const std::string CpuTest11Rom   = BlarggCpuRoms + "individual/11-op a,(hl).gb";

} // namespace boyboy::test::common