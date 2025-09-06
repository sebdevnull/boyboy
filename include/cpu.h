/**
 * @file cpu.h
 * @brief CPU emulation for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <array>
#include <cstddef>
#include <string_view>

#include "mmu.h"
#include "registers.h"

namespace boyboy::cpu {

namespace Flag {

constexpr uint8_t Zero = 0x80;      // Zero flag (Z)
constexpr uint8_t Substract = 0x40; // Subtraction flag (N) (BCD)
constexpr uint8_t HalfCarry = 0x20; // Half Carry flag (H) (BCD)
constexpr uint8_t Carry = 0x10;     // Carry flag (CY)

} // namespace Flag

class Cpu {
public:
    explicit Cpu(mmu::Mmu* mmu) : mmu_{mmu}
    {
        registers_.af = 0;
        registers_.bc = 0;
        registers_.de = 0;
        registers_.hl = 0;
        registers_.sp = 0;
        registers_.pc = 0;
    }

private:
    mmu::Mmu* mmu_;
    Registers registers_{};

    // TODO: find other way to do register mapping
    // NOLINTBEGIN(cppcoreguidelines-avoid-const-or-ref-data-members)
    const std::array<uint8_t* const, 8> Regs = {
        &registers_.b,
        &registers_.c,
        &registers_.d,
        &registers_.e,
        &registers_.h,
        &registers_.l,
        &registers_.f,
        &registers_.a,
    };
    const std::array<uint16_t* const, 4> Regs16 = {
        &registers_.bc,
        &registers_.de,
        &registers_.hl,
        &registers_.sp,
    };
    // NOLINTEND(cppcoreguidelines-avoid-const-or-ref-data-members)

    uint8_t* dst_reg(const uint8_t opcode) { return Regs.at(opcode >> 3 & 7); }
    uint8_t* src_reg(uint8_t opcode) { return Regs.at(opcode & 7); }
    uint16_t* dst_reg16(uint8_t opcode) { return Regs16.at(opcode >> 4 & 7); }
    uint16_t* src_reg16(uint8_t opcode) { return Regs16.at(opcode >> 4 & 7); }

    void reset_flags() { registers_.f &= 0; }

    void add(uint8_t val, bool carry);
    void sub(uint8_t val, bool carry);
    void aand(uint8_t val);
    void xxor(uint8_t val);
    void oor(uint8_t val);
    void cp(uint8_t val);

    void fetch();
};

struct Instruction {
    std::string_view disassembly;
    size_t value_len;
    uint8_t cycles;
};

constexpr std::array<Instruction, 256> InstructionTable = {{
    {.disassembly = "NOP", .value_len = 0, .cycles = 0},                 // 0x00
    {.disassembly = "LD BC, d16", .value_len = 2, .cycles = 0},          // 0x01
    {.disassembly = "LD (BC), A", .value_len = 0, .cycles = 0},          // 0x02
    {.disassembly = "INC BC", .value_len = 0, .cycles = 0},              // 0x03
    {.disassembly = "INC B", .value_len = 0, .cycles = 0},               // 0x04
    {.disassembly = "DEC B", .value_len = 0, .cycles = 0},               // 0x05
    {.disassembly = "LD B, d8", .value_len = 1, .cycles = 0},            // 0x06
    {.disassembly = "RLCA", .value_len = 0, .cycles = 0},                // 0x07
    {.disassembly = "LD (a16), SP", .value_len = 2, .cycles = 0},        // 0x08
    {.disassembly = "ADD HL, BC", .value_len = 0, .cycles = 0},          // 0x09
    {.disassembly = "LD A, (BC)", .value_len = 0, .cycles = 0},          // 0x0a
    {.disassembly = "DEC BC", .value_len = 0, .cycles = 0},              // 0x0b
    {.disassembly = "INC C", .value_len = 0, .cycles = 0},               // 0x0c
    {.disassembly = "DEC C", .value_len = 0, .cycles = 0},               // 0x0d
    {.disassembly = "LD C, d8", .value_len = 1, .cycles = 0},            // 0x0e
    {.disassembly = "RRCA", .value_len = 0, .cycles = 0},                // 0x0f
    {.disassembly = "STOP", .value_len = 1, .cycles = 0},                // 0x10
    {.disassembly = "LD DE, d16", .value_len = 2, .cycles = 0},          // 0x11
    {.disassembly = "LD (DE), A", .value_len = 0, .cycles = 0},          // 0x12
    {.disassembly = "INC DE", .value_len = 0, .cycles = 0},              // 0x13
    {.disassembly = "INC D", .value_len = 0, .cycles = 0},               // 0x14
    {.disassembly = "DEC D", .value_len = 0, .cycles = 0},               // 0x15
    {.disassembly = "LD D, d8", .value_len = 1, .cycles = 0},            // 0x16
    {.disassembly = "RLA", .value_len = 0, .cycles = 0},                 // 0x17
    {.disassembly = "JR d8", .value_len = 1, .cycles = 0},               // 0x18
    {.disassembly = "ADD HL, DE", .value_len = 0, .cycles = 0},          // 0x19
    {.disassembly = "LD A, (DE)", .value_len = 0, .cycles = 0},          // 0x1a
    {.disassembly = "DEC DE", .value_len = 0, .cycles = 0},              // 0x1b
    {.disassembly = "INC E", .value_len = 0, .cycles = 0},               // 0x1c
    {.disassembly = "DEC E", .value_len = 0, .cycles = 0},               // 0x1d
    {.disassembly = "LD E, d8", .value_len = 1, .cycles = 0},            // 0x1e
    {.disassembly = "RRA", .value_len = 0, .cycles = 0},                 // 0x1f
    {.disassembly = "JR NZ, d8", .value_len = 1, .cycles = 0},           // 0x20
    {.disassembly = "LD HL, d16", .value_len = 2, .cycles = 0},          // 0x21
    {.disassembly = "LDI (HL), A", .value_len = 0, .cycles = 0},         // 0x22
    {.disassembly = "INC HL", .value_len = 0, .cycles = 0},              // 0x23
    {.disassembly = "INC H", .value_len = 0, .cycles = 0},               // 0x24
    {.disassembly = "DEC H", .value_len = 0, .cycles = 0},               // 0x25
    {.disassembly = "LD H, d8", .value_len = 1, .cycles = 0},            // 0x26
    {.disassembly = "DAA", .value_len = 0, .cycles = 0},                 // 0x27
    {.disassembly = "JR Z, d8", .value_len = 1, .cycles = 0},            // 0x28
    {.disassembly = "ADD HL, HL", .value_len = 0, .cycles = 0},          // 0x29
    {.disassembly = "LDI A, (HL)", .value_len = 0, .cycles = 0},         // 0x2a
    {.disassembly = "DEC HL", .value_len = 0, .cycles = 0},              // 0x2b
    {.disassembly = "INC L", .value_len = 0, .cycles = 0},               // 0x2c
    {.disassembly = "DEC L", .value_len = 0, .cycles = 0},               // 0x2d
    {.disassembly = "LD L, d8", .value_len = 1, .cycles = 0},            // 0x2e
    {.disassembly = "CPL", .value_len = 0, .cycles = 0},                 // 0x2f
    {.disassembly = "JR NC, d8", .value_len = 1, .cycles = 0},           // 0x30
    {.disassembly = "LD SP, d16", .value_len = 2, .cycles = 0},          // 0x31
    {.disassembly = "LDD (HL), A", .value_len = 0, .cycles = 0},         // 0x32
    {.disassembly = "INC SP", .value_len = 0, .cycles = 0},              // 0x33
    {.disassembly = "INC (HL)", .value_len = 0, .cycles = 0},            // 0x34
    {.disassembly = "DEC (HL)", .value_len = 0, .cycles = 0},            // 0x35
    {.disassembly = "LD (HL), d8", .value_len = 1, .cycles = 0},         // 0x36
    {.disassembly = "SCF", .value_len = 0, .cycles = 0},                 // 0x37
    {.disassembly = "JR C, d8", .value_len = 1, .cycles = 0},            // 0x38
    {.disassembly = "ADD HL, SP", .value_len = 0, .cycles = 0},          // 0x39
    {.disassembly = "LDD A, (HL)", .value_len = 0, .cycles = 0},         // 0x3a
    {.disassembly = "DEC SP", .value_len = 0, .cycles = 0},              // 0x3b
    {.disassembly = "INC A", .value_len = 0, .cycles = 0},               // 0x3c
    {.disassembly = "DEC A", .value_len = 0, .cycles = 0},               // 0x3d
    {.disassembly = "LD A, d8", .value_len = 1, .cycles = 0},            // 0x3e
    {.disassembly = "CCF", .value_len = 0, .cycles = 0},                 // 0x3f
    {.disassembly = "LD B, B", .value_len = 0, .cycles = 0},             // 0x40
    {.disassembly = "LD B, C", .value_len = 0, .cycles = 0},             // 0x41
    {.disassembly = "LD B, D", .value_len = 0, .cycles = 0},             // 0x42
    {.disassembly = "LD B, E", .value_len = 0, .cycles = 0},             // 0x43
    {.disassembly = "LD B, H", .value_len = 0, .cycles = 0},             // 0x44
    {.disassembly = "LD B, L", .value_len = 0, .cycles = 0},             // 0x45
    {.disassembly = "LD B, (HL)", .value_len = 0, .cycles = 0},          // 0x46
    {.disassembly = "LD B, A", .value_len = 0, .cycles = 0},             // 0x47
    {.disassembly = "LD C, B", .value_len = 0, .cycles = 0},             // 0x48
    {.disassembly = "LD C, C", .value_len = 0, .cycles = 0},             // 0x49
    {.disassembly = "LD C, D", .value_len = 0, .cycles = 0},             // 0x4a
    {.disassembly = "LD C, E", .value_len = 0, .cycles = 0},             // 0x4b
    {.disassembly = "LD C, H", .value_len = 0, .cycles = 0},             // 0x4c
    {.disassembly = "LD C, L", .value_len = 0, .cycles = 0},             // 0x4d
    {.disassembly = "LD C, (HL)", .value_len = 0, .cycles = 0},          // 0x4e
    {.disassembly = "LD C, A", .value_len = 0, .cycles = 0},             // 0x4f
    {.disassembly = "LD D, B", .value_len = 0, .cycles = 0},             // 0x50
    {.disassembly = "LD D, C", .value_len = 0, .cycles = 0},             // 0x51
    {.disassembly = "LD D, D", .value_len = 0, .cycles = 0},             // 0x52
    {.disassembly = "LD D, E", .value_len = 0, .cycles = 0},             // 0x53
    {.disassembly = "LD D, H", .value_len = 0, .cycles = 0},             // 0x54
    {.disassembly = "LD D, L", .value_len = 0, .cycles = 0},             // 0x55
    {.disassembly = "LD D, (HL)", .value_len = 0, .cycles = 0},          // 0x56
    {.disassembly = "LD D, A", .value_len = 0, .cycles = 0},             // 0x57
    {.disassembly = "LD E, B", .value_len = 0, .cycles = 0},             // 0x58
    {.disassembly = "LD E, C", .value_len = 0, .cycles = 0},             // 0x59
    {.disassembly = "LD E, D", .value_len = 0, .cycles = 0},             // 0x5a
    {.disassembly = "LD E, E", .value_len = 0, .cycles = 0},             // 0x5b
    {.disassembly = "LD E, H", .value_len = 0, .cycles = 0},             // 0x5c
    {.disassembly = "LD E, L", .value_len = 0, .cycles = 0},             // 0x5d
    {.disassembly = "LD E, (HL)", .value_len = 0, .cycles = 0},          // 0x5e
    {.disassembly = "LD E, A", .value_len = 0, .cycles = 0},             // 0x5f
    {.disassembly = "LD H, B", .value_len = 0, .cycles = 0},             // 0x60
    {.disassembly = "LD H, C", .value_len = 0, .cycles = 0},             // 0x61
    {.disassembly = "LD H, D", .value_len = 0, .cycles = 0},             // 0x62
    {.disassembly = "LD H, E", .value_len = 0, .cycles = 0},             // 0x63
    {.disassembly = "LD H, H", .value_len = 0, .cycles = 0},             // 0x64
    {.disassembly = "LD H, L", .value_len = 0, .cycles = 0},             // 0x65
    {.disassembly = "LD H, (HL)", .value_len = 0, .cycles = 0},          // 0x66
    {.disassembly = "LD H, A", .value_len = 0, .cycles = 0},             // 0x67
    {.disassembly = "LD L, B", .value_len = 0, .cycles = 0},             // 0x68
    {.disassembly = "LD L, C", .value_len = 0, .cycles = 0},             // 0x69
    {.disassembly = "LD L, D", .value_len = 0, .cycles = 0},             // 0x6a
    {.disassembly = "LD L, E", .value_len = 0, .cycles = 0},             // 0x6b
    {.disassembly = "LD L, H", .value_len = 0, .cycles = 0},             // 0x6c
    {.disassembly = "LD L, L", .value_len = 0, .cycles = 0},             // 0x6d
    {.disassembly = "LD L, (HL)", .value_len = 0, .cycles = 0},          // 0x6e
    {.disassembly = "LD L, A", .value_len = 0, .cycles = 0},             // 0x6f
    {.disassembly = "LD (HL), B", .value_len = 0, .cycles = 0},          // 0x70
    {.disassembly = "LD (HL), C", .value_len = 0, .cycles = 0},          // 0x71
    {.disassembly = "LD (HL), D", .value_len = 0, .cycles = 0},          // 0x72
    {.disassembly = "LD (HL), E", .value_len = 0, .cycles = 0},          // 0x73
    {.disassembly = "LD (HL), H", .value_len = 0, .cycles = 0},          // 0x74
    {.disassembly = "LD (HL), L", .value_len = 0, .cycles = 0},          // 0x75
    {.disassembly = "HALT", .value_len = 0, .cycles = 0},                // 0x76
    {.disassembly = "LD (HL), A", .value_len = 0, .cycles = 0},          // 0x77
    {.disassembly = "LD A, B", .value_len = 0, .cycles = 0},             // 0x78
    {.disassembly = "LD A, C", .value_len = 0, .cycles = 0},             // 0x79
    {.disassembly = "LD A, D", .value_len = 0, .cycles = 0},             // 0x7a
    {.disassembly = "LD A, E", .value_len = 0, .cycles = 0},             // 0x7b
    {.disassembly = "LD A, H", .value_len = 0, .cycles = 0},             // 0x7c
    {.disassembly = "LD A, L", .value_len = 0, .cycles = 0},             // 0x7d
    {.disassembly = "LD A, (HL)", .value_len = 0, .cycles = 0},          // 0x7e
    {.disassembly = "LD A, A", .value_len = 0, .cycles = 0},             // 0x7f
    {.disassembly = "ADD A, B", .value_len = 0, .cycles = 0},            // 0x80
    {.disassembly = "ADD A, C", .value_len = 0, .cycles = 0},            // 0x81
    {.disassembly = "ADD A, D", .value_len = 0, .cycles = 0},            // 0x82
    {.disassembly = "ADD A, E", .value_len = 0, .cycles = 0},            // 0x83
    {.disassembly = "ADD A, H", .value_len = 0, .cycles = 0},            // 0x84
    {.disassembly = "ADD A, L", .value_len = 0, .cycles = 0},            // 0x85
    {.disassembly = "ADD A, (HL)", .value_len = 0, .cycles = 0},         // 0x86
    {.disassembly = "ADD A", .value_len = 0, .cycles = 0},               // 0x87
    {.disassembly = "ADC B", .value_len = 0, .cycles = 0},               // 0x88
    {.disassembly = "ADC C", .value_len = 0, .cycles = 0},               // 0x89
    {.disassembly = "ADC D", .value_len = 0, .cycles = 0},               // 0x8a
    {.disassembly = "ADC E", .value_len = 0, .cycles = 0},               // 0x8b
    {.disassembly = "ADC H", .value_len = 0, .cycles = 0},               // 0x8c
    {.disassembly = "ADC L", .value_len = 0, .cycles = 0},               // 0x8d
    {.disassembly = "ADC (HL)", .value_len = 0, .cycles = 0},            // 0x8e
    {.disassembly = "ADC A", .value_len = 0, .cycles = 0},               // 0x8f
    {.disassembly = "SUB B", .value_len = 0, .cycles = 0},               // 0x90
    {.disassembly = "SUB C", .value_len = 0, .cycles = 0},               // 0x91
    {.disassembly = "SUB D", .value_len = 0, .cycles = 0},               // 0x92
    {.disassembly = "SUB E", .value_len = 0, .cycles = 0},               // 0x93
    {.disassembly = "SUB H", .value_len = 0, .cycles = 0},               // 0x94
    {.disassembly = "SUB L", .value_len = 0, .cycles = 0},               // 0x95
    {.disassembly = "SUB (HL)", .value_len = 0, .cycles = 0},            // 0x96
    {.disassembly = "SUB A", .value_len = 0, .cycles = 0},               // 0x97
    {.disassembly = "SBC B", .value_len = 0, .cycles = 0},               // 0x98
    {.disassembly = "SBC C", .value_len = 0, .cycles = 0},               // 0x99
    {.disassembly = "SBC D", .value_len = 0, .cycles = 0},               // 0x9a
    {.disassembly = "SBC E", .value_len = 0, .cycles = 0},               // 0x9b
    {.disassembly = "SBC H", .value_len = 0, .cycles = 0},               // 0x9c
    {.disassembly = "SBC L", .value_len = 0, .cycles = 0},               // 0x9d
    {.disassembly = "SBC (HL)", .value_len = 0, .cycles = 0},            // 0x9e
    {.disassembly = "SBC A", .value_len = 0, .cycles = 0},               // 0x9f
    {.disassembly = "AND B", .value_len = 0, .cycles = 0},               // 0xa0
    {.disassembly = "AND C", .value_len = 0, .cycles = 0},               // 0xa1
    {.disassembly = "AND D", .value_len = 0, .cycles = 0},               // 0xa2
    {.disassembly = "AND E", .value_len = 0, .cycles = 0},               // 0xa3
    {.disassembly = "AND H", .value_len = 0, .cycles = 0},               // 0xa4
    {.disassembly = "AND L", .value_len = 0, .cycles = 0},               // 0xa5
    {.disassembly = "AND (HL)", .value_len = 0, .cycles = 0},            // 0xa6
    {.disassembly = "AND A", .value_len = 0, .cycles = 0},               // 0xa7
    {.disassembly = "XOR B", .value_len = 0, .cycles = 0},               // 0xa8
    {.disassembly = "XOR C", .value_len = 0, .cycles = 0},               // 0xa9
    {.disassembly = "XOR D", .value_len = 0, .cycles = 0},               // 0xaa
    {.disassembly = "XOR E", .value_len = 0, .cycles = 0},               // 0xab
    {.disassembly = "XOR H", .value_len = 0, .cycles = 0},               // 0xac
    {.disassembly = "XOR L", .value_len = 0, .cycles = 0},               // 0xad
    {.disassembly = "XOR (HL)", .value_len = 0, .cycles = 0},            // 0xae
    {.disassembly = "XOR A", .value_len = 0, .cycles = 0},               // 0xaf
    {.disassembly = "OR B", .value_len = 0, .cycles = 0},                // 0xb0
    {.disassembly = "OR C", .value_len = 0, .cycles = 0},                // 0xb1
    {.disassembly = "OR D", .value_len = 0, .cycles = 0},                // 0xb2
    {.disassembly = "OR E", .value_len = 0, .cycles = 0},                // 0xb3
    {.disassembly = "OR H", .value_len = 0, .cycles = 0},                // 0xb4
    {.disassembly = "OR L", .value_len = 0, .cycles = 0},                // 0xb5
    {.disassembly = "OR (HL)", .value_len = 0, .cycles = 0},             // 0xb6
    {.disassembly = "OR A", .value_len = 0, .cycles = 0},                // 0xb7
    {.disassembly = "CP B", .value_len = 0, .cycles = 0},                // 0xb8
    {.disassembly = "CP C", .value_len = 0, .cycles = 0},                // 0xb9
    {.disassembly = "CP D", .value_len = 0, .cycles = 0},                // 0xba
    {.disassembly = "CP E", .value_len = 0, .cycles = 0},                // 0xbb
    {.disassembly = "CP H", .value_len = 0, .cycles = 0},                // 0xbc
    {.disassembly = "CP L", .value_len = 0, .cycles = 0},                // 0xbd
    {.disassembly = "CP (HL)", .value_len = 0, .cycles = 0},             // 0xbe
    {.disassembly = "CP A", .value_len = 0, .cycles = 0},                // 0xbf
    {.disassembly = "RET NZ", .value_len = 0, .cycles = 0},              // 0xc0
    {.disassembly = "POP BC", .value_len = 0, .cycles = 0},              // 0xc1
    {.disassembly = "JP NZ, d16", .value_len = 2, .cycles = 0},          // 0xc2
    {.disassembly = "JP d16", .value_len = 2, .cycles = 0},              // 0xc3
    {.disassembly = "CALL NZ, d16", .value_len = 2, .cycles = 0},        // 0xc4
    {.disassembly = "PUSH BC", .value_len = 0, .cycles = 0},             // 0xc5
    {.disassembly = "ADD A, d8", .value_len = 1, .cycles = 0},           // 0xc6
    {.disassembly = "RST 0x00", .value_len = 0, .cycles = 0},            // 0xc7
    {.disassembly = "RET Z", .value_len = 0, .cycles = 0},               // 0xc8
    {.disassembly = "RET", .value_len = 0, .cycles = 0},                 // 0xc9
    {.disassembly = "JP Z, d16", .value_len = 2, .cycles = 0},           // 0xca
    {.disassembly = "CB %02X", .value_len = 1, .cycles = 0},             // 0xcb
    {.disassembly = "CALL Z, d16", .value_len = 2, .cycles = 0},         // 0xcc
    {.disassembly = "CALL d16", .value_len = 2, .cycles = 0},            // 0xcd
    {.disassembly = "ADC d8", .value_len = 1, .cycles = 0},              // 0xce
    {.disassembly = "RST 0x08", .value_len = 0, .cycles = 0},            // 0xcf
    {.disassembly = "RET NC", .value_len = 0, .cycles = 0},              // 0xd0
    {.disassembly = "POP DE", .value_len = 0, .cycles = 0},              // 0xd1
    {.disassembly = "JP NC, d16", .value_len = 2, .cycles = 0},          // 0xd2
    {.disassembly = "UNKNOWN", .value_len = 0, .cycles = 0},             // 0xd3
    {.disassembly = "CALL NC, d16", .value_len = 2, .cycles = 0},        // 0xd4
    {.disassembly = "PUSH DE", .value_len = 0, .cycles = 0},             // 0xd5
    {.disassembly = "SUB d8", .value_len = 1, .cycles = 0},              // 0xd6
    {.disassembly = "RST 0x10", .value_len = 0, .cycles = 0},            // 0xd7
    {.disassembly = "RET C", .value_len = 0, .cycles = 0},               // 0xd8
    {.disassembly = "RETI", .value_len = 0, .cycles = 0},                // 0xd9
    {.disassembly = "JP C, d16", .value_len = 2, .cycles = 0},           // 0xda
    {.disassembly = "UNKNOWN", .value_len = 0, .cycles = 0},             // 0xdb
    {.disassembly = "CALL C, d16", .value_len = 2, .cycles = 0},         // 0xdc
    {.disassembly = "UNKNOWN", .value_len = 0, .cycles = 0},             // 0xdd
    {.disassembly = "SBC d8", .value_len = 1, .cycles = 0},              // 0xde
    {.disassembly = "RST 0x18", .value_len = 0, .cycles = 0},            // 0xdf
    {.disassembly = "LD (0xFF00 + a8), A", .value_len = 1, .cycles = 0}, // 0xe0
    {.disassembly = "POP HL", .value_len = 0, .cycles = 0},              // 0xe1
    {.disassembly = "LD (0xFF00 + C), A", .value_len = 0, .cycles = 0},  // 0xe2
    {.disassembly = "UNKNOWN", .value_len = 0, .cycles = 0},             // 0xe3
    {.disassembly = "UNKNOWN", .value_len = 0, .cycles = 0},             // 0xe4
    {.disassembly = "PUSH HL", .value_len = 0, .cycles = 0},             // 0xe5
    {.disassembly = "AND d8", .value_len = 1, .cycles = 0},              // 0xe6
    {.disassembly = "RST 0x20", .value_len = 0, .cycles = 0},            // 0xe7
    {.disassembly = "ADD SP,d8", .value_len = 1, .cycles = 0},           // 0xe8
    {.disassembly = "JP HL", .value_len = 0, .cycles = 0},               // 0xe9
    {.disassembly = "LD (a16), A", .value_len = 2, .cycles = 0},         // 0xea
    {.disassembly = "UNKNOWN", .value_len = 0, .cycles = 0},             // 0xeb
    {.disassembly = "UNKNOWN", .value_len = 0, .cycles = 0},             // 0xec
    {.disassembly = "UNKNOWN", .value_len = 0, .cycles = 0},             // 0xed
    {.disassembly = "XOR d8", .value_len = 1, .cycles = 0},              // 0xee
    {.disassembly = "RST 0x28", .value_len = 0, .cycles = 0},            // 0xef
    {.disassembly = "LD A, (0xFF00 + a8)", .value_len = 1, .cycles = 0}, // 0xf0
    {.disassembly = "POP AF", .value_len = 0, .cycles = 0},              // 0xf1
    {.disassembly = "LD A, (0xFF00 + C)", .value_len = 0, .cycles = 0},  // 0xf2
    {.disassembly = "DI", .value_len = 0, .cycles = 0},                  // 0xf3
    {.disassembly = "UNKNOWN", .value_len = 0, .cycles = 0},             // 0xf4
    {.disassembly = "PUSH AF", .value_len = 0, .cycles = 0},             // 0xf5
    {.disassembly = "OR d8", .value_len = 1, .cycles = 0},               // 0xf6
    {.disassembly = "RST 0x30", .value_len = 0, .cycles = 0},            // 0xf7
    {.disassembly = "LD HL, SP+d8", .value_len = 1, .cycles = 0},        // 0xf8
    {.disassembly = "LD SP, HL", .value_len = 0, .cycles = 0},           // 0xf9
    {.disassembly = "LD A, (a16)", .value_len = 2, .cycles = 0},         // 0xfa
    {.disassembly = "EI", .value_len = 0, .cycles = 0},                  // 0xfb
    {.disassembly = "UNKNOWN", .value_len = 0, .cycles = 0},             // 0xfc
    {.disassembly = "UNKNOWN", .value_len = 0, .cycles = 0},             // 0xfd
    {.disassembly = "CP d8", .value_len = 1, .cycles = 0},               // 0xfe
    {.disassembly = "RST 0x38", .value_len = 0, .cycles = 0}             // 0xff
}};

} // namespace boyboy::cpu