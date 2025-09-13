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
#include <string>

namespace boyboy::utils {

// Byte manipulation utilities
constexpr uint16_t to_u16(uint8_t msb, uint8_t lsb)
{
    return (static_cast<uint16_t>(msb) << 8) | lsb;
}

constexpr uint16_t to_u16(std::byte msb, std::byte lsb)
{
    return (static_cast<uint16_t>(msb) << 8) | static_cast<uint16_t>(lsb);
}

constexpr uint8_t to_u8(std::byte byte)
{
    return static_cast<uint8_t>(byte);
}

constexpr uint8_t lsb(uint16_t value)
{
    return value & 0xFF;
}

constexpr uint8_t msb(uint16_t value)
{
    return (value >> 8) & 0xFF;
}

// Print utilities
struct PrettyHex {
    uint32_t value;
    int width;

    PrettyHex(uint8_t v) : value(v), width(2) {}
    PrettyHex(uint16_t v) : value(v), width(4) {}
    PrettyHex(uint32_t v, int w) : value(v), width(w) {}

    [[nodiscard]] std::string to_string() const
    {
        std::ostringstream oss;
        oss << *this;
        return oss.str();
    }

    friend std::ostream& operator<<(std::ostream& os, const PrettyHex& ph)
    {
        os << "0x" << std::hex << std::setw(ph.width) << std::setfill('0') << ph.value << std::dec;
        return os;
    }
};

} // namespace boyboy::utils