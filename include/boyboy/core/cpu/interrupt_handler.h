/**
 * @file interrupt_handler.h
 * @brief CPU interrupt handling for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstdint>

#include "boyboy/core/cpu/cycles.h"
#include "boyboy/core/cpu/interrupts.h"

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

    void tick(Cycles cycles);
    TCycle service();
    void request(Interrupt interrupt);
    void enable(Interrupt interrupt);
    [[nodiscard]] bool is_requested(Interrupt interrupt) const;
    [[nodiscard]] bool is_enabled(Interrupt interrupt) const;
    [[nodiscard]] uint8_t pending() const;
    [[nodiscard]] bool is_servicing() const { return cycles_left_ > 0; }
    [[nodiscard]] bool should_service() const;
    [[nodiscard]] bool should_wake_up() const;

    [[nodiscard]] uint8_t get_ie() const;
    [[nodiscard]] uint8_t get_if() const;

private:
    Cpu& cpu_;
    mmu::Mmu& mmu_;

    // Interrupt service status
    TCycle cycles_left_{};
    Interrupt current_interrupt_{};

    void service_interrupt(Interrupt interrupt);
    void clear_interrupt(Interrupt interrupt);

    void set_ie(uint8_t value);
    void set_if(uint8_t value);

    [[nodiscard]] TCycle service_cycles() const;
};

} // namespace boyboy::core::cpu