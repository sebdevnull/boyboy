/**
 * @file emulator.h
 * @brief Main emulator class for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <memory>
#include <string>

#include "boyboy/cartridge.h"
#include "boyboy/cpu/cpu.h"
#include "boyboy/display.h"
#include "boyboy/mmu.h"

namespace boyboy::emulator {

class Emulator {
public:
    Emulator() : mmu_(std::make_shared<mmu::Mmu>()), cpu_(mmu_) {}

    void load_cartridge(const std::string& path);
    
    void step();
    void run();

    void reset();

    [[nodiscard]] const display::Display& get_display() const { return display_; }

private:
    std::shared_ptr<mmu::Mmu> mmu_;
    cpu::Cpu cpu_;
    display::Display display_;
    cartridge::Cartridge cartridge_;
};

} // namespace boyboy