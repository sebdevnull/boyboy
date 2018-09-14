//
// Created by zepheon on 5/09/18.
//

#pragma once

#include <string>
#include <iostream>

#include "registers.h"
#include "mmu.h"

class Cpu {

public:

    Mmu *m_mmu;

    struct registers m_registers;
    uint8_t *regs[8] = {
            &m_registers.b,
            &m_registers.c,
            &m_registers.d,
            &m_registers.e,
            &m_registers.h,
            &m_registers.l,
            &m_registers.f,
            &m_registers.a
    };

    uint16_t *regs16[4] = {
            &m_registers.bc,
            &m_registers.de,
            &m_registers.hl,
            &m_registers.sp
    };

    Cpu(Mmu *mmu): m_mmu{mmu}{}

    typedef struct Instruction {
        std::string disassembly;
        size_t value_len;
        uint8_t cycles;
    } Instruction_t;

//    enum Opcodes : uint8_t {
//        NOP,        // 0x00
//        LD_BC_d16,  // 0x00
//        LD_pBC_A,   // 0x00
//        INC_BC,    // 0x00
//        INC_B,    // 0x00
//        DEC_B,    // 0x00
//        LD_B_d8,    // 0x00
//        RLCA,    // 0x00
//        LD_pa16_SP,    // 0x00
//        ADD_HL_BC,    // 0x00
//        LD_A_pBC,    // 0x00
//        DEC_BC,    // 0x00
//        INC_C,    // 0x00
//        DEC_C,    // 0x00
//        LD_C_d8,    // 0x00
//        RRCA,    // 0x00
//        STOP_0,    // 0x00
//        LD_DE_d16,    // 0x00
//        LD_pDE_A,    // 0x00
//        INC_DE,    // 0x00
//        INC_D,    // 0x00
//        DEC_D,    // 0x00
//        LD_D_d8,    // 0x00
//        RLA,    // 0x00
//        JR_r8,    // 0x00
//        ADD_HL_DE,    // 0x00
//        LD_A_pDE,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//        NOP,    // 0x00
//    };

    Instruction_t instructions[256] = {
            { "NOP", 0, 0},                           // 0x00
            { "LD BC, d16", 2, 0},            // 0x01
            { "LD (BC), A", 0, 0},               // 0x02
            { "INC BC", 0, 0},                     // 0x03
            { "INC B", 0, 0},                       // 0x04
            { "DEC B", 0, 0},                       // 0x05
            { "LD B, d8", 1, 0},               // 0x06
            { "RLCA", 0, 0},                         // 0x07
            { "LD (a16), SP", 2, 0},         // 0x08
            { "ADD HL, BC", 0, 0},              // 0x09
            { "LD A, (BC)", 0, 0},               // 0x0a
            { "DEC BC", 0, 0},                     // 0x0b
            { "INC C", 0, 0},                       // 0x0c
            { "DEC C", 0, 0},                       // 0x0d
            { "LD C, d8", 1, 0},               // 0x0e
            { "RRCA", 0, 0},                         // 0x0f
            { "STOP", 1, 0},                         // 0x10
            { "LD DE, d16", 2, 0},            // 0x11
            { "LD (DE), A", 0, 0},               // 0x12
            { "INC DE", 0, 0},                     // 0x13
            { "INC D", 0, 0},                       // 0x14
            { "DEC D", 0, 0},                       // 0x15
            { "LD D, d8", 1, 0},               // 0x16
            { "RLA", 0, 0},                           // 0x17
            { "JR d8", 1, 0},                    // 0x18
            { "ADD HL, DE", 0, 0},              // 0x19
            { "LD A, (DE)", 0, 0},               // 0x1a
            { "DEC DE", 0, 0},                     // 0x1b
            { "INC E", 0, 0},                       // 0x1c
            { "DEC E", 0, 0},                       // 0x1d
            { "LD E, d8", 1, 0},               // 0x1e
            { "RRA", 0, 0},                           // 0x1f
            { "JR NZ, d8", 1, 0},             // 0x20
            { "LD HL, d16", 2, 0},            // 0x21
            { "LDI (HL), A", 0, 0},             // 0x22
            { "INC HL", 0, 0},                     // 0x23
            { "INC H", 0, 0},                       // 0x24
            { "DEC H", 0, 0},                       // 0x25
            { "LD H, d8", 1, 0},               // 0x26
            { "DAA", 0, 0},                           // 0x27
            { "JR Z, d8", 1, 0},               // 0x28
            { "ADD HL, HL", 0, 0},              // 0x29
            { "LDI A, (HL)", 0, 0},             // 0x2a
            { "DEC HL", 0, 0},                     // 0x2b
            { "INC L", 0, 0},                       // 0x2c
            { "DEC L", 0, 0},                       // 0x2d
            { "LD L, d8", 1, 0},               // 0x2e
            { "CPL", 0, 0},                           // 0x2f
            { "JR NC, d8", 1, 0},             // 0x30
            { "LD SP, d16", 2, 0},            // 0x31
            { "LDD (HL), A", 0, 0},             // 0x32
            { "INC SP", 0, 0},                     // 0x33
            { "INC (HL)", 0, 0},                  // 0x34
            { "DEC (HL)", 0, 0},                  // 0x35
            { "LD (HL), d8", 1, 0},          // 0x36
            { "SCF", 0, 0},                           // 0x37
            { "JR C, d8", 1, 0},               // 0x38
            { "ADD HL, SP", 0, 0},              // 0x39
            { "LDD A, (HL)", 0, 0},             // 0x3a
            { "DEC SP", 0, 0},                     // 0x3b
            { "INC A", 0, 0},                       // 0x3c
            { "DEC A", 0, 0},                       // 0x3d
            { "LD A, d8", 1, 0},               // 0x3e
            { "CCF", 0, 0},                           // 0x3f
            { "LD B, B", 0, 0},                       // 0x40
            { "LD B, C", 0, 0},                    // 0x41
            { "LD B, D", 0, 0},                    // 0x42
            { "LD B, E", 0, 0},                    // 0x43
            { "LD B, H", 0, 0},                    // 0x44
            { "LD B, L", 0, 0},                    // 0x45
            { "LD B, (HL)", 0, 0},               // 0x46
            { "LD B, A", 0, 0},                    // 0x47
            { "LD C, B", 0, 0},                    // 0x48
            { "LD C, C", 0, 0},                       // 0x49
            { "LD C, D", 0, 0},                    // 0x4a
            { "LD C, E", 0, 0},                    // 0x4b
            { "LD C, H", 0, 0},                    // 0x4c
            { "LD C, L", 0, 0},                    // 0x4d
            { "LD C, (HL)", 0, 0},               // 0x4e
            { "LD C, A", 0, 0},                    // 0x4f
            { "LD D, B", 0, 0},                    // 0x50
            { "LD D, C", 0, 0},                    // 0x51
            { "LD D, D", 0, 0},                       // 0x52
            { "LD D, E", 0, 0},                    // 0x53
            { "LD D, H", 0, 0},                    // 0x54
            { "LD D, L", 0, 0},                    // 0x55
            { "LD D, (HL)", 0, 0},               // 0x56
            { "LD D, A", 0, 0},                    // 0x57
            { "LD E, B", 0, 0},                    // 0x58
            { "LD E, C", 0, 0},                    // 0x59
            { "LD E, D", 0, 0},                    // 0x5a
            { "LD E, E", 0, 0},                       // 0x5b
            { "LD E, H", 0, 0},                    // 0x5c
            { "LD E, L", 0, 0},                    // 0x5d
            { "LD E, (HL)", 0, 0},               // 0x5e
            { "LD E, A", 0, 0},                    // 0x5f
            { "LD H, B", 0, 0},                    // 0x60
            { "LD H, C", 0, 0},                    // 0x61
            { "LD H, D", 0, 0},                    // 0x62
            { "LD H, E", 0, 0},                    // 0x63
            { "LD H, H", 0, 0},                       // 0x64
            { "LD H, L", 0, 0},                    // 0x65
            { "LD H, (HL)", 0, 0},               // 0x66
            { "LD H, A", 0, 0},                    // 0x67
            { "LD L, B", 0, 0},                    // 0x68
            { "LD L, C", 0, 0},                    // 0x69
            { "LD L, D", 0, 0},                    // 0x6a
            { "LD L, E", 0, 0},                    // 0x6b
            { "LD L, H", 0, 0},                    // 0x6c
            { "LD L, L", 0, 0},                       // 0x6d
            { "LD L, (HL)", 0, 0},               // 0x6e
            { "LD L, A", 0, 0},                    // 0x6f
            { "LD (HL), B", 0, 0},               // 0x70
            { "LD (HL), C", 0, 0},               // 0x71
            { "LD (HL), D", 0, 0},               // 0x72
            { "LD (HL), E", 0, 0},               // 0x73
            { "LD (HL), H", 0, 0},               // 0x74
            { "LD (HL), L", 0, 0},               // 0x75
            { "HALT", 0, 0},                         // 0x76
            { "LD (HL), A", 0, 0},               // 0x77
            { "LD A, B", 0, 0},                    // 0x78
            { "LD A, C", 0, 0},                    // 0x79
            { "LD A, D", 0, 0},                    // 0x7a
            { "LD A, E", 0, 0},                    // 0x7b
            { "LD A, H", 0, 0},                    // 0x7c
            { "LD A, L", 0, 0},                    // 0x7d
            { "LD A, (HL)", 0, 0},               // 0x7e
            { "LD A, A", 0, 0},                       // 0x7f
            { "ADD A, B", 0, 0},                  // 0x80
            { "ADD A, C", 0, 0},                  // 0x81
            { "ADD A, D", 0, 0},                  // 0x82
            { "ADD A, E", 0, 0},                  // 0x83
            { "ADD A, H", 0, 0},                  // 0x84
            { "ADD A, L", 0, 0},                  // 0x85
            { "ADD A, (HL)", 0, 0},             // 0x86
            { "ADD A", 0, 0},                     // 0x87
            { "ADC B", 0, 0},                       // 0x88
            { "ADC C", 0, 0},                       // 0x89
            { "ADC D", 0, 0},                       // 0x8a
            { "ADC E", 0, 0},                       // 0x8b
            { "ADC H", 0, 0},                       // 0x8c
            { "ADC L", 0, 0},                       // 0x8d
            { "ADC (HL)", 0, 0},                  // 0x8e
            { "ADC A", 0, 0},                       // 0x8f
            { "SUB B", 0, 0},                       // 0x90
            { "SUB C", 0, 0},                       // 0x91
            { "SUB D", 0, 0},                       // 0x92
            { "SUB E", 0, 0},                       // 0x93
            { "SUB H", 0, 0},                       // 0x94
            { "SUB L", 0, 0},                       // 0x95
            { "SUB (HL)", 0, 0},                  // 0x96
            { "SUB A", 0, 0},                       // 0x97
            { "SBC B", 0, 0},                       // 0x98
            { "SBC C", 0, 0},                       // 0x99
            { "SBC D", 0, 0},                       // 0x9a
            { "SBC E", 0, 0},                       // 0x9b
            { "SBC H", 0, 0},                       // 0x9c
            { "SBC L", 0, 0},                       // 0x9d
            { "SBC (HL)", 0, 0},                  // 0x9e
            { "SBC A", 0, 0},                       // 0x9f
            { "AND B", 0, 0},                       // 0xa0
            { "AND C", 0, 0},                       // 0xa1
            { "AND D", 0, 0},                       // 0xa2
            { "AND E", 0, 0},                       // 0xa3
            { "AND H", 0, 0},                       // 0xa4
            { "AND L", 0, 0},                       // 0xa5
            { "AND (HL)", 0, 0},                  // 0xa6
            { "AND A", 0, 0},                       // 0xa7
            { "XOR B", 0, 0},                       // 0xa8
            { "XOR C", 0, 0},                       // 0xa9
            { "XOR D", 0, 0},                       // 0xaa
            { "XOR E", 0, 0},                       // 0xab
            { "XOR H", 0, 0},                       // 0xac
            { "XOR L", 0, 0},                       // 0xad
            { "XOR (HL)", 0, 0},                  // 0xae
            { "XOR A", 0, 0},                       // 0xaf
            { "OR B", 0, 0},                         // 0xb0
            { "OR C", 0, 0},                         // 0xb1
            { "OR D", 0, 0},                         // 0xb2
            { "OR E", 0, 0},                         // 0xb3
            { "OR H", 0, 0},                         // 0xb4
            { "OR L", 0, 0},                         // 0xb5
            { "OR (HL)", 0, 0},                    // 0xb6
            { "OR A", 0, 0},                         // 0xb7
            { "CP B", 0, 0},                         // 0xb8
            { "CP C", 0, 0},                         // 0xb9
            { "CP D", 0, 0},                         // 0xba
            { "CP E", 0, 0},                         // 0xbb
            { "CP H", 0, 0},                         // 0xbc
            { "CP L", 0, 0},                         // 0xbd
            { "CP (HL)", 0, 0},                    // 0xbe
            { "CP A", 0, 0},                         // 0xbf
            { "RET NZ", 0, 0},                     // 0xc0
            { "POP BC", 0, 0},                     // 0xc1
            { "JP NZ, d16", 2, 0},            // 0xc2
            { "JP d16", 2, 0},                   // 0xc3
            { "CALL NZ, d16", 2, 0},        // 0xc4
            { "PUSH BC", 0, 0},                   // 0xc5
            { "ADD A, d8", 1, 0},             // 0xc6
            { "RST 0x00", 0, 0},                    // 0xc7
            { "RET Z", 0, 0},                       // 0xc8
            { "RET", 0, 0},                           // 0xc9
            { "JP Z, d16", 2, 0},              // 0xca
            { "CB %02X", 1, 0},                      // 0xcb
            { "CALL Z, d16", 2, 0},          // 0xcc
            { "CALL d16", 2, 0},               // 0xcd
            { "ADC d8", 1, 0},                  // 0xce
            { "RST 0x08", 0, 0},                   // 0xcf
            { "RET NC", 0, 0},                     // 0xd0
            { "POP DE", 0, 0},                     // 0xd1
            { "JP NC, d16", 2, 0},            // 0xd2
            { "UNKNOWN", 0, 0},                 // 0xd3
            { "CALL NC, d16", 2, 0},        // 0xd4
            { "PUSH DE", 0, 0},                   // 0xd5
            { "SUB d8", 1, 0},                  // 0xd6
            { "RST 0x10", 0, 0},                   // 0xd7
            { "RET C", 0, 0},                       // 0xd8
            { "RETI", 0, 0},          // 0xd9
            { "JP C, d16", 2, 0},              // 0xda
            { "UNKNOWN", 0, 0},                 // 0xdb
            { "CALL C, d16", 2, 0},          // 0xdc
            { "UNKNOWN", 0, 0},                 // 0xdd
            { "SBC d8", 1, 0},                  // 0xde
            { "RST 0x18", 0, 0},                   // 0xdf
            { "LD (0xFF00 + a8), A", 1, 0},// 0xe0
            { "POP HL", 0, 0},                     // 0xe1
            { "LD (0xFF00 + C), A", 0, 0},      // 0xe2
            { "UNKNOWN", 0, 0},                 // 0xe3
            { "UNKNOWN", 0, 0},                 // 0xe4
            { "PUSH HL", 0, 0},                   // 0xe5
            { "AND d8", 1, 0},                  // 0xe6
            { "RST 0x20", 0, 0},                   // 0xe7
            { "ADD SP,d8", 1, 0},            // 0xe8
            { "JP HL", 0, 0},                       // 0xe9
            { "LD (a16), A", 2, 0},           // 0xea
            { "UNKNOWN", 0, 0},                 // 0xeb
            { "UNKNOWN", 0, 0},                 // 0xec
            { "UNKNOWN", 0, 0},                 // 0xed
            { "XOR d8", 1, 0},                  // 0xee
            { "RST 0x28", 0, 0},                   // 0xef
            { "LD A, (0xFF00 + a8)", 1, 0},// 0xf0
            { "POP AF", 0, 0},                     // 0xf1
            { "LD A, (0xFF00 + C)", 0, 0},      // 0xf2
            { "DI", 0, 0},                        // 0xf3
            { "UNKNOWN", 0, 0},                 // 0xf4
            { "PUSH AF", 0, 0},                   // 0xf5
            { "OR d8", 1, 0},                    // 0xf6
            { "RST 0x30", 0, 0},                   // 0xf7
            { "LD HL, SP+d8", 1, 0},       // 0xf8
            { "LD SP, HL", 0, 0},                // 0xf9
            { "LD A, (a16)", 2, 0},           // 0xfa
            { "EI", 0, 0},                             // 0xfb
            { "UNKNOWN", 0, 0},                 // 0xfc
            { "UNKNOWN", 0, 0},                 // 0xfd
            { "CP d8", 1, 0},                    // 0xfe
            { "RST 0x38", 0, 0}                   // 0xff
    };

    inline uint8_t *dstReg(uint8_t opcode) {
        return regs[opcode >> 3 & 7];
    }

    inline uint8_t *srcReg(uint8_t opcode) {
        return regs[opcode & 7];
    }

    inline uint16_t *dstReg16(uint8_t opcode) {
        return regs16[opcode >> 4 & 7];
    }

    inline uint16_t *srcReg16(uint8_t opcode) {
        return regs16[opcode >> 4 & 7];
    }

    void fetch();
};