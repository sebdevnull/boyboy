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

} // namespace boyboy::cpu