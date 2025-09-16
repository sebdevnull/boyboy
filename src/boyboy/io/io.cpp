/**
 * @file io.cpp
 * @brief Input/Output operations for BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

// TODO: for now we only handle serial output, the rest we read/write as it is without checking

#include "boyboy/io/io.h"

#include <iostream>

#include "boyboy/common/utils.h"
#include "boyboy/log/logging.h"

namespace boyboy::io {

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
[[nodiscard]] uint8_t Io::read(uint16_t addr) const
{
    return registers_.at(io_addr(addr));
}

void Io::write(uint16_t addr, uint8_t value)
{
    if (addr == IoReg::Serial::SB) {
        auto printable = utils::printable_char(static_cast<char>(value));
        log::trace("[IO] Serial Output: {} - '{}'", utils::PrettyHex{value}.to_string(), printable);

        // Output to the serial stream
        serial_out_ << static_cast<char>(value) << std::flush;
    }

    registers_.at(io_addr(addr)) = value;
}

} // namespace boyboy::io