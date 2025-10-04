/**
 * @file emulator.h
 * @brief Main emulator class for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <memory>
#include <string>

#include "boyboy/cart/cartridge.h"
#include "boyboy/cpu/cpu.h"
#include "boyboy/display.h"
#include "boyboy/io/io.h"
#include "boyboy/io/joypad.h"
#include "boyboy/mmu/mmu.h"
#include "boyboy/ppu/ppu.h"

namespace boyboy::emulator {

class Emulator {
public:
    Emulator() : mmu_(std::make_shared<mmu::Mmu>()), cpu_(mmu_) {}

    // Emulator operations
    void load(const std::string& path);
    void step();
    void run();
    void reset();

    // Button event handler
    void on_button_event(io::Button button, bool pressed);

    [[nodiscard]] const display::Display& get_display() const { return display_; }

private:
    // System components
    std::shared_ptr<mmu::Mmu> mmu_;
    cpu::Cpu cpu_;
    io::Io& io_ = mmu_->io();
    ppu::Ppu& ppu_ = io_.ppu();
    io::Joypad& joypad_ = io_.joypad();
    display::Display display_;
    cart::Cartridge cartridge_;

    // Emulator state
    bool running_ = false;

    // Statistics
    double frame_count_ = 0;
    double instruction_count_ = 0;
    double cycle_count_ = 0;
};

} // namespace boyboy::emulator