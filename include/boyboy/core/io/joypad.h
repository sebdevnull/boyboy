/**
 * @file joypad.h
 * @brief Joypad (Gamepad) input handling for BoyBoy emulator.
 *
 * Joypad state is represented in the P1 register (0xFF00).
 * Bits 0-3 represent the state of the buttons (0 = pressed, 1 = released).
 * Bits 4-5 select the button group (directional or action buttons).
 * Bits 6-7 are unused and always read as 1.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstdint>

#include "boyboy/core/io/buttons.h"
#include "boyboy/core/io/iocomponent.h"

namespace boyboy::core::io {

class Joypad : public IoComponent {
public:
    // IoComponent interface
    void init() override;
    void reset() override;
    void tick(uint16_t cycles) override;
    [[nodiscard]] uint8_t read(uint16_t addr) const override;
    void write(uint16_t addr, uint8_t value) override;
    void set_interrupt_cb(cpu::InterruptRequestCallback callback) override;

    // Button operations
    void press(Button button);
    void release(Button button);
    [[nodiscard]] bool is_pressed(Button button) const;

private:
    uint8_t select_ = ButtonMask::SelectMask; // Currently selected button group
    uint8_t buttons_ = 0xFF; // Upper 4 bits: d-pad, lower 4 bits: action (0=pressed, 1=released)

    cpu::InterruptRequestCallback request_interrupt_;

    // Helpers for reading P1 register and button masks
    [[nodiscard]] inline uint8_t p1() const;
    [[nodiscard]] static uint8_t button_mask(Button button);
    [[nodiscard]] bool any_button_pressed() const { return buttons_ != 0xFF; }
};

} // namespace boyboy::core::io