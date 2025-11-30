/**
 * @file apu.cpp
 * @brief APU (Audio Processing Unit) implementation for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/core/io/apu.h"

#include "boyboy/core/io/constants.h"
#include "boyboy/core/io/registers.h"

namespace boyboy::core::io {

void Apu::init()
{
    // Assume DMG0
    using ApuInitVal = RegInitValues::Dmg0::Apu;

    registers_.fill(0xFF);

    registers_.at(IoReg::Apu::local_addr(IoReg::Apu::NR10)) = ApuInitVal::NR10;
    registers_.at(IoReg::Apu::local_addr(IoReg::Apu::NR11)) = ApuInitVal::NR11;
    registers_.at(IoReg::Apu::local_addr(IoReg::Apu::NR12)) = ApuInitVal::NR12;
    registers_.at(IoReg::Apu::local_addr(IoReg::Apu::NR13)) = ApuInitVal::NR13;
    registers_.at(IoReg::Apu::local_addr(IoReg::Apu::NR14)) = ApuInitVal::NR14;
    registers_.at(IoReg::Apu::local_addr(IoReg::Apu::NR21)) = ApuInitVal::NR21;
    registers_.at(IoReg::Apu::local_addr(IoReg::Apu::NR22)) = ApuInitVal::NR22;
    registers_.at(IoReg::Apu::local_addr(IoReg::Apu::NR23)) = ApuInitVal::NR23;
    registers_.at(IoReg::Apu::local_addr(IoReg::Apu::NR24)) = ApuInitVal::NR24;
    registers_.at(IoReg::Apu::local_addr(IoReg::Apu::NR30)) = ApuInitVal::NR30;
    registers_.at(IoReg::Apu::local_addr(IoReg::Apu::NR31)) = ApuInitVal::NR31;
    registers_.at(IoReg::Apu::local_addr(IoReg::Apu::NR32)) = ApuInitVal::NR32;
    registers_.at(IoReg::Apu::local_addr(IoReg::Apu::NR33)) = ApuInitVal::NR33;
    registers_.at(IoReg::Apu::local_addr(IoReg::Apu::NR34)) = ApuInitVal::NR34;
    registers_.at(IoReg::Apu::local_addr(IoReg::Apu::NR41)) = ApuInitVal::NR41;
    registers_.at(IoReg::Apu::local_addr(IoReg::Apu::NR42)) = ApuInitVal::NR42;
    registers_.at(IoReg::Apu::local_addr(IoReg::Apu::NR43)) = ApuInitVal::NR43;
    registers_.at(IoReg::Apu::local_addr(IoReg::Apu::NR44)) = ApuInitVal::NR44;
    registers_.at(IoReg::Apu::local_addr(IoReg::Apu::NR50)) = ApuInitVal::NR50;
    registers_.at(IoReg::Apu::local_addr(IoReg::Apu::NR51)) = ApuInitVal::NR51;
    registers_.at(IoReg::Apu::local_addr(IoReg::Apu::NR52)) = ApuInitVal::NR52;
}

void Apu::reset()
{
    init();
}

void Apu::tick(uint16_t /*cycles*/) {}

[[nodiscard]] uint8_t Apu::read(uint16_t addr) const
{
    return registers_.at(IoReg::Apu::local_addr(addr));
}

void Apu::write(uint16_t addr, uint8_t value)
{
    registers_.at(IoReg::Apu::local_addr(addr)) = value;
}

void Apu::set_interrupt_cb(cpu::InterruptRequestCallback callback)
{
    request_interrupt_ = std::move(callback);
}

} // namespace boyboy::core::io