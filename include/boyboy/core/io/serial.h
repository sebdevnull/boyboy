/**
 * @file serial.h
 * @brief Serial I/O operations for BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <iostream>
#include <ostream>

#include "boyboy/core/io/iocomponent.h"

namespace boyboy::core::io {

class Serial : public IoComponent {
public:
    Serial(std::ostream& out = std::cout) : serial_out_(&out) {}

    void tick(uint16_t cycles) override;
    [[nodiscard]] uint8_t read(uint16_t addr) const override;
    void write(uint16_t addr, uint8_t value) override;
    void set_interrupt_cb(cpu::InterruptRequestCallback callback) override;
    void reset() override;

    void set_output_stream(std::ostream& out) { serial_out_ = &out; }

private:
    std::ostream* serial_out_;
    cpu::InterruptRequestCallback request_interrupt_;
};

} // namespace boyboy::core::io