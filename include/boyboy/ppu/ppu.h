/**
 * @file ppu.h
 * @brief PPU (Pixel Processing Unit) for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstdint>
#include <ostream>

#include "boyboy/io/iocomponent.h"
#include "boyboy/io/registers.h"
#include "boyboy/ppu/registers.h"

namespace boyboy::ppu {

// Screen dimensions
static constexpr int Width = 160;
static constexpr int Height = 144;
static constexpr int FramebufferSize = Width * Height;

// PPU Modes
enum class Mode : uint8_t {
    HBlank = 0,   // Waiting until the end of the scanline
    VBlank = 1,   // Waiting until the next frame
    OAMScan = 2,  // Searching for OBJs which overlap this line
    Transfer = 3, // Sending pixels to the LCD
};

// Mode enum string conversion for debugging
inline const char* to_string(Mode mode)
{
    switch (mode) {
    case Mode::HBlank:
        return "HBlank";
    case Mode::VBlank:
        return "VBlank";
    case Mode::OAMScan:
        return "OAMScan";
    case Mode::Transfer:
        return "Transfer";
    default:
        return "Unknown";
    }
}
inline std::ostream& operator<<(std::ostream& os, Mode mode)
{
    return os << to_string(mode);
}

// Number of cycles in each mode
// TODO: Transfer and HBlank depend on the number of sprites drawn but we use fixed values for now
struct Cycles {
    static constexpr uint16_t OAMScan = 80;
    static constexpr uint16_t Transfer = 172;
    static constexpr uint16_t HBlank = 204;
    static constexpr uint16_t VBlank = 456;
};

// Number of scanlines
static constexpr int VisibleScanlines = Height;
static constexpr int VBlankScanlines = 10;
static constexpr int TotalScanlines = VisibleScanlines + VBlankScanlines;

// Total cycles per frame (70224)
// The number of cycles is fixed, but we derive it from mode cycles and scanlines for clarity
static constexpr uint32_t CyclesPerFrame =
    ((Cycles::OAMScan + Cycles::Transfer + Cycles::HBlank) * VisibleScanlines) +
    (Cycles::VBlank * VBlankScanlines);

// Types definitions
using Pixel = uint32_t; // RGBA format
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

    // Accessors for convenience and testing
    [[nodiscard]] Mode mode() const { return mode_; }
    [[nodiscard]] uint8_t ly() const { return LY_; }
    [[nodiscard]] bool lcd_off() const { return (LCDC_ & registers::LCDC::LCDAndPPUEnable) == 0; }

    void test_framebuffer();

private:
    // PPU state
    Mode mode_ = Mode::OAMScan;
    Mode previous_mode_ = mode_;
    uint8_t previous_ly_ = 0;
    int cycles_ = 0;
    int cycles_in_mode_ = 0;

    // Frame management
    uint64_t frame_count_ = 0;
    bool frame_ready_ = false;
    FrameBuffer framebuffer_{};

    // I/O registers
    // Using array for easy reset and direct access
    // Frequently accessed registers have references to avoid repeated lookups
    std::array<uint8_t, io::IoReg::Ppu::Size> registers_{};
    uint8_t& LY_ = registers_.at(io::IoReg::Ppu::local_addr(io::IoReg::Ppu::LY));
    uint8_t& STAT_ = registers_.at(io::IoReg::Ppu::local_addr(io::IoReg::Ppu::STAT));
    uint8_t& LCDC_ = registers_.at(io::IoReg::Ppu::local_addr(io::IoReg::Ppu::LCDC));
    uint8_t& LYC_ = registers_.at(io::IoReg::Ppu::local_addr(io::IoReg::Ppu::LYC));

    cpu::InterruptRequestCallback request_interrupt_;

    void switch_mode(Mode new_mode);
    void render_scanline();
    void render_background();
    void render_window();
    void render_sprites();

    void check_interrupts();
    void request_interrupt(uint8_t interrupt);

    [[nodiscard]] static Pixel to_rgba(uint8_t color)
    {
        return 0xFF000000 | (color << 16) | (color << 8) | color; // little-endian RGBA
    }
};

} // namespace boyboy::ppu