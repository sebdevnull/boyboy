/**
 * @file ppu.cpp
 * @brief PPU (Pixel Processing Unit) implementation for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/ppu/ppu.h"

#include <cstdint>

#include "boyboy/common/utils.h"
#include "boyboy/io/registers.h"
#include "boyboy/log/logging.h"
#include "boyboy/ppu/registers.h"

namespace boyboy::ppu {

using io::IoReg;

void Ppu::tick(uint16_t cycles)
{
    cycles_ += cycles;
    cycles_in_mode_ += cycles;

    previous_mode_ = mode_;
    previous_ly_ = LY_;

    switch (mode_) {
    case Mode::OAMScan:
        if (cycles_in_mode_ >= Cycles::OAMScan) {
            cycles_in_mode_ -= Cycles::OAMScan;
            switch_mode(Mode::Transfer);
        }
        break;
    case Mode::Transfer:
        if (cycles_in_mode_ >= Cycles::Transfer) {
            cycles_in_mode_ -= Cycles::Transfer;
            mode_ = Mode::HBlank;
            switch_mode(Mode::HBlank);

            // Render the current scanline
            render_scanline();
        }
        break;
    case Mode::HBlank:
        if (cycles_in_mode_ >= Cycles::HBlank) {
            cycles_in_mode_ -= Cycles::HBlank;
            LY_++;
            if (LY_ == VisibleScanlines) {
                // Enter VBlank
                switch_mode(Mode::VBlank);
                frame_ready_ = true;
                frame_count_++;
            }
            else {
                // Continue to next line in OAMScan mode
                switch_mode(Mode::OAMScan);
            }
        }
        break;
    case Mode::VBlank:
        if (cycles_in_mode_ >= Cycles::VBlank) {
            cycles_in_mode_ -= Cycles::VBlank;
            LY_++;
            if (LY_ >= TotalScanlines) {
                // Restart scanning from line 0
                LY_ = 0;
                switch_mode(Mode::OAMScan);
            }
        }
        break;
    }

    check_interrupts();
}

uint8_t Ppu::read(uint16_t addr) const
{
    return registers_.at(IoReg::Ppu::local_addr(addr));
}

void Ppu::write(uint16_t addr, uint8_t value)
{
    if (addr == IoReg::Ppu::LY) {
        // LY is read-only
        return;
    }
    if (addr == IoReg::Ppu::STAT) {
        // Only bits 3-6 are writable
        value &= 0b01111000;
        value |= STAT_ & 0b10000111;
    }

    registers_.at(IoReg::Ppu::local_addr(addr)) = value;
}

void Ppu::set_interrupt_cb(cpu::InterruptRequestCallback callback)
{
    request_interrupt_ = std::move(callback);
}

void Ppu::reset()
{
    registers_.fill(0);
    framebuffer_.fill(0);
    cycles_ = 0;
    cycles_in_mode_ = 0;
    mode_ = Mode::OAMScan;
    frame_ready_ = false;
    frame_count_ = 0;
}

// Fill framebuffer with checkerboard pattern
void Ppu::test_framebuffer()
{
    for (int y = 0; y < LCDHeight; ++y) {
        for (int x = 0; x < LCDWidth; ++x) {
            bool checker = (((x / 8) % 2) ^ ((y / 8) % 2)) != 0;
            uint8_t c = checker ? 0xFF : (frame_count_ % 256);
            framebuffer_.at((y * LCDWidth) + x) = to_rgba(c);
        }
    }
}

void Ppu::switch_mode(Mode new_mode)
{
    mode_ = new_mode;
    STAT_ = (STAT_ & ~registers::STAT::PPUModeMask) | static_cast<uint8_t>(mode_);
}

void Ppu::render_scanline()
{
    render_background();
    render_window();
    render_sprites();
}

void Ppu::render_background()
{
    if (!bg_enabled()) {
        // If background is disabled, fill with color 0
        for (int x = 0; x < LCDWidth; ++x) {
            framebuffer_.at((LY_ * LCDWidth) + x) = palette_color(0, BGP_);
        }
        return;
    }

    uint16_t tilemap_addr = bg_tile_map_addr();
    uint16_t tiledata_addr = bg_window_tile_data_addr();
    uint8_t bg_y = (LY_ + SCY_) & 0xFF;
    uint8_t bg_x = 0;
    uint16_t tile_row = (bg_y / 8) * 32; // Each row has 32 tiles
    uint16_t tile_col = 0;

    for (int x = 0; x < LCDWidth; ++x) {
        bg_x = (SCX_ + x) & 0xFF;
        tile_col = bg_x / 8;

        uint16_t tile_index_addr = tilemap_addr + tile_row + tile_col;
        uint8_t tile_index = mem_read(tile_index_addr);

        uint16_t tile_addr = 0;
        if (tiledata_addr == registers::LCDC::BGAndWindowTileData1) {
            tile_addr = tiledata_addr + (tile_index * 16);
        }
        else {
            tile_addr = tiledata_addr + (static_cast<int8_t>(tile_index) * 16);
        }

        uint8_t line = bg_y % 8;
        uint8_t lsb = mem_read(tile_addr + (line * 2));
        uint8_t msb = mem_read(tile_addr + (line * 2) + 1);

        uint8_t bit = 7 - (bg_x % 8);
        uint8_t color_index = ((msb >> bit) & 0x1) << 1 | ((lsb >> bit) & 0x1);

        framebuffer_.at((LY_ * LCDWidth) + x) = palette_color(color_index, BGP_);
    }
}

void Ppu::render_window()
{
    // Placeholder for window rendering logic
}

void Ppu::render_sprites()
{
    // Placeholder for sprite rendering logic
}

void Ppu::check_interrupts()
{
    if (lcd_off()) {
        return;
    }

    bool stat_request = false;
    bool vblank_request = false;

    if (previous_ly_ != LY_) {
        if ((STAT_ & registers::STAT::LYCInt) != 0 && (LY_ == LYC_)) {
            stat_request = true;
        }
    }

    if (previous_mode_ != mode_) {
        if (mode_ == Mode::OAMScan && (STAT_ & registers::STAT::Mode2OAMInt) != 0) {
            stat_request = true;
        }
        if (mode_ == Mode::VBlank) {
            vblank_request = true;
            if ((STAT_ & registers::STAT::Mode1VBlankInt) != 0) {
                stat_request = true;
            }
        }
        if (mode_ == Mode::HBlank && (STAT_ & registers::STAT::Mode0HBlankInt) != 0) {
            stat_request = true;
        }
    }

    if (vblank_request) {
        request_interrupt(cpu::Interrupts::VBlank);
    }
    if (stat_request) {
        request_interrupt(cpu::Interrupts::LCDStat);
    }
}

void Ppu::request_interrupt(uint8_t interrupt)
{
    if (!request_interrupt_) {
        log::warn("PPU interrupt {} requested but no callback set", interrupt);
        return;
    }

    log::trace("PPU requested interrupt {}. Mode: {}->{}, LY: {}->{}",
               interrupt,
               to_string(previous_mode_),
               to_string(mode_),
               previous_ly_,
               LY_);
    log::trace("LCDC: {}, STAT: {}, SCY: {}, SCX: {}, LYC: {}, BGP: {}",
               utils::PrettyHex(LCDC_).to_string(),
               utils::PrettyHex(STAT_).to_string(),
               utils::PrettyHex(SCY_).to_string(),
               utils::PrettyHex(SCX_).to_string(),
               utils::PrettyHex(LYC_).to_string(),
               utils::PrettyHex(BGP_).to_string());

    request_interrupt_(interrupt);
}

} // namespace boyboy::ppu