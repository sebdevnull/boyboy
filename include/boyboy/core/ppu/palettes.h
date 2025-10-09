/**
 * @file palettes.h
 * @brief Color palettes for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <array>
#include <cstdint>

namespace boyboy::core::ppu::palettes {

using Pixel = uint32_t; // ARGB format

// Simple grayscale palette (white to black)
static constexpr std::array<Pixel, 4> Grayscale = {
    0xFFFFFFFF,
    0xFFAAAAAA,
    0xFF555555,
    0xFF000000,
};

// Green-scale palette (light green to dark green)
static constexpr std::array<Pixel, 4> GreenScale = {
    0xFFD0F8E0,
    0xFF70C088,
    0xFF566834,
    0xFF201808,
};

// Light green palette
static constexpr std::array<Pixel, 4> LightGreen = {
    0xFFF2FFF2,
    0xFFBCD9AD,
    0xFF7C8C5B,
    0xFF1A1A0D,
};

// White chocolate palette
static constexpr std::array<Pixel, 4> WhiteChocolate = {
    0xFFD2E6EC,
    0xFF93A1A5,
    0xFF545C5E,
    0xFF151718,
};

// Sepia palette
static constexpr std::array<Pixel, 4> Sepia = {
    0xFFCFE1F2,
    0xFF8AA8C9,
    0xFF3A5275,
    0xFF18243A,
};

// Olive green palette
static constexpr std::array<Pixel, 4> OliveGreen = {
    0xFF0FBC9B,
    0xFF0FAC8B,
    0xFF306230,
    0xFF0F380F,
};

// Pocket palette
static constexpr std::array<Pixel, 4> PocketGray = {
    0xFFE0E0E0,
    0xFFA5A5A5,
    0xFF5C5C5C,
    0xFF1C1C1C,
};

static constexpr std::array<Pixel, 4> PocketGreen = {
    0xFF9CC6B5,
    0xFF7B9C8D,
    0xFF517263,
    0xFF203830,
};

static constexpr std::array<Pixel, 4> Chocolate = {
    0xFFDDEDF6,
    0xFF8CADCA,
    0xFF1C3B62,
    0xFF0A2649,
};

} // namespace boyboy::core::ppu::palettes