/**
 * @file io.h
 * @brief Input/Output operations for BoyBoy emulator.
 *
 * For now the only thing the module does is redirecting serial output to a stream.
 * Other writes are ignored and reads return 0xFF.
 * Used for debugging and testing purposes.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <array>
#include <cstdint>

#include "boyboy/io/iocomponent.h"
#include "boyboy/io/joypad.h"
#include "boyboy/io/serial.h"
#include "boyboy/io/timer.h"
#include "boyboy/mmu/constants.h"
#include "boyboy/ppu/ppu.h"

namespace boyboy::io {

class Io {
public:
    Io();

    // Read/write I/O registers
    [[nodiscard]] uint8_t read(uint16_t addr) const;
    void write(uint16_t addr, uint8_t value);

    void tick(uint16_t cycles);

    // Components accessors
    std::vector<IoComponent*>& get_components() { return components_; }
    [[nodiscard]] const std::vector<IoComponent*>& get_components() const { return components_; }
    [[nodiscard]] const Timer& timer() const { return timer_; }
    [[nodiscard]] Timer& timer() { return timer_; }
    [[nodiscard]] const Serial& serial() const { return serial_; }
    [[nodiscard]] Serial& serial() { return serial_; }
    [[nodiscard]] const ppu::Ppu& ppu() const { return ppu_; }
    [[nodiscard]] ppu::Ppu& ppu() { return ppu_; }
    [[nodiscard]] const Joypad& joypad() const { return joypad_; }
    [[nodiscard]] Joypad& joypad() { return joypad_; }

    void reset();

    ~Io() = default;
    Io(const Io&) = delete;
    Io& operator=(const Io&) = delete;
    Io(Io&&) = delete;
    Io& operator=(Io&&) = delete;

private:
    Serial serial_;
    Timer timer_;
    ppu::Ppu ppu_;
    Joypad joypad_;

    std::vector<IoComponent*> components_{&serial_, &timer_, &ppu_, &joypad_};

    std::array<uint8_t, mmu::IOSize> registers_{};

    [[nodiscard]] static uint8_t io_addr(uint16_t addr)
    {
        return static_cast<uint8_t>(addr - mmu::IOStart);
    }
};

} // namespace boyboy::io