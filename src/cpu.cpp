//
// Created by zepheon on 5/09/18.
//

#include "cpu.h"

void Cpu::fetch() {
    uint8_t opcode = m_mmu->readByte(m_registers.pc++);

    std::cout << "Executing instruction " + instructions[opcode].disassembly << std::endl;

    switch(opcode) {
        case 0x00: // nop
            break;
        // region 8-bit Load (LD)
        case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x47: // ld b,reg
        case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d: case 0x4f: // ld c,reg
        case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x57: // ld d,reg
        case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d: case 0x5f: // ld e,reg
        case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x67: // ld h,reg
        case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d: case 0x6f: // ld l,reg
        case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c: case 0x7d: case 0x7f: // ld a,reg
        {
            uint8_t *dst = dstReg(opcode);
            uint8_t *src = srcReg(opcode);
            *dst = *src;
            break;
        }
        case 0x06: case 0x0E: case 0x16: case 0x1E: case 0x26: case 0x2E: case 0x3E: // ld reg, d8
        {
            uint8_t value = m_mmu->readByte(m_registers.pc++);
            uint8_t *dst = dstReg(opcode);
            *dst = value;
            break;
        }
        case 0x46: case 0x4E: case 0x56: case 0x5E: case 0x66: case 0x6E: case 0x7E: // ld reg, (hl)
        {
            uint8_t *dst = dstReg(opcode);
            uint16_t src_addr = m_registers.hl;
            *dst = m_mmu->readByte(src_addr);
            break;
        }
        case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x77: // ld (hl), reg
        {
            uint16_t dst_addr = m_registers.hl;
            uint8_t *src = srcReg(opcode);
            m_mmu->writeByte(dst_addr, *src);
            break;
        }
        case 0x02: case 0x12: // ld (reg16), a
        {
            uint16_t dst_addr = *dstReg16(opcode);
            m_mmu->writeByte(dst_addr, m_registers.a);
            break;
        }
        case 0x22: // ld (hli), a
        {
            m_mmu->writeByte(m_registers.hl++, m_registers.a);
            break;
        }
        case 0x32: // ld (hld), a
        {
            m_mmu->writeByte(m_registers.hl--, m_registers.a);
            break;
        }
        case 0x0A: case 0x1A: // ld a, (reg16)
        {
            uint16_t src_addr = *srcReg16(opcode);
            m_registers.a = m_mmu->readByte(src_addr);
            break;
        }
        case 0x2A: // ld a, (hli)
        {
            m_registers.a = m_mmu->readByte(m_registers.hl++);
            break;
        }
        case 0x3A: // ld a, (hld)
        {
            m_registers.a = m_mmu->readByte(m_registers.hl--);
            break;
        }
        case 0x36: // ld (hl), d8
        {
            m_mmu->writeByte(m_registers.hl, m_mmu->readByte(m_registers.pc++));
            break;
        }
        case 0xE0: // ld (0xff00 + a8), a
        {
            uint16_t dst_addr = (uint16_t )0xFF00 + m_mmu->readByte(m_registers.pc++);
            m_mmu->writeByte(dst_addr, m_registers.a);
            break;
        }
        case 0xF0: // ld a, (0xff00 + a8)
        {
            uint16_t src_addr = (uint16_t )0xFF00 + m_mmu->readByte(m_registers.pc++);
            m_registers.a = m_mmu->readByte(src_addr);
            break;
        }
        case 0xE2: // ld (c), a
        {
            m_mmu->writeByte(m_registers.c, m_registers.a);
            break;
        }
        case 0xF2: // ld a, (c)
        {
            m_registers.a = m_mmu->readByte(m_registers.c);
            break;
        }
        case 0xEA: // ld (a16), a
        {
            uint16_t dst_addr = (m_mmu->readByte(m_registers.pc++) << 8) + m_mmu->readByte(m_registers.pc++);
            m_mmu->writeByte(dst_addr, m_registers.a);
            break;
        }
        case 0xFA: // ld a, (a16)
        {
            uint16_t src_addr = (m_mmu->readByte(m_registers.pc++) << 8) + m_mmu->readByte(m_registers.pc++);
            m_registers.a = m_mmu->readByte(src_addr);
            break;
        }
        //endregion
        //region 8-bit ALU
        case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87: // add a, reg
        case 0x88: case 0x89: case 0x8A: case 0x8B: case 0x8C: case 0x8D: case 0x8E: case 0x8F: // adc a, reg
        case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97: // sub reg
        case 0x98: case 0x99: case 0x9A: case 0x9B: case 0x9C: case 0x9D: case 0x9E: case 0x9F: // sbc reg
        case 0xA0: case 0xA1: case 0xA2: case 0xA3: case 0xA4: case 0xA5: case 0xA6: case 0xA7: // and reg
        case 0xA8: case 0xA9: case 0xAA: case 0xAB: case 0xAC: case 0xAD: case 0xAE: case 0xAF: // xor reg
        case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB6: case 0xB7: // or reg
        case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC: case 0xBD: case 0xBE: case 0xBF: // cp reg
        {
            uint8_t val;
            bool carry = false;

            // check if (hl)
            if ((opcode & 0x0F) == 0x06 || (opcode & 0x0F) == 0x0E)
                val = m_mmu->readByte(m_registers.hl);
            else
                val = *srcReg(opcode);

            if ((opcode & 0x0F) > 0x07) // adc
                carry = true;

            if (opcode < 0x90)
                add(val, carry);
            else if (opcode < 0xA0)
                sub(val, carry);
            else if (opcode < 0xA8)
                aand(val);
            else if (opcode < 0xB0)
                xxor(val);
            else if (opcode < 0xB9)
                oor(val);
            else
                cp(val);

            break;
        }
        //endregion
        default:
            std::cout << "Instruction " + instructions[opcode].disassembly + " not implemented" << std::endl;
            break;
    }
}

void Cpu::add(uint8_t val, bool carry) {
    uint8_t result = m_registers.a + val;

    if (carry && (m_registers.f & FLAG_CY))
        result += 1;

    reset_flags();

    // set flags
    if (result == 0)
        m_registers.f |= FLAG_Z;
    if (result < m_registers.a)
        m_registers.f |= FLAG_CY;
    if (m_registers.a & 0x0F > result & 0x0F)
        m_registers.f |= FLAG_H;

    m_registers.a = result;
}

void Cpu::sub(uint8_t val, bool carry) {
    uint8_t result = m_registers.a - val;

    if (carry && (m_registers.f & FLAG_CY))
        result -= 1;

    reset_flags();

    // set flags
    m_registers.f |= FLAG_N;
    if (result == 0)
        m_registers.f |= FLAG_Z;
    if (result > m_registers.a)
        m_registers.f |= FLAG_CY;
    if (m_registers.a & 0x0F < result & 0x0F)
        m_registers.f |= FLAG_H;

    m_registers.a = result;
}

void Cpu::aand(uint8_t val) {
    m_registers.a &= val;

    reset_flags();

    // set flags
    m_registers.f |= FLAG_H;
    if (m_registers.a == 0)
        m_registers.f |= FLAG_Z;
}

void Cpu::xxor(uint8_t val) {
    m_registers.a ^= val;

    reset_flags();

    // set flags
    if (m_registers.a == 0)
        m_registers.f |= FLAG_Z;
}

void Cpu::oor(uint8_t val) {
    m_registers.a |= val;

    reset_flags();

    // set flags
    if (m_registers.a == 0)
        m_registers.f |= FLAG_Z;
}

void Cpu::cp(uint8_t val) {
    reset_flags();

    // set flags
    m_registers.f |= FLAG_N;
    if (m_registers.a == val)
        m_registers.f |= FLAG_Z;
    else if (m_registers.a > val)
        m_registers.f |= FLAG_H;
    else
        m_registers.f |= FLAG_CY;
}