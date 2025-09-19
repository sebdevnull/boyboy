/**
 * @file test_ppu.cpp
 * @brief PPU tests for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include "boyboy/ppu/ppu.h"

using namespace boyboy::ppu;

TEST(PpuTest, InitialState)
{
    Ppu ppu;
    EXPECT_EQ(ppu.mode(), Mode::OAMScan);
    EXPECT_EQ(ppu.ly(), 0);
    EXPECT_FALSE(ppu.frame_ready());
}

TEST(PpuTest, ModeTransitions)
{
    Ppu ppu;

    // Simulate enough cycles to transition through all modes for one frame
    uint32_t total_cycles = 0;
    for (int line = 0; line < VisibleScanlines; ++line) {
        // OAM Scan
        ppu.tick(Cycles::OAMScan);
        total_cycles += Cycles::OAMScan;
        EXPECT_EQ(ppu.mode(), Mode::Transfer) << "Line: " << line;
        EXPECT_EQ(ppu.ly(), line);

        // Pixel Transfer
        ppu.tick(Cycles::Transfer);
        total_cycles += Cycles::Transfer;
        EXPECT_EQ(ppu.mode(), Mode::HBlank) << "Line: " << line;
        EXPECT_EQ(ppu.ly(), line);

        // HBlank
        ppu.tick(Cycles::HBlank);
        total_cycles += Cycles::HBlank;
        if (line < VisibleScanlines - 1) {
            EXPECT_EQ(ppu.mode(), Mode::OAMScan) << "Line: " << line;
            EXPECT_EQ(ppu.ly(), line + 1);
        }
        else if (line == VisibleScanlines - 1) {
            EXPECT_EQ(ppu.mode(), Mode::VBlank) << "Line: " << line;
            EXPECT_EQ(ppu.ly(), VisibleScanlines);
            EXPECT_TRUE(ppu.frame_ready()) << "Frame should be ready after last visible line";
            ppu.consume_frame();
            EXPECT_FALSE(ppu.frame_ready()) << "Frame should be consumed";
        }
        else {
            EXPECT_EQ(ppu.mode(), Mode::VBlank) << "Line: " << line;
            EXPECT_EQ(ppu.ly(), line + 1);
        }
    }

    // Complete VBlank period
    for (int line = VisibleScanlines; line < TotalScanlines; ++line) {
        ppu.tick(Cycles::VBlank);
        total_cycles += Cycles::VBlank;
        if (line < TotalScanlines - 1) {
            EXPECT_EQ(ppu.mode(), Mode::VBlank) << "Line: " << line;
            EXPECT_EQ(ppu.ly(), line + 1);
        }
        else {
            EXPECT_EQ(ppu.mode(), Mode::OAMScan) << "Line: " << line;
            EXPECT_EQ(ppu.ly(), 0);
        }
    }

    EXPECT_EQ(total_cycles, CyclesPerFrame) << "Total cycles should match CyclesPerFrame";
}
