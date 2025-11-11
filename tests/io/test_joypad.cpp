/**
 * @file test_joypad.cpp
 * @brief Unit tests for I/O Joypad.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include <cstdint>

#include "boyboy/core/cpu/interrupts.h"
#include "boyboy/core/io/buttons.h"
#include "boyboy/core/io/joypad.h"
#include "boyboy/core/io/registers.h"

using boyboy::core::io::Button;
using boyboy::core::io::ButtonMask;
using boyboy::core::io::IoReg;
using boyboy::core::io::Joypad;
using boyboy::core::io::to_string;

class IoJoypadTest : public ::testing::Test {
protected:
    void SetUp() override { joypad_.reset(); }

    Joypad joypad_;

    // Helpers to access P1 register
    [[nodiscard]] uint8_t read_p1() const { return joypad_.read(IoReg::Joypad::P1); }
    void write_p1(uint8_t value) { joypad_.write(IoReg::Joypad::P1, value); }

    // Assert helpers
    void assert_select(uint8_t select)
    {
        uint8_t p1 = read_p1();
        EXPECT_EQ(p1 & ButtonMask::SelectMask, select & ButtonMask::SelectMask)
            << "Select bits mismatch";
    }

    // Assert button state and corresponding P1 bit
    // pressed = expected logical state (true=pressed, false=released)
    // state = expected P1 bit state depending on selection (true=pressed (0), false=released (1))
    void assert_button(Button button, bool pressed, bool state, const std::string& msg = "")
    {
        bool is_pressed = joypad_.is_pressed(button);
        EXPECT_EQ(is_pressed, pressed) << "Button " << to_string(button) << " state mismatch"
                                       << (msg.empty() ? "" : " (" + msg + ")");

        uint8_t p1 = read_p1();
        EXPECT_EQ((p1 & ButtonMask::get_mask(button)) == 0, state)
            << "P1 register button bit mismatch for " << to_string(button)
            << (msg.empty() ? "" : " (" + msg + ")");
    }
};

TEST_F(IoJoypadTest, InitialState)
{
    // Initial state: no buttons pressed, select action buttons
    // Bits 4-5 set (no group selected), bits 0-3 high (not pressed)
    EXPECT_EQ(joypad_.read(0xFF00), 0xFF);
}

TEST_F(IoJoypadTest, ResetState)
{
    // Set some initial state
    write_p1(~ButtonMask::SelectAction); // Select action buttons
    assert_select(~ButtonMask::SelectAction);
    EXPECT_NE(read_p1(), 0xFF);

    // Press some buttons
    joypad_.press(Button::A);
    joypad_.press(Button::B);
    assert_button(Button::A, true, true);
    assert_button(Button::B, true, true);

    // Reset joypad
    joypad_.reset();

    // All buttons should be released
    assert_button(Button::A, false, false);
    assert_button(Button::Right, false, false);
    assert_button(Button::B, false, false);
    assert_button(Button::Select, false, false);
    assert_button(Button::Start, false, false);
    assert_button(Button::Left, false, false);
    assert_button(Button::Up, false, false);
    assert_button(Button::Down, false, false);

    // P1 register should be back to initial state
    EXPECT_EQ(read_p1(), 0xFF);
}

TEST_F(IoJoypadTest, SelectActionButtons)
{
    // Select action buttons
    write_p1(~ButtonMask::SelectAction);
    assert_select(~ButtonMask::SelectAction);
    EXPECT_EQ(read_p1() & ButtonMask::AllButtons, ButtonMask::AllButtons); // No buttons pressed

    // Press A button
    joypad_.press(Button::A);
    assert_button(Button::A, true, true);
    assert_button(Button::B, false, false);
    assert_button(Button::Select, false, false);
    assert_button(Button::Start, false, false);
    assert_button(Button::Right, false, true, "Shares mask with A");
    assert_button(Button::Left, false, false);
    assert_button(Button::Up, false, false);
    assert_button(Button::Down, false, false);

    // Press B button
    joypad_.press(Button::B);
    assert_button(Button::A, true, true);
    assert_button(Button::B, true, true);
    assert_button(Button::Right, false, true, "Still pressed because A is pressed");
    assert_button(Button::Left, false, true, "Shares mask with B");

    // Release A button
    joypad_.release(Button::A);
    assert_button(Button::A, false, false);
    assert_button(Button::B, true, true);
    assert_button(Button::Right, false, false, "Released because A is released");
    assert_button(Button::Left, false, true, "Still pressed because B is pressed");

    // Release B button
    joypad_.release(Button::B);
    assert_button(Button::A, false, false);
    assert_button(Button::B, false, false);
    assert_button(Button::Right, false, false);
    assert_button(Button::Left, false, false, "Released because B is released");
}

TEST_F(IoJoypadTest, SelectDirectionalButtons)
{
    // Select directional buttons
    write_p1(~ButtonMask::SelectDPad);
    assert_select(~ButtonMask::SelectDPad);
    EXPECT_EQ(read_p1() & ButtonMask::AllButtons, ButtonMask::AllButtons); // No buttons pressed

    // Press Right button
    joypad_.press(Button::Right);
    assert_button(Button::Right, true, true);
    assert_button(Button::Left, false, false);
    assert_button(Button::Up, false, false);
    assert_button(Button::Down, false, false);
    assert_button(Button::A, false, true, "Shares mask with Right");
    assert_button(Button::B, false, false);
    assert_button(Button::Select, false, false);
    assert_button(Button::Start, false, false);

    // Press Down button
    joypad_.press(Button::Down);
    assert_button(Button::Right, true, true);
    assert_button(Button::Down, true, true);
    assert_button(Button::A, false, true, "Still pressed because Right is pressed");
    assert_button(Button::Start, false, true, "Shares mask with Down");

    // Release Right button
    joypad_.release(Button::Right);
    assert_button(Button::Right, false, false);
    assert_button(Button::Down, true, true);
    assert_button(Button::A, false, false, "Released because Right is released");
    assert_button(Button::Start, false, true, "Still pressed because Down is pressed");

    // Release Down button
    joypad_.release(Button::Down);
    assert_button(Button::Right, false, false);
    assert_button(Button::Down, false, false);
    assert_button(Button::A, false, false);
    assert_button(Button::Start, false, false, "Released because Down is released");
}

TEST_F(IoJoypadTest, SelectBothGroups)
{
    // Select both groups (bits 4-5 low)
    write_p1(~ButtonMask::SelectMask);
    assert_select(~ButtonMask::SelectMask);
    EXPECT_EQ(read_p1() & ButtonMask::AllButtons, ButtonMask::AllButtons); // No buttons pressed

    // Press A button
    joypad_.press(Button::A);
    assert_button(Button::A, true, true);
    assert_button(Button::Right, false, true, "Shares mask with A");
    assert_button(Button::B, false, false);
    assert_button(Button::Select, false, false);
    assert_button(Button::Start, false, false);
    assert_button(Button::Left, false, false);
    assert_button(Button::Up, false, false);
    assert_button(Button::Down, false, false);
    EXPECT_EQ((read_p1() & ButtonMask::AllButtons) | ~ButtonMask::AllButtons, ~ButtonMask::A)
        << "Only A pressed";

    // Release Right button, shouldn't affect
    joypad_.release(Button::Right);
    assert_button(Button::A, true, true);
    assert_button(Button::Right, false, true, "Still pressed because A is pressed");

    // Press Left button
    joypad_.press(Button::Left);
    assert_button(Button::A, true, true);
    assert_button(Button::B, false, true, "Shares mask with Left");
    assert_button(Button::Right, false, true, "Still pressed because A is pressed");
    assert_button(Button::Left, true, true);
    EXPECT_EQ(
        (read_p1() & ButtonMask::AllButtons) | ~ButtonMask::AllButtons,
        ~(ButtonMask::A | ButtonMask::Left)
    ) << " A and Left pressed";

    // Release A button
    joypad_.release(Button::A);
    assert_button(Button::A, false, false);
    assert_button(Button::Right, false, false, "Released because A is released");
    assert_button(Button::B, false, true, "Still pressed because Left is pressed");
    assert_button(Button::Left, true, true);
    EXPECT_EQ((read_p1() & ButtonMask::AllButtons) | ~ButtonMask::AllButtons, ~ButtonMask::Left)
        << "Only Left pressed";

    // Release Left button
    joypad_.release(Button::Left);
    assert_button(Button::A, false, false);
    assert_button(Button::B, false, false, "Released because Left is released");
    assert_button(Button::Right, false, false);
    assert_button(Button::Left, false, false);
    EXPECT_EQ(read_p1() & ButtonMask::AllButtons, ButtonMask::AllButtons) << "No buttons pressed";
}

TEST_F(IoJoypadTest, SelectNoGroup)
{
    // Select no group (bits 4-5 high)
    write_p1(ButtonMask::SelectMask);
    assert_select(ButtonMask::SelectMask);
    EXPECT_EQ(read_p1() & ButtonMask::AllButtons, ButtonMask::AllButtons); // No buttons pressed

    // Press A button
    joypad_.press(Button::A);
    assert_button(Button::A, true, false, "Pressed but not reflected in P1");
    assert_button(Button::B, false, false);
    assert_button(Button::Select, false, false);
    assert_button(Button::Start, false, false);
    assert_button(Button::Right, false, false);
    assert_button(Button::Left, false, false);
    assert_button(Button::Up, false, false);
    assert_button(Button::Down, false, false);

    EXPECT_EQ(read_p1() & ButtonMask::AllButtons, ButtonMask::AllButtons)
        << "No buttons should be reflected";

    // Release A button
    joypad_.release(Button::A);
    assert_button(Button::A, false, false);
}

TEST_F(IoJoypadTest, InvalidButtonHandling)
{
    // Invalid button (not in enum) should be handled gracefully
    auto invalid_button = static_cast<Button>(0xFF);
    joypad_.press(invalid_button); // Should log a warning but not crash
    joypad_.release(invalid_button);
    EXPECT_FALSE(joypad_.is_pressed(invalid_button));
}

TEST_F(IoJoypadTest, InterruptOnPress)
{
    uint8_t irq_count = 0;
    joypad_.set_interrupt_cb([&](boyboy::core::cpu::Interrupt) { irq_count++; });

    // Select action buttons
    write_p1(~ButtonMask::SelectAction);
    assert_select(~ButtonMask::SelectAction);
    EXPECT_EQ(read_p1() & ButtonMask::AllButtons, ButtonMask::AllButtons); // No buttons pressed

    // Press A button, should trigger interrupt
    joypad_.press(Button::A);
    assert_button(Button::A, true, true);
    EXPECT_EQ(irq_count, 1) << "Interrupt should be triggered on button press";

    // Press A button again, should not trigger another interrupt
    joypad_.press(Button::A);
    assert_button(Button::A, true, true);
    EXPECT_EQ(irq_count, 1) << "Interrupt should not be triggered again on repeated press";

    // Press B button, should not trigger another interrupt because A is still pressed
    joypad_.press(Button::B);
    assert_button(Button::B, true, true);
    EXPECT_EQ(
        irq_count, 1
    ) << "Interrupt should not be triggered when another button is pressed while one is held";

    // Reset for next test
    irq_count = 0;

    // Release A and B buttons, should not trigger interrupt
    joypad_.release(Button::A);
    joypad_.release(Button::B);
    assert_button(Button::A, false, false);
    assert_button(Button::B, false, false);
    EXPECT_FALSE(irq_count) << "Interrupt should not be triggered on button release";
}

TEST_F(IoJoypadTest, InterruptOnMultiplePresses)
{
    uint8_t irq_count = 0;
    joypad_.set_interrupt_cb([&](boyboy::core::cpu::Interrupt) { irq_count++; });

    // Select directional buttons
    write_p1(~ButtonMask::SelectDPad);
    assert_select(~ButtonMask::SelectDPad);
    EXPECT_EQ(read_p1() & ButtonMask::AllButtons, ButtonMask::AllButtons); // No buttons pressed

    // Press Right button, should trigger interrupt
    joypad_.press(Button::Right);
    assert_button(Button::Right, true, true);
    EXPECT_EQ(irq_count, 1) << "Interrupt should be triggered on button press";

    // Release Right button
    joypad_.release(Button::Right);
    assert_button(Button::Right, false, false);

    // Press Right button again, should trigger another interrupt
    joypad_.press(Button::Right);
    assert_button(Button::Right, true, true);
    EXPECT_EQ(irq_count, 2) << "Interrupt should be triggered again on new button press";

    // Press more buttons while Right is still pressed, should not trigger interrupts
    joypad_.press(Button::Down);
    joypad_.press(Button::Left);
    joypad_.press(Button::A);
    assert_button(Button::Left, true, true);
    assert_button(Button::Down, true, true);
    assert_button(Button::A, true, true);
    EXPECT_EQ(
        irq_count, 2
    ) << "Interrupt should not be triggered when another button is pressed while one is held";

    // Release both buttons
    joypad_.release(Button::Right);
    joypad_.release(Button::Down);
    assert_button(Button::Right, false, false);
    assert_button(Button::Down, false, false);
}

TEST_F(IoJoypadTest, InterruptMixedSelect)
{
    uint8_t irq_count = 0;
    joypad_.set_interrupt_cb([&](boyboy::core::cpu::Interrupt) { irq_count++; });

    // Select action buttons
    write_p1(~ButtonMask::SelectAction);
    assert_select(~ButtonMask::SelectAction);
    EXPECT_EQ(read_p1() & ButtonMask::AllButtons, ButtonMask::AllButtons); // No buttons pressed

    // Press A button, should trigger interrupt
    joypad_.press(Button::A);
    assert_button(Button::A, true, true);
    EXPECT_EQ(irq_count, 1) << "Interrupt should be triggered on button press";

    // Release A button
    joypad_.release(Button::A);
    assert_button(Button::A, false, false);

    // Press Right button, should trigger another interrupt
    joypad_.press(Button::Right);
    assert_button(Button::Right, true, false, "Action buttons selected, Right not reflected");
    EXPECT_EQ(irq_count, 2) << "Interrupt should be triggered on new button press";

    // Release Right button
    joypad_.release(Button::Right);
    assert_button(Button::Right, false, false);

    // Change selection to directional buttons
    write_p1(~ButtonMask::SelectDPad);
    assert_select(~ButtonMask::SelectDPad);
    EXPECT_EQ(read_p1() & ButtonMask::AllButtons, ButtonMask::AllButtons); // No buttons pressed

    // Press B button, should trigger another interrupt
    joypad_.press(Button::B);
    assert_button(Button::B, true, false, "Directional buttons selected, B not reflected");
    EXPECT_EQ(irq_count, 3) << "Interrupt should be triggered on new button press";

    // Release B button
    joypad_.release(Button::B);
    assert_button(Button::B, false, false);

    // Change selection to both groups
    write_p1(~ButtonMask::SelectMask);
    assert_select(~ButtonMask::SelectMask);
    EXPECT_EQ(read_p1() & ButtonMask::AllButtons, ButtonMask::AllButtons); // No buttons pressed

    irq_count = 0; // Reset count

    // Press B button, should trigger interrupt
    joypad_.press(Button::B);
    assert_button(Button::B, true, true);
    EXPECT_EQ(
        irq_count, 1
    ) << "Interrupt should be triggered on button press with both groups selected";

    // Press Right button while B is still pressed, should not trigger another interrupt
    joypad_.press(Button::Right);
    assert_button(Button::Right, true, true);
    EXPECT_EQ(
        irq_count, 1
    ) << "Interrupt should not be triggered when another button is pressed while one is held";

    // Release Right and B buttons
    joypad_.release(Button::Right);
    joypad_.release(Button::B);
    assert_button(Button::Right, false, false);
    assert_button(Button::B, false, false);
}