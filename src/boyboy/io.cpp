/**
 * @file io.cpp
 * @brief Input/Output operations for BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/io.h"

#include <iostream>

#include "boyboy/common/utils.h"
#include "boyboy/log/logging.h"

namespace boyboy::io {

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
[[nodiscard]] uint8_t Io::read(uint16_t addr) const
{
    // TODO: implement I/O register reads
    log::trace("[IO] Read from unimplemented I/O register: 0x{:04X}", addr);
    return 0xFF;
}

void Io::write(uint16_t addr, uint8_t value)
{
    if (addr == IoReg::Serial::SB) {
        auto printable = utils::printable_char(static_cast<char>(value));
        log::trace("[IO] Serial Output: {} - '{}'", utils::PrettyHex{value}.to_string(), printable);

        // Output to the serial stream
        serial_out_ << static_cast<char>(value) << std::flush;
    }
    else {
        log::trace("[IO] Write to unimplemented I/O register: 0x{:04X} = 0x{:02X}", addr, value);
    }
}

} // namespace boyboy::io