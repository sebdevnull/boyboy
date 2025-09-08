/**
 * @file cpu_stubs.h
 * @brief CPU instruction stubs for unimplemented opcodes tests.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstdint>
#include <vector>

#include "boyboy/cpu/cpu.h" // needed for stub disabling macros

// Regex for opcode extraction: ^void.+\n\{\n.+Opcode\((0x\w{2}).+\n\}

// List of unimplemented opcodes for testing purposes
const std::vector<std::uint8_t> UnprefixedStubOpcodes = {
#ifndef CPU_NOP
    0x00,
#endif // CPU_NOP
#ifndef CPU_LD_BC_N16
    0x01,
#endif // CPU_LD_BC_N16
#ifndef CPU_LD_AT_BC_A
    0x02,
#endif // CPU_LD_AT_BC_A
#ifndef CPU_INC_BC
    0x03,
#endif // CPU_INC_BC
#ifndef CPU_INC_B
    0x04,
#endif // CPU_INC_B
#ifndef CPU_DEC_B
    0x05,
#endif // CPU_DEC_B
#ifndef CPU_LD_B_N8
    0x06,
#endif // CPU_LD_B_N8
#ifndef CPU_RLCA
    0x07,
#endif // CPU_RLCA
#ifndef CPU_LD_AT_A16_SP
    0x08,
#endif // CPU_LD_AT_A16_SP
#ifndef CPU_ADD_HL_BC
    0x09,
#endif // CPU_ADD_HL_BC
#ifndef CPU_LD_A_AT_BC
    0x0A,
#endif // CPU_LD_A_AT_BC
#ifndef CPU_DEC_BC
    0x0B,
#endif // CPU_DEC_BC
#ifndef CPU_INC_C
    0x0C,
#endif // CPU_INC_C
#ifndef CPU_DEC_C
    0x0D,
#endif // CPU_DEC_C
#ifndef CPU_LD_C_N8
    0x0E,
#endif // CPU_LD_C_N8
#ifndef CPU_RRCA
    0x0F,
#endif // CPU_RRCA
#ifndef CPU_STOP_N8
    0x10,
#endif // CPU_STOP_N8
#ifndef CPU_LD_DE_N16
    0x11,
#endif // CPU_LD_DE_N16
#ifndef CPU_LD_AT_DE_A
    0x12,
#endif // CPU_LD_AT_DE_A
#ifndef CPU_INC_DE
    0x13,
#endif // CPU_INC_DE
#ifndef CPU_INC_D
    0x14,
#endif // CPU_INC_D
#ifndef CPU_DEC_D
    0x15,
#endif // CPU_DEC_D
#ifndef CPU_LD_D_N8
    0x16,
#endif // CPU_LD_D_N8
#ifndef CPU_RLA
    0x17,
#endif // CPU_RLA
#ifndef CPU_JR_E8
    0x18,
#endif // CPU_JR_E8
#ifndef CPU_ADD_HL_DE
    0x19,
#endif // CPU_ADD_HL_DE
#ifndef CPU_LD_A_AT_DE
    0x1A,
#endif // CPU_LD_A_AT_DE
#ifndef CPU_DEC_DE
    0x1B,
#endif // CPU_DEC_DE
#ifndef CPU_INC_E
    0x1C,
#endif // CPU_INC_E
#ifndef CPU_DEC_E
    0x1D,
#endif // CPU_DEC_E
#ifndef CPU_LD_E_N8
    0x1E,
#endif // CPU_LD_E_N8
#ifndef CPU_RRA
    0x1F,
#endif // CPU_RRA
#ifndef CPU_JR_NZ_E8
    0x20,
#endif // CPU_JR_NZ_E8
#ifndef CPU_LD_HL_N16
    0x21,
#endif // CPU_LD_HL_N16
#ifndef CPU_LD_AT_HL_INC_A
    0x22,
#endif // CPU_LD_AT_HL_INC_A
#ifndef CPU_INC_HL
    0x23,
#endif // CPU_INC_HL
#ifndef CPU_INC_H
    0x24,
#endif // CPU_INC_H
#ifndef CPU_DEC_H
    0x25,
#endif // CPU_DEC_H
#ifndef CPU_LD_H_N8
    0x26,
#endif // CPU_LD_H_N8
#ifndef CPU_DAA
    0x27,
#endif // CPU_DAA
#ifndef CPU_JR_Z_E8
    0x28,
#endif // CPU_JR_Z_E8
#ifndef CPU_ADD_HL_HL
    0x29,
#endif // CPU_ADD_HL_HL
#ifndef CPU_LD_A_AT_HL_INC
    0x2A,
#endif // CPU_LD_A_AT_HL_INC
#ifndef CPU_DEC_HL
    0x2B,
#endif // CPU_DEC_HL
#ifndef CPU_INC_L
    0x2C,
#endif // CPU_INC_L
#ifndef CPU_DEC_L
    0x2D,
#endif // CPU_DEC_L
#ifndef CPU_LD_L_N8
    0x2E,
#endif // CPU_LD_L_N8
#ifndef CPU_CPL
    0x2F,
#endif // CPU_CPL
#ifndef CPU_JR_NC_E8
    0x30,
#endif // CPU_JR_NC_E8
#ifndef CPU_LD_SP_N16
    0x31,
#endif // CPU_LD_SP_N16
#ifndef CPU_LD_AT_HL_DEC_A
    0x32,
#endif // CPU_LD_AT_HL_DEC_A
#ifndef CPU_INC_SP
    0x33,
#endif // CPU_INC_SP
#ifndef CPU_INC_AT_HL
    0x34,
#endif // CPU_INC_AT_HL
#ifndef CPU_DEC_AT_HL
    0x35,
#endif // CPU_DEC_AT_HL
#ifndef CPU_LD_AT_HL_N8
    0x36,
#endif // CPU_LD_AT_HL_N8
#ifndef CPU_SCF
    0x37,
#endif // CPU_SCF
#ifndef CPU_JR_C_E8
    0x38,
#endif // CPU_JR_C_E8
#ifndef CPU_ADD_HL_SP
    0x39,
#endif // CPU_ADD_HL_SP
#ifndef CPU_LD_A_AT_HL_DEC
    0x3A,
#endif // CPU_LD_A_AT_HL_DEC
#ifndef CPU_DEC_SP
    0x3B,
#endif // CPU_DEC_SP
#ifndef CPU_INC_A
    0x3C,
#endif // CPU_INC_A
#ifndef CPU_DEC_A
    0x3D,
#endif // CPU_DEC_A
#ifndef CPU_LD_A_N8
    0x3E,
#endif // CPU_LD_A_N8
#ifndef CPU_CCF
    0x3F,
#endif // CPU_CCF
#ifndef CPU_LD_B_B
    0x40,
#endif // CPU_LD_B_B
#ifndef CPU_LD_B_C
    0x41,
#endif // CPU_LD_B_C
#ifndef CPU_LD_B_D
    0x42,
#endif // CPU_LD_B_D
#ifndef CPU_LD_B_E
    0x43,
#endif // CPU_LD_B_E
#ifndef CPU_LD_B_H
    0x44,
#endif // CPU_LD_B_H
#ifndef CPU_LD_B_L
    0x45,
#endif // CPU_LD_B_L
#ifndef CPU_LD_B_AT_HL
    0x46,
#endif // CPU_LD_B_AT_HL
#ifndef CPU_LD_B_A
    0x47,
#endif // CPU_LD_B_A
#ifndef CPU_LD_C_B
    0x48,
#endif // CPU_LD_C_B
#ifndef CPU_LD_C_C
    0x49,
#endif // CPU_LD_C_C
#ifndef CPU_LD_C_D
    0x4A,
#endif // CPU_LD_C_D
#ifndef CPU_LD_C_E
    0x4B,
#endif // CPU_LD_C_E
#ifndef CPU_LD_C_H
    0x4C,
#endif // CPU_LD_C_H
#ifndef CPU_LD_C_L
    0x4D,
#endif // CPU_LD_C_L
#ifndef CPU_LD_C_AT_HL
    0x4E,
#endif // CPU_LD_C_AT_HL
#ifndef CPU_LD_C_A
    0x4F,
#endif // CPU_LD_C_A
#ifndef CPU_LD_D_B
    0x50,
#endif // CPU_LD_D_B
#ifndef CPU_LD_D_C
    0x51,
#endif // CPU_LD_D_C
#ifndef CPU_LD_D_D
    0x52,
#endif // CPU_LD_D_D
#ifndef CPU_LD_D_E
    0x53,
#endif // CPU_LD_D_E
#ifndef CPU_LD_D_H
    0x54,
#endif // CPU_LD_D_H
#ifndef CPU_LD_D_L
    0x55,
#endif // CPU_LD_D_L
#ifndef CPU_LD_D_AT_HL
    0x56,
#endif // CPU_LD_D_AT_HL
#ifndef CPU_LD_D_A
    0x57,
#endif // CPU_LD_D_A
#ifndef CPU_LD_E_B
    0x58,
#endif // CPU_LD_E_B
#ifndef CPU_LD_E_C
    0x59,
#endif // CPU_LD_E_C
#ifndef CPU_LD_E_D
    0x5A,
#endif // CPU_LD_E_D
#ifndef CPU_LD_E_E
    0x5B,
#endif // CPU_LD_E_E
#ifndef CPU_LD_E_H
    0x5C,
#endif // CPU_LD_E_H
#ifndef CPU_LD_E_L
    0x5D,
#endif // CPU_LD_E_L
#ifndef CPU_LD_E_AT_HL
    0x5E,
#endif // CPU_LD_E_AT_HL
#ifndef CPU_LD_E_A
    0x5F,
#endif // CPU_LD_E_A
#ifndef CPU_LD_H_B
    0x60,
#endif // CPU_LD_H_B
#ifndef CPU_LD_H_C
    0x61,
#endif // CPU_LD_H_C
#ifndef CPU_LD_H_D
    0x62,
#endif // CPU_LD_H_D
#ifndef CPU_LD_H_E
    0x63,
#endif // CPU_LD_H_E
#ifndef CPU_LD_H_H
    0x64,
#endif // CPU_LD_H_H
#ifndef CPU_LD_H_L
    0x65,
#endif // CPU_LD_H_L
#ifndef CPU_LD_H_AT_HL
    0x66,
#endif // CPU_LD_H_AT_HL
#ifndef CPU_LD_H_A
    0x67,
#endif // CPU_LD_H_A
#ifndef CPU_LD_L_B
    0x68,
#endif // CPU_LD_L_B
#ifndef CPU_LD_L_C
    0x69,
#endif // CPU_LD_L_C
#ifndef CPU_LD_L_D
    0x6A,
#endif // CPU_LD_L_D
#ifndef CPU_LD_L_E
    0x6B,
#endif // CPU_LD_L_E
#ifndef CPU_LD_L_H
    0x6C,
#endif // CPU_LD_L_H
#ifndef CPU_LD_L_L
    0x6D,
#endif // CPU_LD_L_L
#ifndef CPU_LD_L_AT_HL
    0x6E,
#endif // CPU_LD_L_AT_HL
#ifndef CPU_LD_L_A
    0x6F,
#endif // CPU_LD_L_A
#ifndef CPU_LD_AT_HL_B
    0x70,
#endif // CPU_LD_AT_HL_B
#ifndef CPU_LD_AT_HL_C
    0x71,
#endif // CPU_LD_AT_HL_C
#ifndef CPU_LD_AT_HL_D
    0x72,
#endif // CPU_LD_AT_HL_D
#ifndef CPU_LD_AT_HL_E
    0x73,
#endif // CPU_LD_AT_HL_E
#ifndef CPU_LD_AT_HL_H
    0x74,
#endif // CPU_LD_AT_HL_H
#ifndef CPU_LD_AT_HL_L
    0x75,
#endif // CPU_LD_AT_HL_L
#ifndef CPU_HALT
    0x76,
#endif // CPU_HALT
#ifndef CPU_LD_AT_HL_A
    0x77,
#endif // CPU_LD_AT_HL_A
#ifndef CPU_LD_A_B
    0x78,
#endif // CPU_LD_A_B
#ifndef CPU_LD_A_C
    0x79,
#endif // CPU_LD_A_C
#ifndef CPU_LD_A_D
    0x7A,
#endif // CPU_LD_A_D
#ifndef CPU_LD_A_E
    0x7B,
#endif // CPU_LD_A_E
#ifndef CPU_LD_A_H
    0x7C,
#endif // CPU_LD_A_H
#ifndef CPU_LD_A_L
    0x7D,
#endif // CPU_LD_A_L
#ifndef CPU_LD_A_AT_HL
    0x7E,
#endif // CPU_LD_A_AT_HL
#ifndef CPU_LD_A_A
    0x7F,
#endif // CPU_LD_A_A
#ifndef CPU_ADD_A_B
    0x80,
#endif // CPU_ADD_A_B
#ifndef CPU_ADD_A_C
    0x81,
#endif // CPU_ADD_A_C
#ifndef CPU_ADD_A_D
    0x82,
#endif // CPU_ADD_A_D
#ifndef CPU_ADD_A_E
    0x83,
#endif // CPU_ADD_A_E
#ifndef CPU_ADD_A_H
    0x84,
#endif // CPU_ADD_A_H
#ifndef CPU_ADD_A_L
    0x85,
#endif // CPU_ADD_A_L
#ifndef CPU_ADD_A_AT_HL
    0x86,
#endif // CPU_ADD_A_AT_HL
#ifndef CPU_ADD_A_A
    0x87,
#endif // CPU_ADD_A_A
#ifndef CPU_ADC_A_B
    0x88,
#endif // CPU_ADC_A_B
#ifndef CPU_ADC_A_C
    0x89,
#endif // CPU_ADC_A_C
#ifndef CPU_ADC_A_D
    0x8A,
#endif // CPU_ADC_A_D
#ifndef CPU_ADC_A_E
    0x8B,
#endif // CPU_ADC_A_E
#ifndef CPU_ADC_A_H
    0x8C,
#endif // CPU_ADC_A_H
#ifndef CPU_ADC_A_L
    0x8D,
#endif // CPU_ADC_A_L
#ifndef CPU_ADC_A_AT_HL
    0x8E,
#endif // CPU_ADC_A_AT_HL
#ifndef CPU_ADC_A_A
    0x8F,
#endif // CPU_ADC_A_A
#ifndef CPU_SUB_A_B
    0x90,
#endif // CPU_SUB_A_B
#ifndef CPU_SUB_A_C
    0x91,
#endif // CPU_SUB_A_C
#ifndef CPU_SUB_A_D
    0x92,
#endif // CPU_SUB_A_D
#ifndef CPU_SUB_A_E
    0x93,
#endif // CPU_SUB_A_E
#ifndef CPU_SUB_A_H
    0x94,
#endif // CPU_SUB_A_H
#ifndef CPU_SUB_A_L
    0x95,
#endif // CPU_SUB_A_L
#ifndef CPU_SUB_A_AT_HL
    0x96,
#endif // CPU_SUB_A_AT_HL
#ifndef CPU_SUB_A_A
    0x97,
#endif // CPU_SUB_A_A
#ifndef CPU_SBC_A_B
    0x98,
#endif // CPU_SBC_A_B
#ifndef CPU_SBC_A_C
    0x99,
#endif // CPU_SBC_A_C
#ifndef CPU_SBC_A_D
    0x9A,
#endif // CPU_SBC_A_D
#ifndef CPU_SBC_A_E
    0x9B,
#endif // CPU_SBC_A_E
#ifndef CPU_SBC_A_H
    0x9C,
#endif // CPU_SBC_A_H
#ifndef CPU_SBC_A_L
    0x9D,
#endif // CPU_SBC_A_L
#ifndef CPU_SBC_A_AT_HL
    0x9E,
#endif // CPU_SBC_A_AT_HL
#ifndef CPU_SBC_A_A
    0x9F,
#endif // CPU_SBC_A_A
#ifndef CPU_AND_A_B
    0xA0,
#endif // CPU_AND_A_B
#ifndef CPU_AND_A_C
    0xA1,
#endif // CPU_AND_A_C
#ifndef CPU_AND_A_D
    0xA2,
#endif // CPU_AND_A_D
#ifndef CPU_AND_A_E
    0xA3,
#endif // CPU_AND_A_E
#ifndef CPU_AND_A_H
    0xA4,
#endif // CPU_AND_A_H
#ifndef CPU_AND_A_L
    0xA5,
#endif // CPU_AND_A_L
#ifndef CPU_AND_A_AT_HL
    0xA6,
#endif // CPU_AND_A_AT_HL
#ifndef CPU_AND_A_A
    0xA7,
#endif // CPU_AND_A_A
#ifndef CPU_XOR_A_B
    0xA8,
#endif // CPU_XOR_A_B
#ifndef CPU_XOR_A_C
    0xA9,
#endif // CPU_XOR_A_C
#ifndef CPU_XOR_A_D
    0xAA,
#endif // CPU_XOR_A_D
#ifndef CPU_XOR_A_E
    0xAB,
#endif // CPU_XOR_A_E
#ifndef CPU_XOR_A_H
    0xAC,
#endif // CPU_XOR_A_H
#ifndef CPU_XOR_A_L
    0xAD,
#endif // CPU_XOR_A_L
#ifndef CPU_XOR_A_AT_HL
    0xAE,
#endif // CPU_XOR_A_AT_HL
#ifndef CPU_XOR_A_A
    0xAF,
#endif // CPU_XOR_A_A
#ifndef CPU_OR_A_B
    0xB0,
#endif // CPU_OR_A_B
#ifndef CPU_OR_A_C
    0xB1,
#endif // CPU_OR_A_C
#ifndef CPU_OR_A_D
    0xB2,
#endif // CPU_OR_A_D
#ifndef CPU_OR_A_E
    0xB3,
#endif // CPU_OR_A_E
#ifndef CPU_OR_A_H
    0xB4,
#endif // CPU_OR_A_H
#ifndef CPU_OR_A_L
    0xB5,
#endif // CPU_OR_A_L
#ifndef CPU_OR_A_AT_HL
    0xB6,
#endif // CPU_OR_A_AT_HL
#ifndef CPU_OR_A_A
    0xB7,
#endif // CPU_OR_A_A
#ifndef CPU_CP_A_B
    0xB8,
#endif // CPU_CP_A_B
#ifndef CPU_CP_A_C
    0xB9,
#endif // CPU_CP_A_C
#ifndef CPU_CP_A_D
    0xBA,
#endif // CPU_CP_A_D
#ifndef CPU_CP_A_E
    0xBB,
#endif // CPU_CP_A_E
#ifndef CPU_CP_A_H
    0xBC,
#endif // CPU_CP_A_H
#ifndef CPU_CP_A_L
    0xBD,
#endif // CPU_CP_A_L
#ifndef CPU_CP_A_AT_HL
    0xBE,
#endif // CPU_CP_A_AT_HL
#ifndef CPU_CP_A_A
    0xBF,
#endif // CPU_CP_A_A
#ifndef CPU_RET_NZ
    0xC0,
#endif // CPU_RET_NZ
#ifndef CPU_POP_BC
    0xC1,
#endif // CPU_POP_BC
#ifndef CPU_JP_NZ_A16
    0xC2,
#endif // CPU_JP_NZ_A16
#ifndef CPU_JP_A16
    0xC3,
#endif // CPU_JP_A16
#ifndef CPU_CALL_NZ_A16
    0xC4,
#endif // CPU_CALL_NZ_A16
#ifndef CPU_PUSH_BC
    0xC5,
#endif // CPU_PUSH_BC
#ifndef CPU_ADD_A_N8
    0xC6,
#endif // CPU_ADD_A_N8
#ifndef CPU_RST_00
    0xC7,
#endif // CPU_RST_00
#ifndef CPU_RET_Z
    0xC8,
#endif // CPU_RET_Z
#ifndef CPU_RET
    0xC9,
#endif // CPU_RET
#ifndef CPU_JP_Z_A16
    0xCA,
#endif // CPU_JP_Z_A16
#ifndef CPU_PREFIX
    0xCB,
#endif // CPU_PREFIX
#ifndef CPU_CALL_Z_A16
    0xCC,
#endif // CPU_CALL_Z_A16
#ifndef CPU_CALL_A16
    0xCD,
#endif // CPU_CALL_A16
#ifndef CPU_ADC_A_N8
    0xCE,
#endif // CPU_ADC_A_N8
#ifndef CPU_RST_08
    0xCF,
#endif // CPU_RST_08
#ifndef CPU_RET_NC
    0xD0,
#endif // CPU_RET_NC
#ifndef CPU_POP_DE
    0xD1,
#endif // CPU_POP_DE
#ifndef CPU_JP_NC_A16
    0xD2,
#endif // CPU_JP_NC_A16
#ifndef CPU_ILLEGAL_D3
    0xD3,
#endif // CPU_ILLEGAL_D3
#ifndef CPU_CALL_NC_A16
    0xD4,
#endif // CPU_CALL_NC_A16
#ifndef CPU_PUSH_DE
    0xD5,
#endif // CPU_PUSH_DE
#ifndef CPU_SUB_A_N8
    0xD6,
#endif // CPU_SUB_A_N8
#ifndef CPU_RST_10
    0xD7,
#endif // CPU_RST_10
#ifndef CPU_RET_C
    0xD8,
#endif // CPU_RET_C
#ifndef CPU_RETI
    0xD9,
#endif // CPU_RETI
#ifndef CPU_JP_C_A16
    0xDA,
#endif // CPU_JP_C_A16
#ifndef CPU_ILLEGAL_DB
    0xDB,
#endif // CPU_ILLEGAL_DB
#ifndef CPU_CALL_C_A16
    0xDC,
#endif // CPU_CALL_C_A16
#ifndef CPU_ILLEGAL_DD
    0xDD,
#endif // CPU_ILLEGAL_DD
#ifndef CPU_SBC_A_N8
    0xDE,
#endif // CPU_SBC_A_N8
#ifndef CPU_RST_18
    0xDF,
#endif // CPU_RST_18
#ifndef CPU_LDH_AT_A8_A
    0xE0,
#endif // CPU_LDH_AT_A8_A
#ifndef CPU_POP_HL
    0xE1,
#endif // CPU_POP_HL
#ifndef CPU_LDH_AT_C_A
    0xE2,
#endif // CPU_LDH_AT_C_A
#ifndef CPU_ILLEGAL_E3
    0xE3,
#endif // CPU_ILLEGAL_E3
#ifndef CPU_ILLEGAL_E4
    0xE4,
#endif // CPU_ILLEGAL_E4
#ifndef CPU_PUSH_HL
    0xE5,
#endif // CPU_PUSH_HL
#ifndef CPU_AND_A_N8
    0xE6,
#endif // CPU_AND_A_N8
#ifndef CPU_RST_20
    0xE7,
#endif // CPU_RST_20
#ifndef CPU_ADD_SP_E8
    0xE8,
#endif // CPU_ADD_SP_E8
#ifndef CPU_JP_HL
    0xE9,
#endif // CPU_JP_HL
#ifndef CPU_LD_AT_A16_A
    0xEA,
#endif // CPU_LD_AT_A16_A
#ifndef CPU_ILLEGAL_EB
    0xEB,
#endif // CPU_ILLEGAL_EB
#ifndef CPU_ILLEGAL_EC
    0xEC,
#endif // CPU_ILLEGAL_EC
#ifndef CPU_ILLEGAL_ED
    0xED,
#endif // CPU_ILLEGAL_ED
#ifndef CPU_XOR_A_N8
    0xEE,
#endif // CPU_XOR_A_N8
#ifndef CPU_RST_28
    0xEF,
#endif // CPU_RST_28
#ifndef CPU_LDH_A_AT_A8
    0xF0,
#endif // CPU_LDH_A_AT_A8
#ifndef CPU_POP_AF
    0xF1,
#endif // CPU_POP_AF
#ifndef CPU_LDH_A_AT_C
    0xF2,
#endif // CPU_LDH_A_AT_C
#ifndef CPU_DI
    0xF3,
#endif // CPU_DI
#ifndef CPU_ILLEGAL_F4
    0xF4,
#endif // CPU_ILLEGAL_F4
#ifndef CPU_PUSH_AF
    0xF5,
#endif // CPU_PUSH_AF
#ifndef CPU_OR_A_N8
    0xF6,
#endif // CPU_OR_A_N8
#ifndef CPU_RST_30
    0xF7,
#endif // CPU_RST_30
#ifndef CPU_LD_HL_SP_INC_E8
    0xF8,
#endif // CPU_LD_HL_SP_INC_E8
#ifndef CPU_LD_SP_HL
    0xF9,
#endif // CPU_LD_SP_HL
#ifndef CPU_LD_A_AT_A16
    0xFA,
#endif // CPU_LD_A_AT_A16
#ifndef CPU_EI
    0xFB,
#endif // CPU_EI
#ifndef CPU_ILLEGAL_FC
    0xFC,
#endif // CPU_ILLEGAL_FC
#ifndef CPU_ILLEGAL_FD
    0xFD,
#endif // CPU_ILLEGAL_FD
#ifndef CPU_CP_A_N8
    0xFE,
#endif // CPU_CP_A_N8
#ifndef CPU_RST_38
    0xFF,
#endif // CPU_RST_38
};

const std::vector<std::uint8_t> CBPrefixedStubOpcodes = {
#ifndef CPU_RLC_B
    0x00,
#endif // CPU_RLC_B
#ifndef CPU_RLC_C
    0x01,
#endif // CPU_RLC_C
#ifndef CPU_RLC_D
    0x02,
#endif // CPU_RLC_D
#ifndef CPU_RLC_E
    0x03,
#endif // CPU_RLC_E
#ifndef CPU_RLC_H
    0x04,
#endif // CPU_RLC_H
#ifndef CPU_RLC_L
    0x05,
#endif // CPU_RLC_L
#ifndef CPU_RLC_AT_HL
    0x06,
#endif // CPU_RLC_AT_HL
#ifndef CPU_RLC_A
    0x07,
#endif // CPU_RLC_A
#ifndef CPU_RRC_B
    0x08,
#endif // CPU_RRC_B
#ifndef CPU_RRC_C
    0x09,
#endif // CPU_RRC_C
#ifndef CPU_RRC_D
    0x0A,
#endif // CPU_RRC_D
#ifndef CPU_RRC_E
    0x0B,
#endif // CPU_RRC_E
#ifndef CPU_RRC_H
    0x0C,
#endif // CPU_RRC_H
#ifndef CPU_RRC_L
    0x0D,
#endif // CPU_RRC_L
#ifndef CPU_RRC_AT_HL
    0x0E,
#endif // CPU_RRC_AT_HL
#ifndef CPU_RRC_A
    0x0F,
#endif // CPU_RRC_A
#ifndef CPU_RL_B
    0x10,
#endif // CPU_RL_B
#ifndef CPU_RL_C
    0x11,
#endif // CPU_RL_C
#ifndef CPU_RL_D
    0x12,
#endif // CPU_RL_D
#ifndef CPU_RL_E
    0x13,
#endif // CPU_RL_E
#ifndef CPU_RL_H
    0x14,
#endif // CPU_RL_H
#ifndef CPU_RL_L
    0x15,
#endif // CPU_RL_L
#ifndef CPU_RL_AT_HL
    0x16,
#endif // CPU_RL_AT_HL
#ifndef CPU_RL_A
    0x17,
#endif // CPU_RL_A
#ifndef CPU_RR_B
    0x18,
#endif // CPU_RR_B
#ifndef CPU_RR_C
    0x19,
#endif // CPU_RR_C
#ifndef CPU_RR_D
    0x1A,
#endif // CPU_RR_D
#ifndef CPU_RR_E
    0x1B,
#endif // CPU_RR_E
#ifndef CPU_RR_H
    0x1C,
#endif // CPU_RR_H
#ifndef CPU_RR_L
    0x1D,
#endif // CPU_RR_L
#ifndef CPU_RR_AT_HL
    0x1E,
#endif // CPU_RR_AT_HL
#ifndef CPU_RR_A
    0x1F,
#endif // CPU_RR_A
#ifndef CPU_SLA_B
    0x20,
#endif // CPU_SLA_B
#ifndef CPU_SLA_C
    0x21,
#endif // CPU_SLA_C
#ifndef CPU_SLA_D
    0x22,
#endif // CPU_SLA_D
#ifndef CPU_SLA_E
    0x23,
#endif // CPU_SLA_E
#ifndef CPU_SLA_H
    0x24,
#endif // CPU_SLA_H
#ifndef CPU_SLA_L
    0x25,
#endif // CPU_SLA_L
#ifndef CPU_SLA_AT_HL
    0x26,
#endif // CPU_SLA_AT_HL
#ifndef CPU_SLA_A
    0x27,
#endif // CPU_SLA_A
#ifndef CPU_SRA_B
    0x28,
#endif // CPU_SRA_B
#ifndef CPU_SRA_C
    0x29,
#endif // CPU_SRA_C
#ifndef CPU_SRA_D
    0x2A,
#endif // CPU_SRA_D
#ifndef CPU_SRA_E
    0x2B,
#endif // CPU_SRA_E
#ifndef CPU_SRA_H
    0x2C,
#endif // CPU_SRA_H
#ifndef CPU_SRA_L
    0x2D,
#endif // CPU_SRA_L
#ifndef CPU_SRA_AT_HL
    0x2E,
#endif // CPU_SRA_AT_HL
#ifndef CPU_SRA_A
    0x2F,
#endif // CPU_SRA_A
#ifndef CPU_SWAP_B
    0x30,
#endif // CPU_SWAP_B
#ifndef CPU_SWAP_C
    0x31,
#endif // CPU_SWAP_C
#ifndef CPU_SWAP_D
    0x32,
#endif // CPU_SWAP_D
#ifndef CPU_SWAP_E
    0x33,
#endif // CPU_SWAP_E
#ifndef CPU_SWAP_H
    0x34,
#endif // CPU_SWAP_H
#ifndef CPU_SWAP_L
    0x35,
#endif // CPU_SWAP_L
#ifndef CPU_SWAP_AT_HL
    0x36,
#endif // CPU_SWAP_AT_HL
#ifndef CPU_SWAP_A
    0x37,
#endif // CPU_SWAP_A
#ifndef CPU_SRL_B
    0x38,
#endif // CPU_SRL_B
#ifndef CPU_SRL_C
    0x39,
#endif // CPU_SRL_C
#ifndef CPU_SRL_D
    0x3A,
#endif // CPU_SRL_D
#ifndef CPU_SRL_E
    0x3B,
#endif // CPU_SRL_E
#ifndef CPU_SRL_H
    0x3C,
#endif // CPU_SRL_H
#ifndef CPU_SRL_L
    0x3D,
#endif // CPU_SRL_L
#ifndef CPU_SRL_AT_HL
    0x3E,
#endif // CPU_SRL_AT_HL
#ifndef CPU_SRL_A
    0x3F,
#endif // CPU_SRL_A
#ifndef CPU_BIT_0_B
    0x40,
#endif // CPU_BIT_0_B
#ifndef CPU_BIT_0_C
    0x41,
#endif // CPU_BIT_0_C
#ifndef CPU_BIT_0_D
    0x42,
#endif // CPU_BIT_0_D
#ifndef CPU_BIT_0_E
    0x43,
#endif // CPU_BIT_0_E
#ifndef CPU_BIT_0_H
    0x44,
#endif // CPU_BIT_0_H
#ifndef CPU_BIT_0_L
    0x45,
#endif // CPU_BIT_0_L
#ifndef CPU_BIT_0_AT_HL
    0x46,
#endif // CPU_BIT_0_AT_HL
#ifndef CPU_BIT_0_A
    0x47,
#endif // CPU_BIT_0_A
#ifndef CPU_BIT_1_B
    0x48,
#endif // CPU_BIT_1_B
#ifndef CPU_BIT_1_C
    0x49,
#endif // CPU_BIT_1_C
#ifndef CPU_BIT_1_D
    0x4A,
#endif // CPU_BIT_1_D
#ifndef CPU_BIT_1_E
    0x4B,
#endif // CPU_BIT_1_E
#ifndef CPU_BIT_1_H
    0x4C,
#endif // CPU_BIT_1_H
#ifndef CPU_BIT_1_L
    0x4D,
#endif // CPU_BIT_1_L
#ifndef CPU_BIT_1_AT_HL
    0x4E,
#endif // CPU_BIT_1_AT_HL
#ifndef CPU_BIT_1_A
    0x4F,
#endif // CPU_BIT_1_A
#ifndef CPU_BIT_2_B
    0x50,
#endif // CPU_BIT_2_B
#ifndef CPU_BIT_2_C
    0x51,
#endif // CPU_BIT_2_C
#ifndef CPU_BIT_2_D
    0x52,
#endif // CPU_BIT_2_D
#ifndef CPU_BIT_2_E
    0x53,
#endif // CPU_BIT_2_E
#ifndef CPU_BIT_2_H
    0x54,
#endif // CPU_BIT_2_H
#ifndef CPU_BIT_2_L
    0x55,
#endif // CPU_BIT_2_L
#ifndef CPU_BIT_2_AT_HL
    0x56,
#endif // CPU_BIT_2_AT_HL
#ifndef CPU_BIT_2_A
    0x57,
#endif // CPU_BIT_2_A
#ifndef CPU_BIT_3_B
    0x58,
#endif // CPU_BIT_3_B
#ifndef CPU_BIT_3_C
    0x59,
#endif // CPU_BIT_3_C
#ifndef CPU_BIT_3_D
    0x5A,
#endif // CPU_BIT_3_D
#ifndef CPU_BIT_3_E
    0x5B,
#endif // CPU_BIT_3_E
#ifndef CPU_BIT_3_H
    0x5C,
#endif // CPU_BIT_3_H
#ifndef CPU_BIT_3_L
    0x5D,
#endif // CPU_BIT_3_L
#ifndef CPU_BIT_3_AT_HL
    0x5E,
#endif // CPU_BIT_3_AT_HL
#ifndef CPU_BIT_3_A
    0x5F,
#endif // CPU_BIT_3_A
#ifndef CPU_BIT_4_B
    0x60,
#endif // CPU_BIT_4_B
#ifndef CPU_BIT_4_C
    0x61,
#endif // CPU_BIT_4_C
#ifndef CPU_BIT_4_D
    0x62,
#endif // CPU_BIT_4_D
#ifndef CPU_BIT_4_E
    0x63,
#endif // CPU_BIT_4_E
#ifndef CPU_BIT_4_H
    0x64,
#endif // CPU_BIT_4_H
#ifndef CPU_BIT_4_L
    0x65,
#endif // CPU_BIT_4_L
#ifndef CPU_BIT_4_AT_HL
    0x66,
#endif // CPU_BIT_4_AT_HL
#ifndef CPU_BIT_4_A
    0x67,
#endif // CPU_BIT_4_A
#ifndef CPU_BIT_5_B
    0x68,
#endif // CPU_BIT_5_B
#ifndef CPU_BIT_5_C
    0x69,
#endif // CPU_BIT_5_C
#ifndef CPU_BIT_5_D
    0x6A,
#endif // CPU_BIT_5_D
#ifndef CPU_BIT_5_E
    0x6B,
#endif // CPU_BIT_5_E
#ifndef CPU_BIT_5_H
    0x6C,
#endif // CPU_BIT_5_H
#ifndef CPU_BIT_5_L
    0x6D,
#endif // CPU_BIT_5_L
#ifndef CPU_BIT_5_AT_HL
    0x6E,
#endif // CPU_BIT_5_AT_HL
#ifndef CPU_BIT_5_A
    0x6F,
#endif // CPU_BIT_5_A
#ifndef CPU_BIT_6_B
    0x70,
#endif // CPU_BIT_6_B
#ifndef CPU_BIT_6_C
    0x71,
#endif // CPU_BIT_6_C
#ifndef CPU_BIT_6_D
    0x72,
#endif // CPU_BIT_6_D
#ifndef CPU_BIT_6_E
    0x73,
#endif // CPU_BIT_6_E
#ifndef CPU_BIT_6_H
    0x74,
#endif // CPU_BIT_6_H
#ifndef CPU_BIT_6_L
    0x75,
#endif // CPU_BIT_6_L
#ifndef CPU_BIT_6_AT_HL
    0x76,
#endif // CPU_BIT_6_AT_HL
#ifndef CPU_BIT_6_A
    0x77,
#endif // CPU_BIT_6_A
#ifndef CPU_BIT_7_B
    0x78,
#endif // CPU_BIT_7_B
#ifndef CPU_BIT_7_C
    0x79,
#endif // CPU_BIT_7_C
#ifndef CPU_BIT_7_D
    0x7A,
#endif // CPU_BIT_7_D
#ifndef CPU_BIT_7_E
    0x7B,
#endif // CPU_BIT_7_E
#ifndef CPU_BIT_7_H
    0x7C,
#endif // CPU_BIT_7_H
#ifndef CPU_BIT_7_L
    0x7D,
#endif // CPU_BIT_7_L
#ifndef CPU_BIT_7_AT_HL
    0x7E,
#endif // CPU_BIT_7_AT_HL
#ifndef CPU_BIT_7_A
    0x7F,
#endif // CPU_BIT_7_A
#ifndef CPU_RES_0_B
    0x80,
#endif // CPU_RES_0_B
#ifndef CPU_RES_0_C
    0x81,
#endif // CPU_RES_0_C
#ifndef CPU_RES_0_D
    0x82,
#endif // CPU_RES_0_D
#ifndef CPU_RES_0_E
    0x83,
#endif // CPU_RES_0_E
#ifndef CPU_RES_0_H
    0x84,
#endif // CPU_RES_0_H
#ifndef CPU_RES_0_L
    0x85,
#endif // CPU_RES_0_L
#ifndef CPU_RES_0_AT_HL
    0x86,
#endif // CPU_RES_0_AT_HL
#ifndef CPU_RES_0_A
    0x87,
#endif // CPU_RES_0_A
#ifndef CPU_RES_1_B
    0x88,
#endif // CPU_RES_1_B
#ifndef CPU_RES_1_C
    0x89,
#endif // CPU_RES_1_C
#ifndef CPU_RES_1_D
    0x8A,
#endif // CPU_RES_1_D
#ifndef CPU_RES_1_E
    0x8B,
#endif // CPU_RES_1_E
#ifndef CPU_RES_1_H
    0x8C,
#endif // CPU_RES_1_H
#ifndef CPU_RES_1_L
    0x8D,
#endif // CPU_RES_1_L
#ifndef CPU_RES_1_AT_HL
    0x8E,
#endif // CPU_RES_1_AT_HL
#ifndef CPU_RES_1_A
    0x8F,
#endif // CPU_RES_1_A
#ifndef CPU_RES_2_B
    0x90,
#endif // CPU_RES_2_B
#ifndef CPU_RES_2_C
    0x91,
#endif // CPU_RES_2_C
#ifndef CPU_RES_2_D
    0x92,
#endif // CPU_RES_2_D
#ifndef CPU_RES_2_E
    0x93,
#endif // CPU_RES_2_E
#ifndef CPU_RES_2_H
    0x94,
#endif // CPU_RES_2_H
#ifndef CPU_RES_2_L
    0x95,
#endif // CPU_RES_2_L
#ifndef CPU_RES_2_AT_HL
    0x96,
#endif // CPU_RES_2_AT_HL
#ifndef CPU_RES_2_A
    0x97,
#endif // CPU_RES_2_A
#ifndef CPU_RES_3_B
    0x98,
#endif // CPU_RES_3_B
#ifndef CPU_RES_3_C
    0x99,
#endif // CPU_RES_3_C
#ifndef CPU_RES_3_D
    0x9A,
#endif // CPU_RES_3_D
#ifndef CPU_RES_3_E
    0x9B,
#endif // CPU_RES_3_E
#ifndef CPU_RES_3_H
    0x9C,
#endif // CPU_RES_3_H
#ifndef CPU_RES_3_L
    0x9D,
#endif // CPU_RES_3_L
#ifndef CPU_RES_3_AT_HL
    0x9E,
#endif // CPU_RES_3_AT_HL
#ifndef CPU_RES_3_A
    0x9F,
#endif // CPU_RES_3_A
#ifndef CPU_RES_4_B
    0xA0,
#endif // CPU_RES_4_B
#ifndef CPU_RES_4_C
    0xA1,
#endif // CPU_RES_4_C
#ifndef CPU_RES_4_D
    0xA2,
#endif // CPU_RES_4_D
#ifndef CPU_RES_4_E
    0xA3,
#endif // CPU_RES_4_E
#ifndef CPU_RES_4_H
    0xA4,
#endif // CPU_RES_4_H
#ifndef CPU_RES_4_L
    0xA5,
#endif // CPU_RES_4_L
#ifndef CPU_RES_4_AT_HL
    0xA6,
#endif // CPU_RES_4_AT_HL
#ifndef CPU_RES_4_A
    0xA7,
#endif // CPU_RES_4_A
#ifndef CPU_RES_5_B
    0xA8,
#endif // CPU_RES_5_B
#ifndef CPU_RES_5_C
    0xA9,
#endif // CPU_RES_5_C
#ifndef CPU_RES_5_D
    0xAA,
#endif // CPU_RES_5_D
#ifndef CPU_RES_5_E
    0xAB,
#endif // CPU_RES_5_E
#ifndef CPU_RES_5_H
    0xAC,
#endif // CPU_RES_5_H
#ifndef CPU_RES_5_L
    0xAD,
#endif // CPU_RES_5_L
#ifndef CPU_RES_5_AT_HL
    0xAE,
#endif // CPU_RES_5_AT_HL
#ifndef CPU_RES_5_A
    0xAF,
#endif // CPU_RES_5_A
#ifndef CPU_RES_6_B
    0xB0,
#endif // CPU_RES_6_B
#ifndef CPU_RES_6_C
    0xB1,
#endif // CPU_RES_6_C
#ifndef CPU_RES_6_D
    0xB2,
#endif // CPU_RES_6_D
#ifndef CPU_RES_6_E
    0xB3,
#endif // CPU_RES_6_E
#ifndef CPU_RES_6_H
    0xB4,
#endif // CPU_RES_6_H
#ifndef CPU_RES_6_L
    0xB5,
#endif // CPU_RES_6_L
#ifndef CPU_RES_6_AT_HL
    0xB6,
#endif // CPU_RES_6_AT_HL
#ifndef CPU_RES_6_A
    0xB7,
#endif // CPU_RES_6_A
#ifndef CPU_RES_7_B
    0xB8,
#endif // CPU_RES_7_B
#ifndef CPU_RES_7_C
    0xB9,
#endif // CPU_RES_7_C
#ifndef CPU_RES_7_D
    0xBA,
#endif // CPU_RES_7_D
#ifndef CPU_RES_7_E
    0xBB,
#endif // CPU_RES_7_E
#ifndef CPU_RES_7_H
    0xBC,
#endif // CPU_RES_7_H
#ifndef CPU_RES_7_L
    0xBD,
#endif // CPU_RES_7_L
#ifndef CPU_RES_7_AT_HL
    0xBE,
#endif // CPU_RES_7_AT_HL
#ifndef CPU_RES_7_A
    0xBF,
#endif // CPU_RES_7_A
#ifndef CPU_SET_0_B
    0xC0,
#endif // CPU_SET_0_B
#ifndef CPU_SET_0_C
    0xC1,
#endif // CPU_SET_0_C
#ifndef CPU_SET_0_D
    0xC2,
#endif // CPU_SET_0_D
#ifndef CPU_SET_0_E
    0xC3,
#endif // CPU_SET_0_E
#ifndef CPU_SET_0_H
    0xC4,
#endif // CPU_SET_0_H
#ifndef CPU_SET_0_L
    0xC5,
#endif // CPU_SET_0_L
#ifndef CPU_SET_0_AT_HL
    0xC6,
#endif // CPU_SET_0_AT_HL
#ifndef CPU_SET_0_A
    0xC7,
#endif // CPU_SET_0_A
#ifndef CPU_SET_1_B
    0xC8,
#endif // CPU_SET_1_B
#ifndef CPU_SET_1_C
    0xC9,
#endif // CPU_SET_1_C
#ifndef CPU_SET_1_D
    0xCA,
#endif // CPU_SET_1_D
#ifndef CPU_SET_1_E
    0xCB,
#endif // CPU_SET_1_E
#ifndef CPU_SET_1_H
    0xCC,
#endif // CPU_SET_1_H
#ifndef CPU_SET_1_L
    0xCD,
#endif // CPU_SET_1_L
#ifndef CPU_SET_1_AT_HL
    0xCE,
#endif // CPU_SET_1_AT_HL
#ifndef CPU_SET_1_A
    0xCF,
#endif // CPU_SET_1_A
#ifndef CPU_SET_2_B
    0xD0,
#endif // CPU_SET_2_B
#ifndef CPU_SET_2_C
    0xD1,
#endif // CPU_SET_2_C
#ifndef CPU_SET_2_D
    0xD2,
#endif // CPU_SET_2_D
#ifndef CPU_SET_2_E
    0xD3,
#endif // CPU_SET_2_E
#ifndef CPU_SET_2_H
    0xD4,
#endif // CPU_SET_2_H
#ifndef CPU_SET_2_L
    0xD5,
#endif // CPU_SET_2_L
#ifndef CPU_SET_2_AT_HL
    0xD6,
#endif // CPU_SET_2_AT_HL
#ifndef CPU_SET_2_A
    0xD7,
#endif // CPU_SET_2_A
#ifndef CPU_SET_3_B
    0xD8,
#endif // CPU_SET_3_B
#ifndef CPU_SET_3_C
    0xD9,
#endif // CPU_SET_3_C
#ifndef CPU_SET_3_D
    0xDA,
#endif // CPU_SET_3_D
#ifndef CPU_SET_3_E
    0xDB,
#endif // CPU_SET_3_E
#ifndef CPU_SET_3_H
    0xDC,
#endif // CPU_SET_3_H
#ifndef CPU_SET_3_L
    0xDD,
#endif // CPU_SET_3_L
#ifndef CPU_SET_3_AT_HL
    0xDE,
#endif // CPU_SET_3_AT_HL
#ifndef CPU_SET_3_A
    0xDF,
#endif // CPU_SET_3_A
#ifndef CPU_SET_4_B
    0xE0,
#endif // CPU_SET_4_B
#ifndef CPU_SET_4_C
    0xE1,
#endif // CPU_SET_4_C
#ifndef CPU_SET_4_D
    0xE2,
#endif // CPU_SET_4_D
#ifndef CPU_SET_4_E
    0xE3,
#endif // CPU_SET_4_E
#ifndef CPU_SET_4_H
    0xE4,
#endif // CPU_SET_4_H
#ifndef CPU_SET_4_L
    0xE5,
#endif // CPU_SET_4_L
#ifndef CPU_SET_4_AT_HL
    0xE6,
#endif // CPU_SET_4_AT_HL
#ifndef CPU_SET_4_A
    0xE7,
#endif // CPU_SET_4_A
#ifndef CPU_SET_5_B
    0xE8,
#endif // CPU_SET_5_B
#ifndef CPU_SET_5_C
    0xE9,
#endif // CPU_SET_5_C
#ifndef CPU_SET_5_D
    0xEA,
#endif // CPU_SET_5_D
#ifndef CPU_SET_5_E
    0xEB,
#endif // CPU_SET_5_E
#ifndef CPU_SET_5_H
    0xEC,
#endif // CPU_SET_5_H
#ifndef CPU_SET_5_L
    0xED,
#endif // CPU_SET_5_L
#ifndef CPU_SET_5_AT_HL
    0xEE,
#endif // CPU_SET_5_AT_HL
#ifndef CPU_SET_5_A
    0xEF,
#endif // CPU_SET_5_A
#ifndef CPU_SET_6_B
    0xF0,
#endif // CPU_SET_6_B
#ifndef CPU_SET_6_C
    0xF1,
#endif // CPU_SET_6_C
#ifndef CPU_SET_6_D
    0xF2,
#endif // CPU_SET_6_D
#ifndef CPU_SET_6_E
    0xF3,
#endif // CPU_SET_6_E
#ifndef CPU_SET_6_H
    0xF4,
#endif // CPU_SET_6_H
#ifndef CPU_SET_6_L
    0xF5,
#endif // CPU_SET_6_L
#ifndef CPU_SET_6_AT_HL
    0xF6,
#endif // CPU_SET_6_AT_HL
#ifndef CPU_SET_6_A
    0xF7,
#endif // CPU_SET_6_A
#ifndef CPU_SET_7_B
    0xF8,
#endif // CPU_SET_7_B
#ifndef CPU_SET_7_C
    0xF9,
#endif // CPU_SET_7_C
#ifndef CPU_SET_7_D
    0xFA,
#endif // CPU_SET_7_D
#ifndef CPU_SET_7_E
    0xFB,
#endif // CPU_SET_7_E
#ifndef CPU_SET_7_H
    0xFC,
#endif // CPU_SET_7_H
#ifndef CPU_SET_7_L
    0xFD,
#endif // CPU_SET_7_L
#ifndef CPU_SET_7_AT_HL
    0xFE,
#endif // CPU_SET_7_AT_HL
#ifndef CPU_SET_7_A
    0xFF,
#endif // CPU_SET_7_A
};