/**
 * @file display.h
 * @brief Display handling for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

namespace boyboy::display {

class Display {
public:
    void init();

private:
    static constexpr int Width = 160;
    static constexpr int Height = 144;
};

} // namespace boyboy::display