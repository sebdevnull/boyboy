/**
 * @file interrupt_handler.h
 * @brief CPU interrupt handling for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <array>
#include <cstdint>

namespace boyboy::mmu {
// Forward declaration
class Mmu;
} // namespace boyboy::mmu

namespace boyboy::cpu {

// Forward declaration
class Cpu;

struct Interrupts {
    static constexpr uint8_t VBlank = (1 << 0);
    static constexpr uint8_t LCDStat = (1 << 1);
    static constexpr uint8_t Timer = (1 << 2);
    static constexpr uint8_t Serial = (1 << 3);
    static constexpr uint8_t Joypad = (1 << 4);
};

struct InterruptVectors {
    static constexpr uint8_t VBlank = 0x40;
    static constexpr uint8_t LCDStat = 0x48;
    static constexpr uint8_t Timer = 0x50;
    static constexpr uint8_t Serial = 0x58;
    static constexpr uint8_t Joypad = 0x60;

    static constexpr uint8_t VectorsCount = 5;

    static constexpr std::array<uint16_t, VectorsCount> Vectors = {
        VBlank,
        LCDStat,
        Timer,
        Serial,
        Joypad,
    };
};

class InterruptHandler {
public:
    InterruptHandler(Cpu& cpu, mmu::Mmu& mmu) : cpu_(cpu), mmu_(mmu) {}
    ~InterruptHandler() = default;

    // delete move and copy
    InterruptHandler(const InterruptHandler&) = delete;
    InterruptHandler& operator=(const InterruptHandler&) = delete;
    InterruptHandler(InterruptHandler&&) = delete;
    InterruptHandler& operator=(InterruptHandler&&) = delete;

    void service();
    void request(uint8_t interrupt);
    void enable(uint8_t interrupt);
    [[nodiscard]] bool is_requested(uint8_t interrupt) const;
    [[nodiscard]] bool is_enabled(uint8_t interrupt) const;

private:
    Cpu& cpu_;
    mmu::Mmu& mmu_;

    void clear_interrupt(uint8_t interrupt);

    [[nodiscard]] uint8_t get_ie() const;
    void set_ie(uint8_t value);
    [[nodiscard]] uint8_t get_if() const;
    void set_if(uint8_t value);
};

} // namespace boyboy::cpu