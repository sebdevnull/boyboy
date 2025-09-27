/**
 * @file test_mmu.cpp
 * @brief Unit tests for MMU functionality.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include "boyboy/common/utils.h"
#include "boyboy/mmu.h"
#include "boyboy/mmu_constants.h"

using namespace boyboy::mmu;
using namespace boyboy::utils;

class MmuTest : public ::testing::Test {
protected:
    void SetUp() override { mmu.reset(); }
    Mmu mmu;
};

TEST_F(MmuTest, ReadWriteByte)
{
    uint16_t address = VRAMStart;
    uint8_t value    = 0xAB;

    mmu.write_byte(address, value);
    EXPECT_EQ(mmu.read_byte(address), value);
}

TEST_F(MmuTest, ReadWriteWord)
{
    uint16_t address = VRAMStart + 1;
    uint16_t value   = 0xABCD;

    mmu.write_word(address, value);
    EXPECT_EQ(mmu.read_word(address), value);
}

TEST_F(MmuTest, Copy)
{
    uint16_t address            = VRAMStart + 2;
    std::array<uint8_t, 4> data = {0xDE, 0xAD, 0xBE, 0xEF};

    mmu.copy(address, data);
    for (size_t i = 0; i < data.size(); i++) {
        EXPECT_EQ(mmu.read_byte(address + i), data.at(i));
    }
}

TEST_F(MmuTest, WriteByteReadWord)
{
    uint16_t address = VRAMStart + 3;
    uint16_t word    = 0x1234;

    mmu.write_byte(address, lsb(word));
    mmu.write_byte(address + 1, msb(word));
    EXPECT_EQ(mmu.read_word(address), word);
}

TEST_F(MmuTest, WriteWordReadByte)
{
    uint16_t address = VRAMStart + 4;
    uint16_t word    = 0x5678;

    mmu.write_word(address, word);
    EXPECT_EQ(mmu.read_byte(address), lsb(word));
    EXPECT_EQ(mmu.read_byte(address + 1), msb(word));
}

TEST_F(MmuTest, MemoryRegionsRW)
{
    // We don't test all regions in detail, just a few to verify read/write and mirroring
    // Some things to note:
    //      - IO registers have their own tests
    //      - NotUsable is always 0x00 on read and ignores writes
    //      - ROM is read-only
    //      - ECHO is mirrored to WRAM0
    // TODO: Not yet implemented/tested behaviors
    //      - Reading from unmapped areas returns open bus value (last value on bus)
    //      - Bank switching (not implemented yet) -> ROMBank1, ERAM, WRAM1
    //          - ROM and ERAM should return 0xFF if not mapped to anything
    //      - OAM is inaccessible during DMA transfer (ignores writes, reads return open bus)
    //      - Echo area's behavior is strange in DMG: with normal cartridge, mirrors WRAM,
    //        but with some cartridges it mirrors WRAM AND ERAM; it writes to both and reads
    //        are a bitwise AND of both. We ignore this for now and just mirror WRAM0.

    // ROMBank0
    uint8_t ro_value = mmu.read_byte(ROMBank0Start);
    mmu.write_byte(ROMBank0Start, ro_value + 1); // should be ignored
    EXPECT_EQ(mmu.read_byte(ROMBank0Start), ro_value) << "ROMBank0 should be read-only";

    // ROMBank1
    ro_value = mmu.read_byte(ROMBank1Start);
    mmu.write_byte(ROMBank1Start, ro_value + 1); // should be ignored
    EXPECT_EQ(mmu.read_byte(ROMBank1Start), ro_value) << "ROMBank1 should be read-only";

    // VRAM
    mmu.write_byte(VRAMStart, 0xAA);
    EXPECT_EQ(mmu.read_byte(VRAMStart), 0xAA) << "VRAM should be writable";

    // ERAM
    mmu.write_byte(ERAMStart, 0xBB);
    EXPECT_EQ(mmu.read_byte(ERAMStart), 0xBB) << "ERAM should be writable";

    // WRAM0
    mmu.write_byte(WRAM0Start, 0xCC);
    EXPECT_EQ(mmu.read_byte(WRAM0Start), 0xCC) << "WRAM0 should be writable";

    // WRAM1
    mmu.write_byte(WRAM1Start, 0xDD);
    EXPECT_EQ(mmu.read_byte(WRAM1Start), 0xDD) << "WRAM1 should be writable";

    // ECHO (mirrored to WRAM0)
    mmu.write_byte(ECHOStart, 0xEE);
    EXPECT_EQ(mmu.read_byte(ECHOStart), 0xEE) << "ECHO should be writable";
    EXPECT_EQ(mmu.read_byte(WRAM0Start), 0xEE) << "ECHO should mirror WRAM0";
    mmu.write_byte(WRAM0Start, 0xFF);
    EXPECT_EQ(mmu.read_byte(WRAM0Start), 0xFF) << "WRAM0 should be writable";
    EXPECT_EQ(mmu.read_byte(ECHOStart), 0xFF) << "WRAM0 should mirror ECHO";

    // OAM
    mmu.write_byte(OAMStart, 0x11);
    EXPECT_EQ(mmu.read_byte(OAMStart), 0x11) << "OAM should be writable";

    // NotUsable
    uint8_t nu_value = mmu.read_byte(NotUsableStart);
    mmu.write_byte(NotUsableStart, nu_value + 1); // should be ignored
    EXPECT_EQ(mmu.read_byte(NotUsableStart), 0x00) << "NotUsable should always read 0x00";

    // IO (just test a couple of registers)
    mmu.write_byte(IOStart + 6, 0x23); // Timer TMA
    EXPECT_EQ(mmu.read_byte(IOStart + 6), 0x23) << "IO register TMA should be writable";
    mmu.write_byte(IOStart + 16, 0x34); // Sound NR10
    EXPECT_EQ(mmu.read_byte(IOStart + 16), 0x34) << "IO register NR10 should be writable";

    // HRAM
    mmu.write_byte(HRAMStart, 0x33);
    EXPECT_EQ(mmu.read_byte(HRAMStart), 0x33) << "HRAM should be writable";

    // IEReg
    // Only lower 5 bits are used by the interrupt handler, but all bits are r/w
    mmu.write_byte(IEAddr, 0x44);
    EXPECT_EQ(mmu.read_byte(IEAddr), 0x44) << "IEReg should be writable";
}