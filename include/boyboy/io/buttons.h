/**
 * @file buttons.h
 * @brief Button definitions for BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstdint>
#include <string>

namespace boyboy::io {

// Button definitions
enum class Button : uint8_t {
    A,
    B,
    Select,
    Start,
    Right,
    Left,
    Up,
    Down,
};

inline static bool is_action(Button button)
{
    return button >= Button::A && button <= Button::Start;
}

inline static bool is_directional(Button button)
{
    return button >= Button::Right && button <= Button::Down;
}

// Button string conversion for logging
std::string to_string(Button button);
std::ostream& operator<<(std::ostream& os, Button button);

// Button bitmasks for P1 register (0xFF00)
struct ButtonMask {
    // Action buttons
    static constexpr uint8_t A = 0b00000001;      // Bit 0 - A Button
    static constexpr uint8_t B = 0b00000010;      // Bit 1 - B Button
    static constexpr uint8_t Select = 0b00000100; // Bit 2 - Select Button
    static constexpr uint8_t Start = 0b00001000;  // Bit 3 - Start Button

    // Directional buttons (share the same bits as action buttons)
    static constexpr uint8_t Right = A;    // Bit 0 - Right Button
    static constexpr uint8_t Left = B;     // Bit 1 - Left Button
    static constexpr uint8_t Up = Select;  // Bit 2 - Up Button
    static constexpr uint8_t Down = Start; // Bit 3 - Down Button

    // Button select bits
    static constexpr uint8_t SelectAction = 0b00100000; // Bit 5
    static constexpr uint8_t SelectDPad = 0b00010000;   // Bit 4

    static constexpr uint8_t AllButtons = 0b00001111; // Bits 0-3
    static constexpr uint8_t SelectMask = 0b00110000; // Bits 4-5

    // Get the bitmask for a specific button
    // Returns 0 if the button is unknown
    static uint8_t get_mask(Button button)
    {
        switch (button) {
            case Button::A:
                return A;
            case Button::B:
                return B;
            case Button::Select:
                return Select;
            case Button::Start:
                return Start;
            case Button::Right:
                return Right;
            case Button::Left:
                return Left;
            case Button::Up:
                return Up;
            case Button::Down:
                return Down;
            default:
                return 0;
        }
    }
};

} // namespace boyboy::io