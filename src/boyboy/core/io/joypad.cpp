/**
 * @file joypad.cpp
 * @brief Joypad (Gamepad) input handling for BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/core/io/joypad.h"

#include <cstdint>

#include "boyboy/common/log/logging.h"
#include "boyboy/common/utils.h"
#include "boyboy/core/io/buttons.h"
#include "boyboy/core/io/registers.h"

namespace boyboy::core::io {

using namespace boyboy::common;

void Joypad::tick(uint16_t /*cycles*/)
{
    // Joypad does not need to do anything on tick
}

[[nodiscard]] uint8_t Joypad::read(uint16_t addr) const
{
    if (!IoReg::Joypad::contains(addr)) {
        log::warn(
            "Joypad read from invalid address: {}", common::utils::PrettyHex(addr).to_string()
        );
        return 0xFF; // Open bus
    }
    return p1();
}

void Joypad::write(uint16_t addr, uint8_t value)
{
    if (!IoReg::Joypad::contains(addr)) {
        log::warn(
            "Joypad write to invalid address: {}", common::utils::PrettyHex(addr).to_string()
        );
        return;
    }

    select_ = value & ButtonMask::SelectMask;

    log::trace(
        "Joypad Write: {} <- {}, P1={}",
        common::utils::PrettyHex(addr).to_string(),
        common::utils::PrettyHex(value).to_string(),
        common::utils::PrettyHex(p1()).to_string()
    );
}

void Joypad::set_interrupt_cb(cpu::InterruptRequestCallback callback)
{
    request_interrupt_ = std::move(callback);
}

void Joypad::reset()
{
    select_ = ButtonMask::SelectMask; // Neither group selected
    buttons_ = 0xFF;                  // All buttons released (1)
}

void Joypad::press(Button button)
{
    uint8_t mask = button_mask(button);
    if (mask == 0) {
        log::warn("Unknown button pressed: {}", static_cast<int>(button));
        return;
    }

    if ((buttons_ & mask) == 0) {
        log::trace(
            "Button already pressed: {}, P1={}",
            to_string(button),
            common::utils::PrettyHex(p1()).to_string()
        );
        return;
    }

    bool was_any_pressed = any_button_pressed(); // Check before changing state for IRQ logic

    buttons_ &= ~mask; // Set to 0 (pressed)

    log::debug(
        "Button Pressed: {}, P1={}", to_string(button), common::utils::PrettyHex(p1()).to_string()
    );

    // Trigger interrupt if any button was not previously pressed and one or two groups are selected
    if (request_interrupt_ && !was_any_pressed &&
        (select_ & ButtonMask::SelectMask) != ButtonMask::SelectMask) {
        request_interrupt_(cpu::Interrupts::Joypad);
        log::debug("Joypad interrupt requested: {} pressed", to_string(button));
    }
}

void Joypad::release(Button button)
{
    uint8_t mask = button_mask(button);
    if (mask == 0) {
        log::warn("Unknown button released: {}", static_cast<int>(button));
        return;
    }

    if ((buttons_ & mask) != 0) {
        log::debug(
            "Button already released: {}, P1={}",
            to_string(button),
            common::utils::PrettyHex(p1()).to_string()
        );
        return;
    }

    buttons_ |= mask; // Set to 1 (released)

    log::debug(
        "Button Released: {}, P1={}", to_string(button), common::utils::PrettyHex(p1()).to_string()
    );
}

[[nodiscard]] bool Joypad::is_pressed(Button button) const
{
    uint8_t mask = button_mask(button);
    if (mask == 0) {
        log::warn("Unknown button state queried: {}", static_cast<int>(button));
        return false;
    }

    return (buttons_ & mask) == 0; // 0 = pressed, 1 = released
}

[[nodiscard]] uint8_t Joypad::p1() const
{
    // Compose P1 register value
    // Bits 0-3: button states (0=pressed, 1=released)
    // Bits 4-5: button group selection
    // Bits 6-7: always 1

    // Bits 6-7 set to 1
    // Start with all buttons released
    uint8_t p1 = select_ | 0b11000000 | ButtonMask::AllButtons;

    if ((select_ & ButtonMask::SelectMask) == 0) {
        // Both groups selected, bits 0-3 reflect both groups (ANDed)
        p1 &= (0xF0 | ((buttons_ & ButtonMask::AllButtons) & (buttons_ >> 4)));
    }
    else if ((select_ & ButtonMask::SelectAction) == 0) {
        // Action buttons selected
        p1 &= (0xF0 | (buttons_ & ButtonMask::AllButtons));
    }
    else if ((select_ & ButtonMask::SelectDPad) == 0) {
        // Directional buttons selected
        p1 &= (0xF0 | buttons_ >> 4);
    }
    // Else no group selected, bits 0-3 remain high (no buttons pressed)

    return p1;
}

[[nodiscard]] uint8_t Joypad::button_mask(Button button)
{
    uint8_t mask = ButtonMask::get_mask(button);
    if (mask == 0) {
        log::warn("Unknown button for mask: {}", static_cast<int>(button));
    }

    if (is_directional(button)) {
        mask <<= 4; // Shift to bits 4-7
    }

    return mask;
}

// Button string conversion for logging
inline std::string to_string(Button button)
{
    switch (button) {
        case Button::A:
            return "A";
        case Button::B:
            return "B";
        case Button::Select:
            return "Select";
        case Button::Start:
            return "Start";
        case Button::Right:
            return "Right";
        case Button::Left:
            return "Left";
        case Button::Up:
            return "Up";
        case Button::Down:
            return "Down";
        default:
            return "Unknown";
    }
}
inline std::ostream& operator<<(std::ostream& os, Button button)
{
    return os << to_string(button);
}

} // namespace boyboy::core::io