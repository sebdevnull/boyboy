/**
 * @file cpu.cpp
 * @brief CPU emulation for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/core/cpu/cpu.h"

#include <cstdint>

#include "boyboy/core/cpu/instructions.h"
#include "boyboy/core/cpu/instructions_table.h"
#include "boyboy/common/log/logging.h"
#include "boyboy/core/profiling/profiler_utils.h"

namespace boyboy::core::cpu {

using namespace boyboy::common;

uint8_t Cpu::get_register(Reg8Name reg) const
{
    switch (reg) {
        case Reg8Name::A:
            return registers_.a();
        case Reg8Name::F:
            return registers_.f();
        case Reg8Name::B:
            return registers_.b();
        case Reg8Name::C:
            return registers_.c();
        case Reg8Name::D:
            return registers_.d();
        case Reg8Name::E:
            return registers_.e();
        case Reg8Name::H:
            return registers_.h();
        case Reg8Name::L:
            return registers_.l();
        default:
            return 0;
    }
}

uint16_t Cpu::get_register(Reg16Name reg) const
{
    switch (reg) {
        case Reg16Name::AF:
            return registers_.af;
        case Reg16Name::BC:
            return registers_.bc;
        case Reg16Name::DE:
            return registers_.de;
        case Reg16Name::HL:
            return registers_.hl;
        case Reg16Name::SP:
            return registers_.sp;
        case Reg16Name::PC:
            return registers_.pc;
        default:
            return 0;
    }
}

void Cpu::set_register(Reg8Name reg, uint8_t value)
{
    switch (reg) {
        case Reg8Name::A:
            registers_.a(value);
            break;
        case Reg8Name::F:
            registers_.f(value);
            break;
        case Reg8Name::B:
            registers_.b(value);
            break;
        case Reg8Name::C:
            registers_.c(value);
            break;
        case Reg8Name::D:
            registers_.d(value);
            break;
        case Reg8Name::E:
            registers_.e(value);
            break;
        case Reg8Name::H:
            registers_.h(value);
            break;
        case Reg8Name::L:
            registers_.l(value);
            break;
        default:
            break;
    }
}

void Cpu::set_register(Reg16Name reg, uint16_t value)
{
    switch (reg) {
        case Reg16Name::AF:
            registers_.af = value;
            break;
        case Reg16Name::BC:
            registers_.bc = value;
            break;
        case Reg16Name::DE:
            registers_.de = value;
            break;
        case Reg16Name::HL:
            registers_.hl = value;
            break;
        case Reg16Name::SP:
            registers_.sp = value;
            break;
        case Reg16Name::PC:
            registers_.pc = value;
            break;
        default:
            break;
    }
}

uint8_t Cpu::step()
{
    BB_PROFILE_SCOPE(profiling::FrameTimer::Cpu);

    interrupt_handler_.service();

    if (halted_) {
        // TODO: This should be 0 cycles, but until we have a clock, we use 4 cycles
        // to keep other components ticking
        cycles_ += 4;
        return 4;
    }

#ifdef DISASSEMBLY_LOG
    trace();
#endif

    uint8_t opcode = fetch();
    InstructionType instr_type = InstructionType::Unprefixed;

    if (opcode == CBInstructionPrefix) {
        opcode = fetch();
        instr_type = InstructionType::CBPrefixed;
    }

    uint8_t cycles = execute(opcode, instr_type);

    // IME is enabled after the instruction following EI
    if (ime_scheduled_ &&
        (opcode != static_cast<uint8_t>(Opcode::EI) || instr_type != InstructionType::Unprefixed)) {
        ime_scheduled_ = false;
        ime_ = true;
    }

    return cycles;
}

uint8_t Cpu::fetch()
{
    BB_PROFILE_START(profiling::HotSection::CpuFetch);
    uint8_t result = read_byte(registers_.pc++);
    BB_PROFILE_STOP(profiling::HotSection::CpuFetch);
    return result;
}

[[nodiscard]] uint8_t Cpu::peek() const
{
    return read_byte(registers_.pc);
}

uint8_t Cpu::execute(uint8_t opcode, InstructionType instr_type)
{
    BB_PROFILE_START(profiling::HotSection::CpuExecute);
    const auto& instr = InstructionTable::get_instruction(instr_type, opcode);
    (this->*instr.execute)();
    cycles_ += instr.cycles;
    BB_PROFILE_STOP(profiling::HotSection::CpuExecute);
    return instr.cycles;
}

void Cpu::trace() const
{
    log::cpu_trace("--- CPU TRACE ---");
    log::cpu_trace(
        "Instruction: {} ({})", disassemble(registers_.pc), common::utils::PrettyHex(peek()).to_string()
    );
    log::cpu_trace(
        "Next bytes: {} {} {}",
        common::utils::PrettyHex(read_byte(registers_.pc + 1)).to_string(),
        common::utils::PrettyHex(read_byte(registers_.pc + 2)).to_string(),
        common::utils::PrettyHex(read_byte(registers_.pc + 3)).to_string()
    );
    log::cpu_trace(
        "CPU State: PC={}, SP={}, AF={}, BC={}, DE={}, HL={}, Flags=[Z={}, N={}, H={}, C={}], "
        "IME={}, HALT={}, Cycles={}",
        common::utils::PrettyHex(registers_.pc).to_string(),
        common::utils::PrettyHex(registers_.sp).to_string(),
        common::utils::PrettyHex(registers_.af).to_string(),
        common::utils::PrettyHex(registers_.bc).to_string(),
        common::utils::PrettyHex(registers_.de).to_string(),
        common::utils::PrettyHex(registers_.hl).to_string(),
        get_flag(Flag::Zero) ? 1 : 0,
        get_flag(Flag::Substract) ? 1 : 0,
        get_flag(Flag::HalfCarry) ? 1 : 0,
        get_flag(Flag::Carry) ? 1 : 0,
        ime_ ? "ENABLED" : "DISABLED",
        halted_ ? "HALTED" : "RUNNING",
        cycles_
    );
    log::cpu_trace("----------------");
}

[[nodiscard]] std::string_view Cpu::disassemble(uint16_t addr) const
{
    uint8_t opcode = read_byte(addr);
    InstructionType instr_type = InstructionType::Unprefixed;

    if (opcode == CBInstructionPrefix) {
        opcode = read_byte(addr + 1);
        instr_type = InstructionType::CBPrefixed;
    }

    const auto& instr = InstructionTable::get_instruction(instr_type, opcode);

    // Simple implementation: just return the mnemonic for now
    return instr.mnemonic;
}

// Private methods

void Cpu::add(uint8_t val, bool use_carry)
{
    uint8_t a = registers_.a();
    uint8_t carry_in = (use_carry && get_flag(Flag::Carry)) ? 1 : 0;

    uint16_t sum = uint16_t(a) + uint16_t(val) + carry_in;
    uint8_t result = sum & 0xFF;

    registers_.a(result);

    set_flag(Flag::Zero, result == 0);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, ((a & 0xF) + (val & 0xF) + carry_in) > 0xF);
    set_flag(Flag::Carry, sum > 0xFF);
}

void Cpu::sub(uint8_t val, bool use_carry)
{
    uint8_t a = registers_.a();
    uint8_t carry_in = (use_carry && get_flag(Flag::Carry)) ? 1 : 0;

    uint16_t sub = uint16_t(val) + carry_in;
    uint8_t result = a - sub;

    registers_.a(result);

    set_flag(Flag::Zero, result == 0);
    set_flag(Flag::Substract, true);
    set_flag(Flag::HalfCarry, (a & 0xF) < ((val & 0xF) + carry_in));
    set_flag(Flag::Carry, a < sub);
}

void Cpu::aand(uint8_t val)
{
    auto result = registers_.a() & val;
    registers_.a(result);

    set_flag(Flag::Zero, result == 0);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, true);
    set_flag(Flag::Carry, false);
}

void Cpu::xxor(uint8_t val)
{
    auto result = registers_.a() ^ val;
    registers_.a(result);

    set_flag(Flag::Zero, result == 0);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, false);
    set_flag(Flag::Carry, false);
}

void Cpu::oor(uint8_t val)
{
    auto result = registers_.a() | val;
    registers_.a(result);

    set_flag(Flag::Zero, result == 0);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, false);
    set_flag(Flag::Carry, false);
}

void Cpu::cp(uint8_t val)
{
    // Compare is essentially a subtraction without modifying A
    uint8_t a = registers_.a();
    uint8_t result = a - val;

    set_flag(Flag::Zero, result == 0);
    set_flag(Flag::Substract, true);
    set_flag(Flag::HalfCarry, (result & 0x0F) > (a & 0x0F));
    set_flag(Flag::Carry, result > a);
}

} // namespace boyboy::core::cpu