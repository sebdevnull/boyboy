/**
 * @file ppu.cpp
 * @brief PPU (Pixel Processing Unit) implementation for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/ppu/ppu.h"

#include <cstdint>

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
            // test_framebuffer();
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
    for (int y = 0; y < Height; ++y) {
        for (int x = 0; x < Width; ++x) {
            bool checker = (((x / 8) % 2) ^ ((y / 8) % 2)) != 0;
            uint8_t c = checker ? 0xFF : (frame_count_ % 256);
            framebuffer_.at((y * Width) + x) = to_rgba(c);
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
    // For now, just fill the scanline with a color based on LY
    uint8_t color = (LY_ % 256);
    for (int x = 0; x < Width; ++x) {
        framebuffer_.at((LY_ * Width) + x) = to_rgba(color);
    }
}

void Ppu::render_background()
{
    // Placeholder for background rendering logic
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

    if (previous_ly_ != LY_) {
        if ((STAT_ & registers::STAT::LYCInt) != 0 && (LY_ == LYC_)) {
            request_interrupt(cpu::Interrupts::LCDStat);
            return;
        }
    }

    if (previous_mode_ != mode_) {
        if (mode_ == Mode::OAMScan && (STAT_ & registers::STAT::Mode2OAMInt) != 0) {
            request_interrupt(cpu::Interrupts::LCDStat);
            return;
        }
        if (mode_ == Mode::VBlank) {
            request_interrupt(cpu::Interrupts::VBlank);
            if ((STAT_ & registers::STAT::Mode1VBlankInt) != 0) {
                request_interrupt(cpu::Interrupts::LCDStat);
                return;
            }
        }
        if (mode_ == Mode::HBlank && (STAT_ & registers::STAT::Mode0HBlankInt) != 0) {
            request_interrupt(cpu::Interrupts::LCDStat);
            return;
        }
    }
}

void Ppu::request_interrupt(uint8_t interrupt)
{
    if (!request_interrupt_) {
        log::warn("PPU interrupt {} requested but no callback set", interrupt);
        return;
    }

    request_interrupt_(interrupt);
}

} // namespace boyboy::ppu