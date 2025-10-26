/**
 * @file emulator.h
 * @brief Main emulator class for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <memory>
#include <string>

// Core components forward declarations
namespace boyboy::core {
namespace cpu {
class Cpu;
}
namespace mmu {
class Mmu;
}
namespace ppu {
class Ppu;
}
namespace io {
class Io;
class Timer;
class Joypad;
class Serial;
enum class Button : uint8_t;
} // namespace io
namespace display {
class Display;
}
namespace cartridge {
class Cartridge;
}
} // namespace boyboy::core

// Config forward declaration
namespace boyboy::common::config {
struct Config;
}

namespace boyboy::core::emulator {

class Emulator {
public:
    Emulator();
    ~Emulator();
    Emulator(Emulator&) = delete;
    Emulator(Emulator&&) = delete;
    Emulator& operator=(Emulator&) = delete;
    Emulator& operator=(Emulator&&) = delete;

    // Emulator operations
    void init();
    void reset();
    void load(const std::string& path);
    void start();
    void stop();
    void run();

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
    std::shared_ptr<io::Io> io_;
    std::shared_ptr<mmu::Mmu> mmu_;
    std::shared_ptr<cpu::Cpu> cpu_;
    std::shared_ptr<ppu::Ppu> ppu_;
    std::shared_ptr<io::Timer> timer_;
    std::shared_ptr<io::Joypad> joypad_;
    std::shared_ptr<io::Serial> serial_;
    std::shared_ptr<display::Display> display_;
    std::unique_ptr<cartridge::Cartridge> cartridge_;

    // Emulator state
    bool running_ = false;
    bool started_ = false;
    bool frame_rate_limited_ = true;
    int speed_ = 1;

    // Statistics
    uint64_t instruction_count_ = 0;
    uint64_t cycle_count_ = 0;

    // Emulation methods
    void emulate_frame();
    void render_frame();
};

} // namespace boyboy::core::emulator