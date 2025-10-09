/**
 * @file serial.cpp
 * @brief Serial I/O operations for BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

// TODO: for now we only handle serial output to a stream

#include "boyboy/io/serial.h"

#include "boyboy/common/utils.h"
#include "boyboy/io/registers.h"
#include "boyboy/log/logging.h"

namespace boyboy::io {

void Serial::tick([[maybe_unused]] uint16_t cycles) {}

[[nodiscard]] uint8_t Serial::read([[maybe_unused]] uint16_t addr) const
{
    return 0xFF;
}

void Serial::write(uint16_t addr, uint8_t value)
{
    if (addr == IoReg::Serial::SB) {
        auto printable = utils::printable_char(static_cast<char>(value));
        log::trace("[Serial] Output: {} - '{}'", utils::PrettyHex{value}.to_string(), printable);

        // Output to the serial stream
        *serial_out_ << static_cast<char>(value) << std::flush;
    }
}

void Serial::set_interrupt_cb(cpu::InterruptRequestCallback callback)
{
    request_interrupt_ = std::move(callback);
}

void Serial::reset() {}

} // namespace boyboy::io