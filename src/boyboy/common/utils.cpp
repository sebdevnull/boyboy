/**
 * @file utils.cpp
 * @brief Utility functions for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/common/utils.h"

namespace boyboy::utils {

/**
 * @brief Returns a printable representation of a character.
 *
 * @details Converts control characters to escape sequences,
 *          non-printable characters to their hex representation,
 *          and printable characters to themselves.
 *
 * @param c Character to convert
 * @return std::string Printable string representation
 */
std::string printable_char(char c)
{
    switch (c) {
    case '\n':
        return "\\n";
    case '\r':
        return "\\r";
    case '\t':
        return "\\t";
    case '\0':
        return "\\0";
    default:
        if (isprint(static_cast<unsigned char>(c)) != 0) {
            return std::string{c};
        }
        else {
            return utils::PrettyHex{static_cast<uint8_t>(c)}.to_string();
        }
    }
}

char* as_char_ptr(std::byte* ptr) noexcept
{
    static_assert(sizeof(std::byte) == sizeof(char), "std::byte and char must have the same size");
    static_assert(alignof(std::byte) == alignof(char),
                  "std::byte and char must have the same alignment");
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return reinterpret_cast<char*>(ptr);
}

const char* as_char_ptr(const std::byte* ptr) noexcept
{
    static_assert(sizeof(std::byte) == sizeof(char), "std::byte and char must have the same size");
    static_assert(alignof(std::byte) == alignof(char),
                  "std::byte and char must have the same alignment");
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return reinterpret_cast<const char*>(ptr);
}

} // namespace boyboy::utils