/**
 * @file ppu.h
 * @brief PPU (Pixel Processing Unit) for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstdint>

#include "boyboy/io/iocomponent.h"
#include "boyboy/io/io.h"

namespace boyboy::ppu {

static constexpr int Width = 160;
static constexpr int Height = 144;
static constexpr int FramebufferSize = Width * Height;

static constexpr uint32_t CyclesPerFrame = 70224; // 456 dots per line * 154 lines

using Pixel = uint32_t; // ARGB format
using FrameBuffer = std::array<Pixel, FramebufferSize>;

class Ppu : public io::IoComponent {
public:
    Ppu() { test_framebuffer(); };

    // IoComponent interface
    void tick(uint16_t cycles) override;
    [[nodiscard]] uint8_t read(uint16_t addr) const override;
    void write(uint16_t addr, uint8_t value) override;
    void set_interrupt_cb(cpu::InterruptRequestCallback callback) override;
    void reset() override;

    // Frame management
    [[nodiscard]] bool frame_ready() const { return frame_ready_; }
    void consume_frame() { frame_ready_ = false; }
    [[nodiscard]] const FrameBuffer& framebuffer() const { return framebuffer_; }

    void test_framebuffer();

private:
    FrameBuffer framebuffer_{};
    uint64_t cycles_ = 0;
    uint64_t frame_count_ = 0;
    bool frame_ready_ = false;
    std::array<uint8_t, io::IoReg::Ppu::Size> registers_{};

    cpu::InterruptRequestCallback request_interrupt_;

};

} // namespace boyboy::ppu