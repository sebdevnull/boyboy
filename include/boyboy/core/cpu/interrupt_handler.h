/**
 * @file interrupt_handler.h
 * @brief CPU interrupt handling for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstdint>

namespace boyboy::core::mmu {
// Forward declaration
class Mmu;
} // namespace boyboy::core::mmu

namespace boyboy::core::cpu {

// Forward declaration
class Cpu;

class InterruptHandler {
public:
    InterruptHandler(Cpu& cpu, mmu::Mmu& mmu) : cpu_(cpu), mmu_(mmu) {}
    ~InterruptHandler() = default;

    // delete move and copy
    InterruptHandler(const InterruptHandler&) = delete;
    InterruptHandler& operator=(const InterruptHandler&) = delete;
    InterruptHandler(InterruptHandler&&) = delete;
    InterruptHandler& operator=(InterruptHandler&&) = delete;

    uint8_t service();
    void request(uint8_t interrupt);
    void enable(uint8_t interrupt);
    [[nodiscard]] bool is_requested(uint8_t interrupt) const;
    [[nodiscard]] bool is_enabled(uint8_t interrupt) const;
    [[nodiscard]] uint8_t pending() const;

    [[nodiscard]] uint8_t get_ie() const;
    [[nodiscard]] uint8_t get_if() const;

private:
    Cpu& cpu_;
    mmu::Mmu& mmu_;

    void clear_interrupt(uint8_t interrupt);

    void set_ie(uint8_t value);
    void set_if(uint8_t value);
};

} // namespace boyboy::core::cpu