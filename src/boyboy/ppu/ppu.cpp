/**
 * @file ppu.cpp
 * @brief PPU (Pixel Processing Unit) implementation for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/ppu/ppu.h"

#include <algorithm>
#include <cstdint>

#include "boyboy/common/utils.h"
#include "boyboy/cpu/interrupts.h"
#include "boyboy/io/registers.h"
#include "boyboy/log/logging.h"
#include "boyboy/mmu/constants.h"
#include "boyboy/ppu/registers.h"
#include "boyboy/profiling/profiler_utils.h"

namespace boyboy::ppu {

using io::IoReg;

void Ppu::tick(uint16_t cycles)
{
    BB_PROFILE_SCOPE(profiling::FrameTimer::Ppu);

    if (lcd_off()) {
        return;
    }

    if ((STAT_ & registers::STAT::PPUModeMask) != static_cast<uint8_t>(mode_)) {
        log::warn("PPU mode out of sync! STAT={}, Mode={}",
                  utils::PrettyHex(STAT_).to_string(),
                  to_string(mode_));
        // Correct the mode in STAT
        STAT_ = (STAT_ & ~registers::STAT::PPUModeMask) | static_cast<uint8_t>(mode_);
    }

    if (((STAT_ & registers::STAT::LYCEqualsLY) != 0) != (LY_ == LYC_)) {
        log::warn("PPU LYC=LY flag out of sync! STAT={}, LY={}, LYC={}",
                  utils::PrettyHex(STAT_).to_string(),
                  LY_,
                  LYC_);
    }

    cycles_ += cycles;
    cycles_in_mode_ += cycles;

    previous_mode_ = mode_;
    previous_ly_ = LY_;

    switch (mode_) {
    case Mode::OAMScan:
        if (cycles_in_mode_ >= Cycles::OAMScan) {
            cycles_in_mode_ -= Cycles::OAMScan;
            set_mode(Mode::Transfer);
        }
        break;
    case Mode::Transfer:
        if (cycles_in_mode_ >= Cycles::Transfer) {
            cycles_in_mode_ -= Cycles::Transfer;
            set_mode(Mode::HBlank);

            // Render the current scanline
            render_scanline();
        }
        break;
    case Mode::HBlank:
        if (cycles_in_mode_ >= Cycles::HBlank) {
            cycles_in_mode_ -= Cycles::HBlank;
            // LY_++;
            inc_ly();
            // log::trace("PPU LY incremented to {}", LY_);
            if (LY_ == VisibleScanlines) {
                // Enter VBlank
                set_mode(Mode::VBlank);
                frame_ready_ = true;
                frame_count_++;
                window_line_counter_ = 0;
            }
            else {
                // Continue to next line in OAMScan mode
                set_mode(Mode::OAMScan);
            }
        }
        break;
    case Mode::VBlank:
        if (cycles_in_mode_ >= Cycles::VBlank) {
            cycles_in_mode_ -= Cycles::VBlank;
            // LY_++;
            inc_ly();
            // log::trace("PPU LY incremented to {}", LY_);
            if (LY_ >= TotalScanlines) {
                // Restart scanning from line 0
                // LY_ = 0;
                set_ly(0);
                window_line_counter_ = 0;
                set_mode(Mode::OAMScan);
            }
        }
        break;
    }

    // check_interrupts();
}

uint8_t Ppu::read(uint16_t addr) const
{
    return registers_.at(IoReg::Ppu::local_addr(addr));
}

void Ppu::write(uint16_t addr, uint8_t value)
{
    log::trace("PPU Write: {} <- {}, STAT={}, LY={}, Mode={}",
               IoReg::Ppu::to_string(addr),
               utils::PrettyHex(value).to_string(),
               utils::PrettyHex(STAT_).to_string(),
               LY_,
               to_string(mode_));

    if (addr == IoReg::Ppu::LY) {
        // LY is read-only
        return;
    }

    if (addr == IoReg::Ppu::LCDC) {
        // Check if LCD is being disabled
        bool lcd_enabled = (value & registers::LCDC::LCDAndPPUEnable) != 0;
        if (!lcd_off() && !lcd_enabled) {
            log::info("LCD disabled");
            log::debug("PPU state before LCD OFF: mode={}, LY={}", to_string(mode_), LY_);
            // LY_ = 0;
            set_ly(0);
            cycles_in_mode_ = 0;
            window_line_counter_ = 0;
            set_mode(Mode::HBlank);
            log::debug("PPU state after LCD OFF: mode={}, LY={}", to_string(mode_), LY_);
        }
        else if (lcd_off() && lcd_enabled) {
            log::info("LCD enabled");
            log::debug("PPU state before LCD ON: mode={}, LY={}", to_string(mode_), LY_);
            // LY_ = 0;
            set_ly(0);
            cycles_in_mode_ = 0;
            window_line_counter_ = 0;
            set_mode(Mode::OAMScan);
            log::debug("PPU state after LCD ON: mode={}, LY={}", to_string(mode_), LY_);
        }
    }
    else if (addr == IoReg::Ppu::STAT) {
        // Only bits 3-6 are writable
        value &= 0b01111000;
        value |= STAT_ & 0b10000111;
        log::trace("PPU STAT write, old value: {}, new value: {}",
                   utils::PrettyHex(STAT_).to_string(),
                   utils::PrettyHex(value).to_string());
    }
    else if (addr == IoReg::Ppu::LYC) {
        log::trace("PPU LYC write, old value: {}, new value: {}",
                   utils::PrettyHex(LYC_).to_string(),
                   utils::PrettyHex(value).to_string());
        // Update LYC=LY flag immediately
        LYC_ = value;
        update_lyc();
        return;
    }
    else if (addr == IoReg::Ppu::DMA) {
        dma_start(value);
        return;
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
    // mode_ = Mode::OAMScan;
    previous_mode_ = mode_;
    frame_ready_ = false;
    frame_count_ = 0;
    window_line_counter_ = 0;
    set_mode(Mode::OAMScan);
}

void Ppu::set_ly(uint8_t ly)
{
    LY_ = ly;
    update_lyc();
}

void Ppu::inc_ly()
{
    LY_++;
    update_lyc();
}

void Ppu::update_lyc()
{
    if (LY_ == LYC_) {
        STAT_ |= registers::STAT::LYCEqualsLY;
        if ((STAT_ & registers::STAT::LYCInt) != 0) {
            request_interrupt(cpu::Interrupts::LCDStat);
        }
    }
    else {
        STAT_ &= ~registers::STAT::LYCEqualsLY;
    }
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

void Ppu::set_mode(Mode new_mode)
{
    // Update STAT mode bits
    mode_ = new_mode;
    STAT_ = (STAT_ & ~registers::STAT::PPUModeMask) | static_cast<uint8_t>(mode_);

    // Update LY=LYC flag
    // update_lyc();

    check_interrupts();
}

void Ppu::render_scanline()
{
    // log::trace("Rendering scanline {}", LY_);
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
    if (!window_enabled() || LY_ < WY_) {
        return;
    }

    uint16_t tilemap_addr = window_tile_map_addr();
    uint16_t tiledata_addr = bg_window_tile_data_addr();
    uint8_t win_y = window_line_counter_;

    int first_x = std::max(0, WX_ - 7);
    bool win_visible = first_x < LCDWidth;

    if (!win_visible) {
        return;
    }

    bool drawn = false;
    for (int x = first_x; x < LCDWidth; ++x) {
        uint8_t win_x = x + 7 - WX_;
        uint16_t tile_col = win_x / 8;
        uint16_t tile_row = win_y / 8;
        uint8_t px_in_tile_x = win_x % 8;
        uint8_t px_in_tile_y = win_y % 8;

        uint8_t tile_index = mem_read(tilemap_addr + (tile_row * 32) + tile_col);
        uint16_t tile_addr = 0;
        if (tiledata_addr == registers::LCDC::BGAndWindowTileData1) {
            tile_addr = tiledata_addr + (tile_index * 16);
        }
        else {
            tile_addr = tiledata_addr + (static_cast<int8_t>(tile_index) * 16);
        }

        uint8_t lsb = mem_read(tile_addr + (px_in_tile_y * 2));
        uint8_t msb = mem_read(tile_addr + (px_in_tile_y * 2) + 1);

        uint8_t color_index =
            ((msb >> (7 - px_in_tile_x)) & 0x1) << 1 | ((lsb >> (7 - px_in_tile_x)) & 0x1);

        framebuffer_.at((LY_ * LCDWidth) + x) = palette_color(color_index, BGP_);
        drawn = true;
    }

    if (drawn) {
        window_line_counter_++;
    }
}

void Ppu::render_sprites()
{
    if (!sprites_enabled()) {
        return;
    }

    auto oam_sprites = read_oam();
    std::vector<Sprite> scanline_sprites;

    // Select sprites on current scanline
    for (const auto& sprite : oam_sprites) {
        int sprite_height = large_sprites() ? 16 : 8;
        int sprite_y = sprite.y - 16;
        if (LY_ >= sprite_y && LY_ < (sprite_y + sprite_height)) {
            scanline_sprites.push_back(sprite);
            if (scanline_sprites.size() >= 10) {
                break; // Max 10 sprites per scanline
            }
        }
    }

    // Stable sort by X coordinate to preserve OAM order for sprites with same X
    std::ranges::stable_sort(scanline_sprites,
                             [](const Sprite& a, const Sprite& b) { return a.x < b.x; });

    // Track which X positions have been drawn
    std::array<bool, LCDWidth> x_drawn{false};

    // Render sprites
    for (const auto& sprite : scanline_sprites) {
        render_sprite_pixel(sprite, x_drawn);
    }
}

void Ppu::render_sprite_pixel(const Sprite& sprite, std::array<bool, LCDWidth>& x_drawn)
{
    int sprite_height = large_sprites() ? 16 : 8;

    int sprite_y = sprite.y - 16;
    int y_in_sprite = LY_ - sprite_y;
    if (y_in_sprite < 0 || y_in_sprite >= sprite_height) {
        return; // off-screen vertically
    }

    uint8_t palette = sprite.palette() ? OBP1_ : OBP0_;

    int sprite_x = sprite.x - 8;
    for (int px = 0; px < 8; ++px) {
        int framebuffer_x = sprite_x + px;
        if (framebuffer_x < 0 || framebuffer_x >= LCDWidth) {
            continue; // off-screen horizontally
        }

        if (x_drawn.at(framebuffer_x)) {
            continue; // pixel already drawn by higher priority sprite
        }

        uint8_t color_index = sprite_pixel_color(sprite, y_in_sprite, px);

        if (color_index == 0) {
            continue; // transparent pixel
        }

        Pixel bg_pixel = framebuffer_.at((LY_ * LCDWidth) + framebuffer_x);
        Pixel bg_color0 = palette_color(0, BGP_);
        Pixel sprite_color = palette_color(color_index, palette);

        // Handle sprite priority
        if (!sprite.behind_bg() || bg_pixel == bg_color0) {
            framebuffer_.at((LY_ * LCDWidth) + framebuffer_x) = sprite_color;
            x_drawn.at(framebuffer_x) = true;
        }
    }
}

uint8_t Ppu::sprite_pixel_color(const Sprite& sprite, uint8_t y_in_sprite, uint8_t x_in_sprite)
{
    int sprite_height = large_sprites() ? 16 : 8;

    if (sprite.y_flipped()) {
        y_in_sprite = sprite_height - 1 - y_in_sprite;
    }

    uint8_t tile_index = sprite.tile;
    if (sprite_height == 16) {
        tile_index &= 0xFE; // ignore LSB for 16px sprites
        if (y_in_sprite >= 8) {
            tile_index |= 1;  // use second tile
            y_in_sprite -= 8; // adjust to tile row
        }
    }

    // sprites always use 0x8000 method
    uint16_t tile_addr = registers::LCDC::OBJTileData + (tile_index * 16);

    uint8_t lsb = mem_read(tile_addr + (y_in_sprite * 2));
    uint8_t msb = mem_read(tile_addr + (y_in_sprite * 2) + 1);

    if (sprite.x_flipped()) {
        x_in_sprite = 7 - x_in_sprite;
    }

    // raw color index (0-3)
    uint8_t color_index =
        ((msb >> (7 - x_in_sprite)) & 0x1) << 1 | ((lsb >> (7 - x_in_sprite)) & 0x1);
    return color_index;
}

std::array<Sprite, 40> Ppu::read_oam() const
{
    std::array<Sprite, 40> sprites{};
    for (size_t i = 0; i < 40; ++i) {
        uint16_t base_addr = mmu::OAMStart + (i * 4);
        sprites.at(i).y = mem_read(base_addr);
        sprites.at(i).x = mem_read(base_addr + 1);
        sprites.at(i).tile = mem_read(base_addr + 2);
        sprites.at(i).flags = mem_read(base_addr + 3);
    }
    return sprites;
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
            log::trace("LYC=LY coincidence STAT interrupt triggered (LYC={}, LY={})", LYC_, LY_);
        }
    }

    if (previous_mode_ != mode_) {
        if (mode_ == Mode::OAMScan && (STAT_ & registers::STAT::Mode2OAMInt) != 0) {
            stat_request = true;
            log::trace("OAM STAT interrupt triggered");
        }
        if (mode_ == Mode::VBlank) {
            vblank_request = true;
            log::trace("VBlank interrupt triggered");
            if ((STAT_ & registers::STAT::Mode1VBlankInt) != 0) {
                stat_request = true;
                log::trace("VBlank STAT interrupt triggered");
            }
        }
        if (mode_ == Mode::HBlank && (STAT_ & registers::STAT::Mode0HBlankInt) != 0) {
            stat_request = true;
            log::trace("HBlank STAT interrupt triggered");
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

    uint8_t ie_reg = mem_read(IoReg::Interrupts::IE);
    uint8_t if_reg = mem_read(IoReg::Interrupts::IF);
    log::trace("IE: {}, IF: {}",
               utils::PrettyHex(ie_reg).to_string(),
               utils::PrettyHex(if_reg).to_string());
}

[[nodiscard]] uint8_t Ppu::mem_read(uint16_t addr) const
{
    if (!mem_read_cb_) {
        log::warn("PPU memory read at {} but no callback set", utils::PrettyHex(addr).to_string());
        return 0xFF;
    }
    return mem_read_cb_(addr);
}
void Ppu::mem_write(uint16_t addr, uint8_t value)
{
    if (!mem_write_cb_) {
        log::warn("PPU memory write at {} but no callback set", utils::PrettyHex(addr).to_string());
        return;
    }
    mem_write_cb_(addr, value);
}
void Ppu::dma_start(uint8_t value)
{
    if (!dma_start_cb_) {
        log::warn("PPU DMA start at {} but no callback set", utils::PrettyHex(value).to_string());
        return;
    }
    dma_start_cb_(value);
}

} // namespace boyboy::ppu