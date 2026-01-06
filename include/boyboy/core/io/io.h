/**
 * @file io.h
 * @brief Input/Output operations for BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "boyboy/core/cpu/interrupts.h"
#include "boyboy/core/io/iocomponent.h"
#include "boyboy/core/io/registers.h"
#include "boyboy/core/mmu/constants.h"

namespace boyboy::core::ppu {
class Ppu;
}

namespace boyboy::core::io {

class IoComponent;
class Timer;
class Joypad;
class Serial;
class Apu;

class Io {
public:
    // I/O operations
    void init();
    void reset();
    [[nodiscard]] uint8_t read(uint16_t addr) const;
    void write(uint16_t addr, uint8_t value);
    void tick(uint16_t cycles);

    /**
     * @brief Register an I/O component.
     *
     * @tparam T Component class. Must be derived from IoComponent.
     * @param comp Component to register.
     */
    template <typename T>
        requires std::is_base_of_v<IoComponent, T>
    void register_component(std::shared_ptr<T> comp)
    {
        if constexpr (std::is_same_v<T, ppu::Ppu>) {
            ppu_ = comp;
        }
        else if constexpr (std::is_same_v<T, Timer>) {
            timer_ = comp;
        }
        else if constexpr (std::is_same_v<T, Joypad>) {
            joypad_ = comp;
        }
        else if constexpr (std::is_same_v<T, Serial>) {
            serial_ = comp;
        }
        else if constexpr (std::is_same_v<T, Apu>) {
            apu_ = comp;
        }

        comp->set_interrupt_cb([this](cpu::Interrupt interrupt) {
            this->write(IoReg::Interrupts::IF, std::to_underlying(interrupt));
        });

        components_.push_back(comp);
    }

    // Components accessors
    std::vector<std::shared_ptr<IoComponent>>& get_components() { return components_; }
    [[nodiscard]] const std::vector<std::shared_ptr<IoComponent>>& get_components() const
    {
        return components_;
    }
    [[nodiscard]] const std::shared_ptr<ppu::Ppu>& ppu() const;
    [[nodiscard]] std::shared_ptr<ppu::Ppu>& ppu();
    [[nodiscard]] const std::shared_ptr<Timer>& timer() const;
    [[nodiscard]] std::shared_ptr<Timer> timer();
    [[nodiscard]] const std::shared_ptr<Joypad>& joypad() const;
    [[nodiscard]] std::shared_ptr<Joypad> joypad();
    [[nodiscard]] const std::shared_ptr<Serial>& serial() const;
    [[nodiscard]] std::shared_ptr<Serial> serial();
    [[nodiscard]] const std::shared_ptr<Apu>& apu() const;
    [[nodiscard]] std::shared_ptr<Apu> apu();

private:
    std::shared_ptr<ppu::Ppu> ppu_;
    std::shared_ptr<Timer> timer_;
    std::shared_ptr<Joypad> joypad_;
    std::shared_ptr<Serial> serial_;
    std::shared_ptr<Apu> apu_;

    // Components registry
    std::vector<std::shared_ptr<IoComponent>> components_;

    // Register address space for unmapped addresses
    std::array<uint8_t, mmu::IOSize> registers_{};

    [[nodiscard]] static uint8_t io_addr(uint16_t addr)
    {
        return static_cast<uint8_t>(addr - mmu::IOStart);
    }

    // Generic component read/write
    [[nodiscard]] uint8_t component_read(const IoComponent* comp, uint16_t addr) const;
    void component_write(IoComponent* comp, uint16_t addr, uint8_t value);
};

} // namespace boyboy::core::io