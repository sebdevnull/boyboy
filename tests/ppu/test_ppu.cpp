/**
 * @file test_ppu.cpp
 * @brief PPU tests for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include "boyboy/core/mmu/mmu.h"
#include "boyboy/core/ppu/ppu.h"
#include "boyboy/core/ppu/registers.h"

using namespace boyboy::core::ppu;
using boyboy::core::io::IoReg;
using boyboy::core::mmu::Mmu;

class PpuTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        mmu_.reset();
        ppu_.reset();
        ppu_.set_mem_read_cb([this](uint16_t addr) { return mmu_.read_byte(addr); });
        ppu_.set_lock_vram_cb([this](bool lock) { mmu_.lock_vram(lock); });
        ppu_.set_lock_oam_cb([this](bool lock) { mmu_.lock_oam(lock); });
    }

    Mmu mmu_;
    Ppu ppu_;

    // Helper to fill a tile in VRAM for background rendering tests
    void fill_bg_tile(uint16_t tile_addr, uint8_t pattern_lo, uint8_t pattern_hi)
    {
        for (int i = 0; i < 8; ++i) {
            mmu_.write_byte(tile_addr + (i * 2), pattern_lo);
            mmu_.write_byte(tile_addr + (i * 2) + 1, pattern_hi);
        }
    }
};

TEST_F(PpuTest, InitialState)
{
    EXPECT_TRUE(ppu_.lcd_off());
    EXPECT_EQ(ppu_.mode(), Mode::HBlank);
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
        if (interrupt == boyboy::core::cpu::Interrupts::VBlank) {
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
        if (interrupt == boyboy::core::cpu::Interrupts::VBlank) {
            vblank_irq_count++;
        }
        else if (interrupt == boyboy::core::cpu::Interrupts::LCDStat) {
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
    EXPECT_EQ(stat_irq_count, (2 * VisibleScanlines) + 1)
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
    EXPECT_EQ(
        stat_irq_count, 2
    ) << "STAT interrupts should be triggered for VBlank->OAMScan and LYC=LY";
    EXPECT_EQ(vblank_irq_count, 0) << "No additional VBlank interrupts should be triggered";
    EXPECT_EQ(ppu_.ly(), 0) << "LY should reset to 0 after VBlank";
    EXPECT_EQ(ppu_.mode(), Mode::OAMScan) << "Should be back to OAMScan mode after VBlank";
}

TEST_F(PpuTest, BG4x4Tilemap)
{
    // Setup tile data at area 1 (0x8000)
    uint8_t pattern_lo     = 0b10101010;
    uint8_t pattern_hi     = 0b01010101;
    uint16_t tiledata_addr = registers::LCDC::BGAndWindowTileData1;
    for (int i = 0; i < 4; ++i) {
        fill_bg_tile(tiledata_addr + (i * 16), pattern_lo, pattern_hi);
    }

    // Setup tilemap (4x4 top-left) at area 0 (0x9800)
    uint16_t tilemap_addr = registers::LCDC::BGTileMapArea0;
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            mmu_.write_byte(tilemap_addr + (y * 32) + x, x); // Tile indices 0-3
        }
    }

    // Configure PPU registers
    uint8_t lcdc = registers::LCDC::LCDAndPPUEnable | registers::LCDC::BGAndWindowEnable |
                   registers::LCDC::BGAndWindowTileData;
    uint8_t bgp = 0xE4; // 11=Black, 10=Dark gray, 01=Light gray, 00=White
    ppu_.write(IoReg::Ppu::LCDC, lcdc);
    ppu_.write(IoReg::Ppu::SCX, 0);   // No horizontal scroll
    ppu_.write(IoReg::Ppu::SCY, 0);   // No vertical scroll
    ppu_.write(IoReg::Ppu::BGP, bgp); // Arbitrary palette

    // Render visible scanlines
    for (int line = 0; line < VisibleScanlines; ++line) {
        ppu_.tick(Cycles::OAMScan);
        ppu_.tick(Cycles::Transfer);
        ppu_.tick(Cycles::HBlank);
    }

    const auto& framebuffer = ppu_.framebuffer();

    // Check first scanline
    int pixel_row = 0;
    for (int x = 0; x < 32; ++x) {
        int tile_index    = x / 8;
        int pixel_in_tile = x % 8;

        uint16_t tile_addr = tiledata_addr + (tile_index * 16);
        uint8_t lo         = mmu_.read_byte(tile_addr + (pixel_row * 2));
        uint8_t hi         = mmu_.read_byte(tile_addr + (pixel_row * 2) + 1);

        uint8_t expected_color_index = ((hi >> (7 - pixel_in_tile)) & 1) << 1 |
                                       ((lo >> (7 - pixel_in_tile)) & 1);

        Pixel expected_px = Ppu::palette_color(expected_color_index, bgp);
        Pixel px          = framebuffer.at((pixel_row * LCDWidth) + x);
        EXPECT_EQ(px, expected_px) << "Pixel mismatch at (" << pixel_row << "," << x << ")";
    }

    // Check a later scanline
    pixel_row = 3;
    for (int x = 0; x < 32; ++x) {
        int tile_index    = x / 8;
        int pixel_in_tile = x % 8;

        uint16_t tile_addr = tiledata_addr + (tile_index * 16);
        uint8_t lo         = mmu_.read_byte(tile_addr + (pixel_row * 2));
        uint8_t hi         = mmu_.read_byte(tile_addr + (pixel_row * 2) + 1);

        uint8_t expected_color_index = ((hi >> (7 - pixel_in_tile)) & 1) << 1 |
                                       ((lo >> (7 - pixel_in_tile)) & 1);

        Pixel expected_px = Ppu::palette_color(expected_color_index, bgp);
        Pixel px          = framebuffer.at((pixel_row * LCDWidth) + x);
        EXPECT_EQ(px, expected_px) << "Pixel mismatch at (" << pixel_row << "," << x << ")";
    }
}

TEST_F(PpuTest, BG4x4TilemapScroll)
{
    // Tile data (same as before)
    uint8_t pattern_lo     = 0b10101010;
    uint8_t pattern_hi     = 0b01010101;
    uint16_t tiledata_addr = registers::LCDC::BGAndWindowTileData1;

    auto fill_bg_tile = [&](uint16_t addr, uint8_t lo, uint8_t hi) {
        for (int row = 0; row < 8; ++row) {
            mmu_.write_byte(addr + (row * 2), lo);
            mmu_.write_byte(addr + (row * 2) + 1, hi);
        }
    };
    for (int i = 0; i < 4; ++i) {
        fill_bg_tile(tiledata_addr + (i * 16), pattern_lo, pattern_hi);
    }

    // Tilemap setup
    uint16_t tilemap_addr = registers::LCDC::BGTileMapArea0;
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            mmu_.write_byte(tilemap_addr + (y * 32) + x, x);
        }
    }

    // PPU registers
    uint8_t lcdc = registers::LCDC::LCDAndPPUEnable | registers::LCDC::BGAndWindowEnable |
                   registers::LCDC::BGAndWindowTileData;
    uint8_t bgp = 0xE4;
    uint8_t scx = 3; // horizontal scroll
    uint8_t scy = 2; // vertical scroll
    ppu_.write(IoReg::Ppu::LCDC, lcdc);
    ppu_.write(IoReg::Ppu::SCX, scx);
    ppu_.write(IoReg::Ppu::SCY, scy);
    ppu_.write(IoReg::Ppu::BGP, bgp);

    // Render first visible scanline (after vertical scroll)
    ppu_.tick(Cycles::OAMScan);
    ppu_.tick(Cycles::Transfer);
    ppu_.tick(Cycles::HBlank);

    const auto& framebuffer = ppu_.framebuffer();

    int pixel_row = scy;           // first visible scanline after SCY=2
    for (int x = 0; x < 32; ++x) { // check first 4 tiles
        int scrolled_x      = (x + scx) % 256;
        int tilemap_row     = pixel_row / 8;
        int tilemap_col     = scrolled_x / 8;
        int pixel_in_tile_x = scrolled_x % 8;
        int pixel_in_tile_y = pixel_row % 8;

        uint8_t tile_number = mmu_.read_byte(tilemap_addr + (tilemap_row * 32) + tilemap_col);
        uint16_t tile_addr  = tiledata_addr + (tile_number * 16);

        uint8_t lo = mmu_.read_byte(tile_addr + (pixel_in_tile_y * 2));
        uint8_t hi = mmu_.read_byte(tile_addr + (pixel_in_tile_y * 2) + 1);

        uint8_t expected_color_index = ((hi >> (7 - pixel_in_tile_x)) & 1) << 1 |
                                       ((lo >> (7 - pixel_in_tile_x)) & 1);
        Pixel expected_px = Ppu::palette_color(expected_color_index, bgp);

        Pixel px = framebuffer.at(x); // first line in framebuffer
        EXPECT_EQ(px, expected_px) << "Pixel mismatch at (0," << x << ")";
    }
}

TEST_F(PpuTest, PpuModeMemLock)
{
    // Ppu starts with LCD off, both should be unlocked
    EXPECT_TRUE(ppu_.lcd_off());
    EXPECT_EQ(ppu_.mode(), Mode::HBlank);
    EXPECT_FALSE(mmu_.is_vram_locked());
    EXPECT_FALSE(mmu_.is_oam_locked());

    // When LCD ON, Ppu starts in mode 2 (OAMScan), OAM should be locked
    ppu_.enable_lcd(true);
    EXPECT_FALSE(ppu_.lcd_off());
    EXPECT_EQ(ppu_.mode(), Mode::OAMScan);
    EXPECT_FALSE(mmu_.is_vram_locked());
    EXPECT_TRUE(mmu_.is_oam_locked());

    // Advance to mode 3 (Transfer), both should be locked
    ppu_.tick(Cycles::OAMScan);
    EXPECT_EQ(ppu_.mode(), Mode::Transfer);
    EXPECT_TRUE(mmu_.is_vram_locked());
    EXPECT_TRUE(mmu_.is_oam_locked());

    // Advance to mode 0 (HBlank), both should be unlocked
    ppu_.tick(Cycles::Transfer);
    EXPECT_EQ(ppu_.mode(), Mode::HBlank);
    EXPECT_FALSE(mmu_.is_vram_locked());
    EXPECT_FALSE(mmu_.is_oam_locked());

    // Finish first scanline and return to mode 2
    ppu_.tick(Cycles::HBlank);

    // Render remaining scanlines to advance to mode 1 (VBlank)
    // Both should be unlocked
    for (int line = 1; line < VisibleScanlines; ++line) {
        ppu_.tick(Cycles::OAMScan);
        ppu_.tick(Cycles::Transfer);
        ppu_.tick(Cycles::HBlank);
    }
    EXPECT_EQ(ppu_.mode(), Mode::VBlank);
    EXPECT_FALSE(mmu_.is_vram_locked());
    EXPECT_FALSE(mmu_.is_oam_locked());

    // Finish frame and go back to mode 3 so that both are locked
    for (int line = 0; line < VBlankScanlines; ++line) {
        ppu_.tick(Cycles::VBlank);
    }
    EXPECT_EQ(ppu_.mode(), Mode::OAMScan);
    ppu_.tick(Cycles::OAMScan);
    EXPECT_EQ(ppu_.mode(), Mode::Transfer);
    EXPECT_TRUE(mmu_.is_vram_locked());
    EXPECT_TRUE(mmu_.is_oam_locked());

    // Turn off LCD, both should be unlocked (because of mode 0)
    ppu_.enable_lcd(false);
    EXPECT_EQ(ppu_.mode(), Mode::HBlank);
    EXPECT_FALSE(mmu_.is_vram_locked());
    EXPECT_FALSE(mmu_.is_oam_locked());
}