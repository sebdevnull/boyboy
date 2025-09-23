/**
 * @file cpu_constants.h
 * @brief
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstdint>

namespace boyboy::cpu {

// Initial values for registers after boot ROM
constexpr uint16_t AFStartValue = 0x01B0; // assumes header cks != 0, otherwise 0x0180
constexpr uint16_t BCStartValue = 0x0013;
constexpr uint16_t DEStartValue = 0x00D8;
constexpr uint16_t HLStartValue = 0x014D;
constexpr uint16_t PCStartValue = 0x0100;
constexpr uint16_t SPStartValue = 0xFFFE;

constexpr uint8_t CBInstructionPrefix = 0xCB;

constexpr uint16_t HighRAMOffset = 0xFF00;

} // namespace boyboy::cpu