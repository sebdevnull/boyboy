/**
 * @file test_roms.h
 * @brief Test ROM file paths for unit tests.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <string>

namespace boyboy::test::common {

const std::string ROMsPath = "roms/";

// CPU test ROM files
const std::string CpuTestFullRom = ROMsPath + "cpu/cpu_instrs.gb";
const std::string CpuTest01Rom   = ROMsPath + "cpu/individual/01-special.gb";
const std::string CpuTest02Rom   = ROMsPath + "cpu/individual/02-interrupts.gb";
const std::string CpuTest03Rom   = ROMsPath + "cpu/individual/03-op sp,hl.gb";
const std::string CpuTest04Rom   = ROMsPath + "cpu/individual/04-op r,imm.gb";
const std::string CpuTest05Rom   = ROMsPath + "cpu/individual/05-op rp.gb";
const std::string CpuTest06Rom   = ROMsPath + "cpu/individual/06-ld r,r.gb";
const std::string CpuTest07Rom   = ROMsPath + "cpu/individual/07-jr,jp,call,ret,rst.gb";
const std::string CpuTest08Rom   = ROMsPath + "individual/cpu/08-misc instrs.gb";
const std::string CpuTest09Rom   = ROMsPath + "cpu/individual/09-op r,r.gb";
const std::string CpuTest10Rom   = ROMsPath + "cpu/individual/10-bit ops.gb";
const std::string CpuTest11Rom   = ROMsPath + "cpu/individual/11-op a,(hl).gb";

const std::string ValidROM   = ROMsPath + "life.gb";
const std::string InvalidROM = ROMsPath + "invalid.gb";

} // namespace boyboy::test::common