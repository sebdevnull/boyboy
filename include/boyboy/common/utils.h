/**
 * @file utils.h
 * @brief Utility functions for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <ios>
#include <ostream>

namespace boyboy::utils {

// Byte manipulation utilities
constexpr uint16_t to_u16(uint8_t high, uint8_t low)
{
    return (static_cast<uint16_t>(high) << 8) | low;
}

constexpr uint16_t to_u16(std::byte high, std::byte low)
{
    return (static_cast<uint16_t>(high) << 8) | static_cast<uint16_t>(low);
}

constexpr uint8_t to_u8(std::byte byte)
{
    return static_cast<uint8_t>(byte);
}

constexpr uint8_t get_lsb(uint16_t value)
{
    return value & 0xFF;
}

constexpr uint8_t get_msb(uint16_t value)
{
    return (value >> 8) & 0xFF;
}

// Print utilities
struct PrettyHex {
    uint32_t value;
    int width;

    PrettyHex(uint8_t v)  : value(v), width(2) {}
    PrettyHex(uint16_t v) : value(v), width(4) {}
    PrettyHex(uint32_t v, int w) : value(v), width(w) {}
};

inline std::ostream& operator<<(std::ostream& os, const PrettyHex& ph) {
    os << "0x" << std::hex << std::setw(ph.width) << std::setfill('0') << ph.value << std::dec;
    return os;
}

} // namespace boyboy::utils