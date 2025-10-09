/**
 * @file cpu_constants.h
 * @brief
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstdint>

namespace boyboy::core::cpu {

// Initial values for registers after boot ROM
constexpr uint16_t AFStartValue = 0x01B0; // assumes header cks != 0, otherwise 0x0180
constexpr uint16_t BCStartValue = 0x0013;
constexpr uint16_t DEStartValue = 0x00D8;
constexpr uint16_t HLStartValue = 0x014D;
constexpr uint16_t PCStartValue = 0x0100;
constexpr uint16_t SPStartValue = 0xFFFE;

constexpr uint8_t CBInstructionPrefix = 0xCB;

constexpr uint16_t HighRAMOffset = 0xFF00;

constexpr uint64_t MasterClockFrequencyHz = 4'194'304;                  // 4.194304 MHz
constexpr uint64_t SystemClockFrequencyHz = MasterClockFrequencyHz / 4; // 1.048576 MHz

} // namespace boyboy::core::cpu