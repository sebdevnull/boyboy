/**
 * @file cpu.cpp
 * @brief CPU emulation for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "cpu.h"

#include <iostream>

namespace boyboy::cpu {

void Cpu::fetch()
{
    uint8_t opcode = mmu_->read_byte(registers_.pc++);

    std::cout << "Executing instruction " << InstructionTable.at(opcode).disassembly << "\n";

    // TODO: rework this long, ugly switch and avoid "magic numbers"
    // clang-format off
    switch (opcode) {
    case 0x00: // nop
        break;
    // 8-bit Load (LD)
    case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x47: // ld b,reg
    case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d: case 0x4f: // ld c,reg
    case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x57: // ld d,reg
    case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d: case 0x5f: // ld e,reg
    case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x67: // ld h,reg
    case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d: case 0x6f: // ld l,reg
    case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c: case 0x7d: case 0x7f: // ld a,reg
    {
        uint8_t* dst = dst_reg(opcode);
        uint8_t* src = src_reg(opcode);
        *dst = *src;
        break;
    }
    case 0x06: case 0x0E: case 0x16: case 0x1E: case 0x26: case 0x2E: case 0x3E: // ld reg, d8
    {
        uint8_t value = mmu_->read_byte(registers_.pc++);
        uint8_t* dst = dst_reg(opcode);
        *dst = value;
        break;
    }
    case 0x46: case 0x4E: case 0x56: case 0x5E: case 0x66: case 0x6E: case 0x7E: // ld reg, (hl)
    {
        uint8_t* dst = dst_reg(opcode);
        uint16_t src_addr = registers_.hl;
        *dst = mmu_->read_byte(src_addr);
        break;
    }
    case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x77: // ld (hl), reg
    {
        uint16_t dst_addr = registers_.hl;
        uint8_t* src = src_reg(opcode);
        mmu_->write_byte(dst_addr, *src);
        break;
    }
    case 0x02: case 0x12: // ld (reg16), a
    {
        uint16_t dst_addr = *dst_reg16(opcode);
        mmu_->write_byte(dst_addr, registers_.a);
        break;
    }
    case 0x22: // ld (hli), a
    {
        mmu_->write_byte(registers_.hl++, registers_.a);
        break;
    }
    case 0x32: // ld (hld), a
    {
        mmu_->write_byte(registers_.hl--, registers_.a);
        break;
    }
    case 0x0A: case 0x1A: // ld a, (reg16)
    {
        uint16_t src_addr = *src_reg16(opcode);
        registers_.a = mmu_->read_byte(src_addr);
        break;
    }
    case 0x2A: // ld a, (hli)
    {
        registers_.a = mmu_->read_byte(registers_.hl++);
        break;
    }
    case 0x3A: // ld a, (hld)
    {
        registers_.a = mmu_->read_byte(registers_.hl--);
        break;
    }
    case 0x36: // ld (hl), d8
    {
        mmu_->write_byte(registers_.hl, mmu_->read_byte(registers_.pc++));
        break;
    }
    case 0xE0: // ld (0xff00 + a8), a
    {
        uint16_t dst_addr = (uint16_t)0xFF00 + mmu_->read_byte(registers_.pc++);
        mmu_->write_byte(dst_addr, registers_.a);
        break;
    }
    case 0xF0: // ld a, (0xff00 + a8)
    {
        uint16_t src_addr = (uint16_t)0xFF00 + mmu_->read_byte(registers_.pc++);
        registers_.a = mmu_->read_byte(src_addr);
        break;
    }
    case 0xE2: // ld (c), a
    {
        mmu_->write_byte(registers_.c, registers_.a);
        break;
    }
    case 0xF2: // ld a, (c)
    {
        registers_.a = mmu_->read_byte(registers_.c);
        break;
    }
    case 0xEA: // ld (a16), a
    {
        uint16_t dst_addr = (mmu_->read_byte(registers_.pc) << 8);
        ++registers_.pc;
        dst_addr += mmu_->read_byte(registers_.pc);
        ++registers_.pc;
        mmu_->write_byte(dst_addr, registers_.a);
        break;
    }
    case 0xFA: // ld a, (a16)
    {
        uint16_t src_addr = (mmu_->read_byte(registers_.pc) << 8);
        ++registers_.pc;
        src_addr += mmu_->read_byte(registers_.pc);
        ++registers_.pc;
        registers_.a = mmu_->read_byte(src_addr);
        break;
    }
    // 8-bit ALU
    case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87: // add a, reg
    case 0x88: case 0x89: case 0x8A: case 0x8B: case 0x8C: case 0x8D: case 0x8E: case 0x8F: // adc a, reg
    case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97: // sub reg
    case 0x98: case 0x99: case 0x9A: case 0x9B: case 0x9C: case 0x9D: case 0x9E: case 0x9F: // sbc reg
    case 0xA0: case 0xA1: case 0xA2: case 0xA3: case 0xA4: case 0xA5: case 0xA6: case 0xA7: // and reg
    case 0xA8: case 0xA9: case 0xAA: case 0xAB: case 0xAC: case 0xAD: case 0xAE: case 0xAF: // xor reg
    case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB6: case 0xB7: // or reg
    case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC: case 0xBD: case 0xBE: case 0xBF: // cp reg
    {
        uint8_t val = 0;
        bool carry = false;

        // check if (hl)
        if ((opcode & 0x0F) == 0x06 || (opcode & 0x0F) == 0x0E) {
            val = mmu_->read_byte(registers_.hl);
        } else {
            val = *src_reg(opcode);
        }

        // adc
        if ((opcode & 0x0F) > 0x07) {
            carry = true;
        }

        if (opcode < 0x90) {
            add(val, carry);
        } else if (opcode < 0xA0) {
            sub(val, carry);
        } else if (opcode < 0xA8) {
            aand(val);
        } else if (opcode < 0xB0) {
            xxor(val);
        } else if (opcode < 0xB9) {
            oor(val);
        } else {
            cp(val);
        }

        break;
    }
    default:
        std::cout << "Instruction " << InstructionTable.at(opcode).disassembly << " not implemented"
                  << "\n";
        break;
    }
    // clang-format on
}

void Cpu::add(uint8_t val, bool carry)
{
    uint8_t result = registers_.a + val;

    if (carry && (registers_.f & Flag::Carry) != 0) {
        result += 1;
    }

    reset_flags();

    // set flags
    if (result == 0) {
        registers_.f |= Flag::Zero;
    }
    if (result < registers_.a) {
        registers_.f |= Flag::Carry;
    }
    if ((registers_.a & 0x0F) > (result & 0x0F)) {
        registers_.f |= Flag::HalfCarry;
    }

    registers_.a = result;
}

void Cpu::sub(uint8_t val, bool carry)
{
    uint8_t result = registers_.a - val;

    if (carry && (registers_.f & Flag::Carry) != 0) {
        result -= 1;
    }

    reset_flags();

    // set flags
    registers_.f |= Flag::Substract;
    if (result == 0) {
        registers_.f |= Flag::Zero;
    }
    if (result > registers_.a) {
        registers_.f |= Flag::Carry;
    }
    if ((registers_.a & 0x0F) < (result & 0x0F)) {
        registers_.f |= Flag::HalfCarry;
    }

    registers_.a = result;
}

void Cpu::aand(uint8_t val)
{
    registers_.a &= val;

    reset_flags();

    // set flags
    registers_.f |= Flag::HalfCarry;
    if (registers_.a == 0) {
        registers_.f |= Flag::Zero;
    }
}

void Cpu::xxor(uint8_t val)
{
    registers_.a ^= val;

    reset_flags();

    // set flags
    if (registers_.a == 0) {
        registers_.f |= Flag::Zero;
    }
}

void Cpu::oor(uint8_t val)
{
    registers_.a |= val;

    reset_flags();

    // set flags
    if (registers_.a == 0) {
        registers_.f |= Flag::Zero;
    }
}

void Cpu::cp(uint8_t val)
{
    reset_flags();

    // set flags
    registers_.f |= Flag::Substract;
    if (registers_.a == val) {
        registers_.f |= Flag::Zero;
    }
    else if (registers_.a > val) {
        registers_.f |= Flag::HalfCarry;
    }
    else {
        registers_.f |= Flag::Carry;
    }
}

} // namespace boyboy::cpu