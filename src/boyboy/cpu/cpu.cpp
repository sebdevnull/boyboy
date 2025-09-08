/**
 * @file cpu.cpp
 * @brief CPU emulation for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/cpu/cpu.h"

#include <cstdint>

#include "boyboy/cpu/instructions.h"
#include "boyboy/cpu/instructions_table.h"

namespace boyboy::cpu {

uint8_t Cpu::get_register(Register8Name reg) const
{
    switch (reg) {
    case Register8Name::A:
        return registers_.a();
    case Register8Name::F:
        return registers_.f();
    case Register8Name::B:
        return registers_.b();
    case Register8Name::C:
        return registers_.c();
    case Register8Name::D:
        return registers_.d();
    case Register8Name::E:
        return registers_.e();
    case Register8Name::H:
        return registers_.h();
    case Register8Name::L:
        return registers_.l();
    default:
        return 0;
    }
}

uint16_t Cpu::get_register(Register16Name reg) const
{
    switch (reg) {
    case Register16Name::AF:
        return registers_.af;
    case Register16Name::BC:
        return registers_.bc;
    case Register16Name::DE:
        return registers_.de;
    case Register16Name::HL:
        return registers_.hl;
    case Register16Name::SP:
        return registers_.sp;
    case Register16Name::PC:
        return registers_.pc;
    default:
        return 0;
    }
}

void Cpu::set_register(Register8Name reg, uint8_t value)
{
    switch (reg) {
    case Register8Name::A:
        registers_.a(value);
        break;
    case Register8Name::F:
        registers_.f(value);
        break;
    case Register8Name::B:
        registers_.b(value);
        break;
    case Register8Name::C:
        registers_.c(value);
        break;
    case Register8Name::D:
        registers_.d(value);
        break;
    case Register8Name::E:
        registers_.e(value);
        break;
    case Register8Name::H:
        registers_.h(value);
        break;
    case Register8Name::L:
        registers_.l(value);
        break;
    default:
        break;
    }
}

void Cpu::set_register(Register16Name reg, uint16_t value)
{
    switch (reg) {
    case Register16Name::AF:
        registers_.af = value;
        break;
    case Register16Name::BC:
        registers_.bc = value;
        break;
    case Register16Name::DE:
        registers_.de = value;
        break;
    case Register16Name::HL:
        registers_.hl = value;
        break;
    case Register16Name::SP:
        registers_.sp = value;
        break;
    case Register16Name::PC:
        registers_.pc = value;
        break;
    default:
        break;
    }
}

bool Cpu::get_flag(uint8_t flag) const
{
    return registers_.af.get_flag(flag);
}

void Cpu::set_flag(uint8_t flag, bool value)
{
    registers_.af.set_flag(flag, value);
}

void Cpu::tick()
{
    uint8_t opcode = fetch();
    InstructionType instr_type = InstructionType::Unprefixed;

    if (opcode == CBInstructionPrefix) {
        opcode = fetch();
        instr_type = InstructionType::CBPrefixed;
    }

    execute(opcode, instr_type);
}

uint8_t Cpu::fetch()
{
    return mmu_.read_byte(registers_.pc++);
}

void Cpu::execute(uint8_t opcode, InstructionType instr_type)
{
    const auto& instr = InstructionTable::get_instruction(instr_type, opcode);
    (this->*instr.execute)();
    cycles_ += instr.cycles;
}

std::string_view Cpu::disassemble(uint16_t addr) const
{
    uint8_t opcode = mmu_.read_byte(addr);
    InstructionType instr_type = InstructionType::Unprefixed;

    if (opcode == CBInstructionPrefix) {
        opcode = mmu_.read_byte(addr + 1);
        instr_type = InstructionType::CBPrefixed;
    }

    const auto& instr = InstructionTable::get_instruction(instr_type, opcode);

    // Simple implementation: just return the mnemonic for now
    return instr.mnemonic;
}

// Private methods

void Cpu::add(uint8_t val, bool carry)
{
    uint8_t result = registers_.a() + val;

    if (carry && (registers_.f() & Flag::Carry) != 0) {
        result += 1;
    }

    reset_flags();

    // set flags
    if (result == 0) {
        registers_.af.zero_flag(true);
    }
    if (result < registers_.a()) {
        registers_.af.carry_flag(true);
    }
    if ((registers_.a() & 0x0F) > (result & 0x0F)) {
        registers_.af.half_carry_flag(true);
    }

    registers_.a(result);
}

void Cpu::sub(uint8_t val, bool carry)
{
    uint8_t result = registers_.a() - val;

    if (carry && (registers_.f() & Flag::Carry) != 0) {
        result -= 1;
    }

    reset_flags();

    // set flags
    registers_.af.substract_flag(true);
    if (result == 0) {
        registers_.af.zero_flag(true);
    }
    if (result > registers_.a()) {
        registers_.af.carry_flag(true);
    }
    if ((registers_.a() & 0x0F) < (result & 0x0F)) {
        registers_.af.half_carry_flag(true);
    }

    registers_.a(result);
}

void Cpu::aand(uint8_t val)
{
    auto new_val = registers_.a() & val;
    registers_.a(new_val);

    reset_flags();

    // set flags
    registers_.af.half_carry_flag(true);
    if (registers_.a() == 0) {
        registers_.af.zero_flag(true);
    }
}

void Cpu::xxor(uint8_t val)
{
    auto new_val = registers_.a() ^ val;
    registers_.a(new_val);

    reset_flags();

    // set flags
    if (registers_.a() == 0) {
        registers_.af.zero_flag(true);
    }
}

void Cpu::oor(uint8_t val)
{
    auto new_val = registers_.a() | val;
    registers_.a(new_val);

    reset_flags();

    // set flags
    if (registers_.a() == 0) {
        registers_.af.zero_flag(true);
    }
}

void Cpu::cp(uint8_t val)
{
    reset_flags();

    // set flags
    registers_.af.substract_flag(true);
    if (registers_.a() == val) {
        registers_.af.zero_flag(true);
    }
    else if (registers_.a() > val) {
        registers_.af.half_carry_flag(true);
    }
    else {
        registers_.af.carry_flag(true);
    }
}

} // namespace boyboy::cpu