/**
 * @file test_ppu.cpp
 * @brief PPU tests for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include <cstdint>

#include "boyboy/mmu.h"
#include "boyboy/ppu/ppu.h"
#include "boyboy/ppu/registers.h"

using namespace boyboy::ppu;
using boyboy::io::IoReg;
using boyboy::mmu::Mmu;

class PpuTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        mmu_.reset();
        ppu_.reset();
        ppu_.set_mem_read_cb([this](uint16_t addr) { return mmu_.read_byte(addr); });
    }

    Mmu mmu_;
    Ppu ppu_;
};

TEST_F(PpuTest, InitialState)
{
    EXPECT_EQ(ppu_.mode(), Mode::OAMScan);
    EXPECT_EQ(ppu_.ly(), 0);
    EXPECT_FALSE(ppu_.frame_ready());
}

TEST_F(PpuTest, ModeTransitions)
{
    // Enable LCD
    ppu_.write(IoReg::Ppu::LCDC, registers::LCDC::LCDAndPPUEnable);

    // Simulate enough cycles to transition through all modes for one frame
    uint32_t total_cycles = 0;
    for (int line = 0; line < VisibleScanlines; ++line) {
        // OAM Scan
        ppu_.tick(Cycles::OAMScan);
        total_cycles += Cycles::OAMScan;
        EXPECT_EQ(ppu_.mode(), Mode::Transfer) << "Line: " << line;
        EXPECT_EQ(ppu_.ly(), line) << "Line: " << line;

        // Pixel Transfer
        ppu_.tick(Cycles::Transfer);
        total_cycles += Cycles::Transfer;
        EXPECT_EQ(ppu_.mode(), Mode::HBlank) << "Line: " << line;
        EXPECT_EQ(ppu_.ly(), line) << "Line: " << line;

        // HBlank
        ppu_.tick(Cycles::HBlank);
        total_cycles += Cycles::HBlank;
        if (line < VisibleScanlines - 1) {
            EXPECT_EQ(ppu_.mode(), Mode::OAMScan) << "Line: " << line;
            EXPECT_EQ(ppu_.ly(), line + 1) << "Line: " << line;
        }
        else if (line == VisibleScanlines - 1) {
            EXPECT_EQ(ppu_.mode(), Mode::VBlank) << "Line: " << line;
            EXPECT_EQ(ppu_.ly(), VisibleScanlines) << "Line: " << line;
            EXPECT_TRUE(ppu_.frame_ready()) << "Frame should be ready after last visible line";
            ppu_.consume_frame();
            EXPECT_FALSE(ppu_.frame_ready()) << "Frame should be consumed";
        }
        else {
            EXPECT_EQ(ppu_.mode(), Mode::VBlank) << "Line: " << line;
            EXPECT_EQ(ppu_.ly(), line + 1) << "Line: " << line;
        }
    }

    // Complete VBlank period
    for (int line = VisibleScanlines; line < TotalScanlines; ++line) {
        ppu_.tick(Cycles::VBlank);
        total_cycles += Cycles::VBlank;
        if (line < TotalScanlines - 1) {
            EXPECT_EQ(ppu_.mode(), Mode::VBlank) << "Line: " << line;
            EXPECT_EQ(ppu_.ly(), line + 1) << "Line: " << line;
        }
        else {
            EXPECT_EQ(ppu_.mode(), Mode::OAMScan) << "Line: " << line;
            EXPECT_EQ(ppu_.ly(), 0) << "Line: " << line;
        }
    }

    EXPECT_EQ(total_cycles, CyclesPerFrame) << "Total cycles should match CyclesPerFrame";
}

TEST_F(PpuTest, VBlankInterrupt)
{
    bool vblank_irq_triggered = false;

    ppu_.set_interrupt_cb([&](uint8_t interrupt) {
        if (interrupt == boyboy::cpu::Interrupts::VBlank) {
            vblank_irq_triggered = true;
        }
    });

    // Enable LCD
    ppu_.write(IoReg::Ppu::LCDC, registers::LCDC::LCDAndPPUEnable);

    // Simulate enough cycles to reach VBlank
    for (int line = 0; line < VisibleScanlines; ++line) {
        ppu_.tick(Cycles::OAMScan);
        ppu_.tick(Cycles::Transfer);
        ppu_.tick(Cycles::HBlank);
    }

    EXPECT_EQ(ppu_.mode(), Mode::VBlank) << "Should be in VBlank mode after visible lines";
    EXPECT_EQ(ppu_.ly(), VisibleScanlines) << "LY should be at the start of VBlank";
    EXPECT_TRUE(vblank_irq_triggered) << "VBlank interrupt should have been triggered";
}

TEST_F(PpuTest, StatInterrupts)
{
    uint8_t vblank_irq_count = 0;
    uint16_t stat_irq_count  = 0;

    ppu_.set_interrupt_cb([&](uint8_t interrupt) {
        if (interrupt == boyboy::cpu::Interrupts::VBlank) {
            vblank_irq_count++;
        }
        else if (interrupt == boyboy::cpu::Interrupts::LCDStat) {
            stat_irq_count++;
        }
    });

    // Enable LCD
    ppu_.write(IoReg::Ppu::LCDC, registers::LCDC::LCDAndPPUEnable);

    // Enable all STAT interrupts for testing
    uint8_t stat = registers::STAT::LYCInt | registers::STAT::Mode2OAMInt |
                   registers::STAT::Mode1VBlankInt | registers::STAT::Mode0HBlankInt;
    ppu_.write(IoReg::Ppu::STAT, stat);

    uint8_t lyc_value = 10;
    ppu_.write(IoReg::Ppu::LYC, lyc_value);

    // Simulate enough cycles to go through one full frame
    for (int line = 0; line < VisibleScanlines; ++line) {
        ppu_.tick(Cycles::OAMScan);
        ppu_.tick(Cycles::Transfer);
        ppu_.tick(Cycles::HBlank);
    }
    // Should trigger 2 mode switch interrupts per visible line (OAM and HBlank) minus one OAMScan
    // for the first line
    // Plus one LYC=LY interrupt when LY matches LYC
    EXPECT_EQ(stat_irq_count, 2 * VisibleScanlines)
        << "STAT interrupts should be triggered for OAM, HBlank, and LYC=LY";
    EXPECT_EQ(vblank_irq_count, 1) << "One VBlank interrupt should be triggered";
    EXPECT_EQ(ppu_.mode(), Mode::VBlank) << "Should be in VBlank mode after visible lines";

    // Reset counts for VBlank period test
    stat_irq_count   = 0;
    vblank_irq_count = 0;

    lyc_value = TotalScanlines - 1; // Set LYC to last line of VBlank
    ppu_.write(IoReg::Ppu::LYC, lyc_value);

    // Complete VBlank period
    for (int line = 0; line < VBlankScanlines; ++line) {
        ppu_.tick(Cycles::VBlank);
    }
    // Should trigger 1 mode switch interrupt from VBlank to OAMScan in the last line
    // Plus one LYC=LY interrupt when LY matches LYC
    EXPECT_EQ(stat_irq_count, 2)
        << "STAT interrupts should be triggered for VBlank->OAMScan and LYC=LY";
    EXPECT_EQ(vblank_irq_count, 0) << "No additional VBlank interrupts should be triggered";
    EXPECT_EQ(ppu_.ly(), 0) << "LY should reset to 0 after VBlank";
    EXPECT_EQ(ppu_.mode(), Mode::OAMScan) << "Should be back to OAMScan mode after VBlank";
}
