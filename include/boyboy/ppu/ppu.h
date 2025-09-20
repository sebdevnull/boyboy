/**
 * @file ppu.h
 * @brief PPU (Pixel Processing Unit) for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstdint>
#include <functional>
#include <ostream>

#include "boyboy/io/iocomponent.h"
#include "boyboy/io/registers.h"
#include "boyboy/ppu/registers.h"

namespace boyboy::ppu {

// Screen dimensions
static constexpr int LCDWidth = 160;
static constexpr int LCDHeight = 144;
static constexpr int FramebufferSize = LCDWidth * LCDHeight;

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
static constexpr int VisibleScanlines = LCDHeight;
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
using MemoryReadCB = std::function<uint8_t(uint16_t)>;

// Simple grayscale palette (white to black)
static constexpr std::array<Pixel, 4> Palette = {
    0xFFFFFFFF, // White
    0xFFAAAAAA, // Light gray
    0xFF555555, // Dark gray
    0xFF000000  // Black
};

// Green-scale palette (light green to dark green)
// static constexpr std::array<Pixel, 4> Palette = {
//     0xFFE0F8D0, // Light green
//     0xFF88C070, // Medium light green
//     0xFF346856, // Medium dark green
//     0xFF081820  // Dark green
// };

class Ppu : public io::IoComponent {
public:
    Ppu() { reset(); }

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

    // Memory read callback for PPU to access VRAM/OAM through MMU
    void set_mem_read_cb(MemoryReadCB cb) { mem_read_ = std::move(cb); }

    // Get color from palette
    [[nodiscard]] static Pixel palette_color(uint8_t color_id, uint8_t palette)
    {
        uint8_t color = (palette >> (color_id * 2)) & 0x3;
        return Palette.at(color);
    }

    void test_framebuffer();

private:
    MemoryReadCB mem_read_;

    // PPU state
    Mode mode_ = Mode::OAMScan;
    Mode previous_mode_ = mode_;
    uint8_t previous_ly_ = 0;
    int cycles_ = 0;
    int cycles_in_mode_ = 0;
    uint8_t window_line_counter_ = 0;

    // Frame management
    uint64_t frame_count_ = 0;
    bool frame_ready_ = false;
    FrameBuffer framebuffer_{};

    // I/O registers
    // Using array for easy reset and direct access
    // Frequently accessed registers have references to avoid repeated lookups
    std::array<uint8_t, io::IoReg::Ppu::Size> registers_{};
    uint8_t& LCDC_ = registers_.at(io::IoReg::Ppu::local_addr(io::IoReg::Ppu::LCDC));
    uint8_t& STAT_ = registers_.at(io::IoReg::Ppu::local_addr(io::IoReg::Ppu::STAT));
    uint8_t& SCY_ = registers_.at(io::IoReg::Ppu::local_addr(io::IoReg::Ppu::SCY));
    uint8_t& SCX_ = registers_.at(io::IoReg::Ppu::local_addr(io::IoReg::Ppu::SCX));
    uint8_t& LY_ = registers_.at(io::IoReg::Ppu::local_addr(io::IoReg::Ppu::LY));
    uint8_t& LYC_ = registers_.at(io::IoReg::Ppu::local_addr(io::IoReg::Ppu::LYC));
    uint8_t& BGP_ = registers_.at(io::IoReg::Ppu::local_addr(io::IoReg::Ppu::BGP));
    uint8_t& WY_ = registers_.at(io::IoReg::Ppu::local_addr(io::IoReg::Ppu::WY));
    uint8_t& WX_ = registers_.at(io::IoReg::Ppu::local_addr(io::IoReg::Ppu::WX));

    cpu::InterruptRequestCallback request_interrupt_;

    void switch_mode(Mode new_mode);

    // Rendering
    void render_scanline();
    void render_background();
    void render_window();
    void render_sprites();

    // Interrupt handling
    void check_interrupts();
    void request_interrupt(uint8_t interrupt);

    [[nodiscard]] uint8_t mem_read(uint16_t addr) const
    {
        if (mem_read_) {
            return mem_read_(addr);
        }
        return 0xFF;
    }

    // Helpers to check LCDC flags
    [[nodiscard]] bool bg_enabled() const
    {
        return (LCDC_ & registers::LCDC::BGAndWindowEnable) != 0;
    }
    [[nodiscard]] bool window_enabled() const
    {
        return bg_enabled() && (LCDC_ & registers::LCDC::WindowEnable) != 0;
    }
    [[nodiscard]] bool sprites_enabled() const { return (LCDC_ & registers::LCDC::OBJEnable) != 0; }
    [[nodiscard]] bool large_sprites() const { return (LCDC_ & registers::LCDC::OBJSize) != 0; }

    // Helpers to get addresses based on LCDC settings
    [[nodiscard]] uint16_t bg_tile_map_addr() const
    {
        return (LCDC_ & registers::LCDC::BGTileMapArea) != 0 ? registers::LCDC::BGTileMapArea1
                                                             : registers::LCDC::BGTileMapArea0;
    }
    [[nodiscard]] uint16_t window_tile_map_addr() const
    {
        return (LCDC_ & registers::LCDC::WindowTileMap) != 0 ? registers::LCDC::WindowTileMapArea1
                                                             : registers::LCDC::WindowTileMapArea0;
    }
    [[nodiscard]] uint16_t bg_window_tile_data_addr() const
    {
        return (LCDC_ & registers::LCDC::BGAndWindowTileData) != 0
                   ? registers::LCDC::BGAndWindowTileData1
                   : registers::LCDC::BGAndWindowTileData0;
    }

    [[nodiscard]] static Pixel to_rgba(uint8_t color)
    {
        return 0xFF000000 | (color << 16) | (color << 8) | color; // little-endian RGBA
    }
};

} // namespace boyboy::ppu