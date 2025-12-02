/**
 * @file apu.h
 * @brief APU (Audio Processing Unit) for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include "boyboy/core/io/iocomponent.h"
#include "boyboy/core/io/registers.h"

namespace boyboy::core::io {

class Apu : public IoComponent {
public:
    // IoComponent interface
    void init() override;
    void reset() override;
    void tick(uint16_t cycles) override;
    [[nodiscard]] uint8_t read(uint16_t addr) const override;
    void write(uint16_t addr, uint8_t value) override;
    void set_interrupt_cb(cpu::InterruptRequestCallback callback) override;

private:
    cpu::InterruptRequestCallback request_interrupt_;
    std::array<uint8_t, io::IoReg::Apu::Size> registers_{};
};

} // namespace boyboy::core::io