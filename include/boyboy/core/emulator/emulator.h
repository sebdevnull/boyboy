/**
 * @file emulator.h
 * @brief Main emulator class for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <memory>
#include <string>

#include "boyboy/core/cartridge/cartridge.h"
#include "boyboy/core/cpu/cpu.h"
#include "boyboy/core/display/display.h"
#include "boyboy/core/io/io.h"
#include "boyboy/core/io/joypad.h"
#include "boyboy/core/mmu/mmu.h"
#include "boyboy/core/ppu/ppu.h"

// Forward declaration
namespace boyboy::common::config {
struct Config;
}

namespace boyboy::core::emulator {

class Emulator {
public:
    Emulator() : mmu_(std::make_shared<mmu::Mmu>()), cpu_(mmu_) {}

    // Emulator operations
    void load(const std::string& path);
    void start();
    void stop();
    void run();
    void reset();

    // State accessors
    [[nodiscard]] bool is_running() const { return running_; }
    [[nodiscard]] bool is_started() const { return started_; }
    void limit_frame_rate(bool limit) { frame_rate_limited_ = limit; }
    [[nodiscard]] bool is_frame_rate_limited() const { return frame_rate_limited_; }

    // Configuration
    void apply_config(const common::config::Config& config);

    // Button event handler
    void on_button_event(io::Button button, bool pressed);

private:
    // System components
    std::shared_ptr<mmu::Mmu> mmu_;
    cpu::Cpu cpu_;
    io::Io& io_ = mmu_->io();
    ppu::Ppu& ppu_ = io_.ppu();
    io::Joypad& joypad_ = io_.joypad();
    display::Display display_;
    std::unique_ptr<cartridge::Cartridge> cartridge_;

    // Emulator state
    bool running_ = false;
    bool started_ = false;
    bool frame_rate_limited_ = true;
    int speed_ = 1;

    // Statistics
    uint64_t instruction_count_ = 0;
    uint64_t cycle_count_ = 0;

    void emulate_frame();
    void render_frame();
};

} // namespace boyboy::core::emulator