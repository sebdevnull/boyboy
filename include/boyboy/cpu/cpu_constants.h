/**
 * @file cpu_constants.h
 * @brief
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstdint>

namespace boyboy::cpu {

constexpr uint16_t PCStartValue = 0x0100;
constexpr uint16_t SPStartValue = 0xFFFE;

constexpr uint8_t CBInstructionPrefix = 0xCB;

constexpr uint16_t HighRAMOffset = 0xFF00;

constexpr uint8_t VBlankVector = 0x40;
constexpr uint8_t LCDStatVector = 0x48;
constexpr uint8_t TimerVector = 0x50;
constexpr uint8_t SerialVector = 0x58;
constexpr uint8_t JoypadVector = 0x60;

constexpr uint8_t VBlankInterrupt = (1 << 0);
constexpr uint8_t LCDStatInterrupt = (1 << 1);
constexpr uint8_t TimerInterrupt = (1 << 2);
constexpr uint8_t SerialInterrupt = (1 << 3);
constexpr uint8_t JoypadInterrupt = (1 << 4);

} // namespace boyboy::cpu