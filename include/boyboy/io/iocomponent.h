/**
 * @file iocomponent.h
 * @brief I/O component interface for BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstdint>

#include "boyboy/cpu/interrupts.h"

namespace boyboy::io {

class IoComponent {
public:
    virtual ~IoComponent() = default;

    // Called every N CPU cycles to update the component state
    virtual void tick(uint16_t cycles) = 0;

    // Read/write I/O registers
    [[nodiscard]] virtual uint8_t read(uint16_t addr) const = 0;
    virtual void write(uint16_t addr, uint8_t value) = 0;

    // Set callback to request CPU interrupts
    virtual void set_interrupt_cb(cpu::InterruptRequestCallback callback) = 0;

    // Delete move and copy
    IoComponent(const IoComponent&) = delete;
    IoComponent& operator=(const IoComponent&) = delete;
    IoComponent(IoComponent&&) = delete;
    IoComponent& operator=(IoComponent&&) = delete;

protected:
    IoComponent() = default;
};

} // namespace boyboy::io