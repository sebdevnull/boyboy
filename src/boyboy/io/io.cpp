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

void Io::tick(uint16_t cycles)
{
    timer_.tick(cycles);
}

[[nodiscard]] uint8_t Io::read(uint16_t addr) const
{
    if (IoReg::Timer::contains(addr)) {
        return timer_.read(addr);
    }

    // Default behavior: return the value in the register
    return registers_.at(io_addr(addr));
}

void Io::write(uint16_t addr, uint8_t value)
{
    if (addr == IoReg::Serial::SB) {
        // TODO: move to serial module
        auto printable = utils::printable_char(static_cast<char>(value));
        log::trace("[IO] Serial Output: {} - '{}'", utils::PrettyHex{value}.to_string(), printable);

        // Output to the serial stream
        serial_out_ << static_cast<char>(value) << std::flush;
    }
    else if (IoReg::Timer::contains(addr)) {
        timer_.write(addr, value);
        return;
    }

    registers_.at(io_addr(addr)) = value;
}

} // namespace boyboy::io