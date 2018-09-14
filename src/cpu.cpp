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
        default:
            std::cout << "Instruction " + instructions[opcode].disassembly + " not implemented" << std::endl;
            break;
    }
}