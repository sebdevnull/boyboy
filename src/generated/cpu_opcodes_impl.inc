// ============================================================
// CPU stub implementations (unprefixed)
// AUTO-GENERATED FILE — DO NOT EDIT MANUALLY
// Generated from Opcodes.json on 2025-09-07 10:00:59 UTC
// ============================================================

#include "errors.h"

#ifndef CPU_NOP
void boyboy::cpu::Cpu::nop() {
    throw boyboy::errors::UnimplementedOpcode(0x00, "NOP");
}
#endif // CPU_NOP

#ifndef CPU_LD_BC_N16
void boyboy::cpu::Cpu::ld_bc_n16() {
    throw boyboy::errors::UnimplementedOpcode(0x01, "LD BC, n16");
}
#endif // CPU_LD_BC_N16

#ifndef CPU_LD_AT_BC_A
void boyboy::cpu::Cpu::ld_at_bc_a() {
    throw boyboy::errors::UnimplementedOpcode(0x02, "LD [BC], A");
}
#endif // CPU_LD_AT_BC_A

#ifndef CPU_INC_BC
void boyboy::cpu::Cpu::inc_bc() {
    throw boyboy::errors::UnimplementedOpcode(0x03, "INC BC");
}
#endif // CPU_INC_BC

#ifndef CPU_INC_B
void boyboy::cpu::Cpu::inc_b() {
    throw boyboy::errors::UnimplementedOpcode(0x04, "INC B");
}
#endif // CPU_INC_B

#ifndef CPU_DEC_B
void boyboy::cpu::Cpu::dec_b() {
    throw boyboy::errors::UnimplementedOpcode(0x05, "DEC B");
}
#endif // CPU_DEC_B

#ifndef CPU_LD_B_N8
void boyboy::cpu::Cpu::ld_b_n8() {
    throw boyboy::errors::UnimplementedOpcode(0x06, "LD B, n8");
}
#endif // CPU_LD_B_N8

#ifndef CPU_RLCA
void boyboy::cpu::Cpu::rlca() {
    throw boyboy::errors::UnimplementedOpcode(0x07, "RLCA");
}
#endif // CPU_RLCA

#ifndef CPU_LD_AT_A16_SP
void boyboy::cpu::Cpu::ld_at_a16_sp() {
    throw boyboy::errors::UnimplementedOpcode(0x08, "LD [a16], SP");
}
#endif // CPU_LD_AT_A16_SP

#ifndef CPU_ADD_HL_BC
void boyboy::cpu::Cpu::add_hl_bc() {
    throw boyboy::errors::UnimplementedOpcode(0x09, "ADD HL, BC");
}
#endif // CPU_ADD_HL_BC

#ifndef CPU_LD_A_AT_BC
void boyboy::cpu::Cpu::ld_a_at_bc() {
    throw boyboy::errors::UnimplementedOpcode(0x0A, "LD A, [BC]");
}
#endif // CPU_LD_A_AT_BC

#ifndef CPU_DEC_BC
void boyboy::cpu::Cpu::dec_bc() {
    throw boyboy::errors::UnimplementedOpcode(0x0B, "DEC BC");
}
#endif // CPU_DEC_BC

#ifndef CPU_INC_C
void boyboy::cpu::Cpu::inc_c() {
    throw boyboy::errors::UnimplementedOpcode(0x0C, "INC C");
}
#endif // CPU_INC_C

#ifndef CPU_DEC_C
void boyboy::cpu::Cpu::dec_c() {
    throw boyboy::errors::UnimplementedOpcode(0x0D, "DEC C");
}
#endif // CPU_DEC_C

#ifndef CPU_LD_C_N8
void boyboy::cpu::Cpu::ld_c_n8() {
    throw boyboy::errors::UnimplementedOpcode(0x0E, "LD C, n8");
}
#endif // CPU_LD_C_N8

#ifndef CPU_RRCA
void boyboy::cpu::Cpu::rrca() {
    throw boyboy::errors::UnimplementedOpcode(0x0F, "RRCA");
}
#endif // CPU_RRCA

#ifndef CPU_STOP_N8
void boyboy::cpu::Cpu::stop_n8() {
    throw boyboy::errors::UnimplementedOpcode(0x10, "STOP n8");
}
#endif // CPU_STOP_N8

#ifndef CPU_LD_DE_N16
void boyboy::cpu::Cpu::ld_de_n16() {
    throw boyboy::errors::UnimplementedOpcode(0x11, "LD DE, n16");
}
#endif // CPU_LD_DE_N16

#ifndef CPU_LD_AT_DE_A
void boyboy::cpu::Cpu::ld_at_de_a() {
    throw boyboy::errors::UnimplementedOpcode(0x12, "LD [DE], A");
}
#endif // CPU_LD_AT_DE_A

#ifndef CPU_INC_DE
void boyboy::cpu::Cpu::inc_de() {
    throw boyboy::errors::UnimplementedOpcode(0x13, "INC DE");
}
#endif // CPU_INC_DE

#ifndef CPU_INC_D
void boyboy::cpu::Cpu::inc_d() {
    throw boyboy::errors::UnimplementedOpcode(0x14, "INC D");
}
#endif // CPU_INC_D

#ifndef CPU_DEC_D
void boyboy::cpu::Cpu::dec_d() {
    throw boyboy::errors::UnimplementedOpcode(0x15, "DEC D");
}
#endif // CPU_DEC_D

#ifndef CPU_LD_D_N8
void boyboy::cpu::Cpu::ld_d_n8() {
    throw boyboy::errors::UnimplementedOpcode(0x16, "LD D, n8");
}
#endif // CPU_LD_D_N8

#ifndef CPU_RLA
void boyboy::cpu::Cpu::rla() {
    throw boyboy::errors::UnimplementedOpcode(0x17, "RLA");
}
#endif // CPU_RLA

#ifndef CPU_JR_E8
void boyboy::cpu::Cpu::jr_e8() {
    throw boyboy::errors::UnimplementedOpcode(0x18, "JR e8");
}
#endif // CPU_JR_E8

#ifndef CPU_ADD_HL_DE
void boyboy::cpu::Cpu::add_hl_de() {
    throw boyboy::errors::UnimplementedOpcode(0x19, "ADD HL, DE");
}
#endif // CPU_ADD_HL_DE

#ifndef CPU_LD_A_AT_DE
void boyboy::cpu::Cpu::ld_a_at_de() {
    throw boyboy::errors::UnimplementedOpcode(0x1A, "LD A, [DE]");
}
#endif // CPU_LD_A_AT_DE

#ifndef CPU_DEC_DE
void boyboy::cpu::Cpu::dec_de() {
    throw boyboy::errors::UnimplementedOpcode(0x1B, "DEC DE");
}
#endif // CPU_DEC_DE

#ifndef CPU_INC_E
void boyboy::cpu::Cpu::inc_e() {
    throw boyboy::errors::UnimplementedOpcode(0x1C, "INC E");
}
#endif // CPU_INC_E

#ifndef CPU_DEC_E
void boyboy::cpu::Cpu::dec_e() {
    throw boyboy::errors::UnimplementedOpcode(0x1D, "DEC E");
}
#endif // CPU_DEC_E

#ifndef CPU_LD_E_N8
void boyboy::cpu::Cpu::ld_e_n8() {
    throw boyboy::errors::UnimplementedOpcode(0x1E, "LD E, n8");
}
#endif // CPU_LD_E_N8

#ifndef CPU_RRA
void boyboy::cpu::Cpu::rra() {
    throw boyboy::errors::UnimplementedOpcode(0x1F, "RRA");
}
#endif // CPU_RRA

#ifndef CPU_JR_NZ_E8
void boyboy::cpu::Cpu::jr_nz_e8() {
    throw boyboy::errors::UnimplementedOpcode(0x20, "JR NZ, e8");
}
#endif // CPU_JR_NZ_E8

#ifndef CPU_LD_HL_N16
void boyboy::cpu::Cpu::ld_hl_n16() {
    throw boyboy::errors::UnimplementedOpcode(0x21, "LD HL, n16");
}
#endif // CPU_LD_HL_N16

#ifndef CPU_LD_AT_HL_INC_A
void boyboy::cpu::Cpu::ld_at_hl_inc_a() {
    throw boyboy::errors::UnimplementedOpcode(0x22, "LD [HL+], A");
}
#endif // CPU_LD_AT_HL_INC_A

#ifndef CPU_INC_HL
void boyboy::cpu::Cpu::inc_hl() {
    throw boyboy::errors::UnimplementedOpcode(0x23, "INC HL");
}
#endif // CPU_INC_HL

#ifndef CPU_INC_H
void boyboy::cpu::Cpu::inc_h() {
    throw boyboy::errors::UnimplementedOpcode(0x24, "INC H");
}
#endif // CPU_INC_H

#ifndef CPU_DEC_H
void boyboy::cpu::Cpu::dec_h() {
    throw boyboy::errors::UnimplementedOpcode(0x25, "DEC H");
}
#endif // CPU_DEC_H

#ifndef CPU_LD_H_N8
void boyboy::cpu::Cpu::ld_h_n8() {
    throw boyboy::errors::UnimplementedOpcode(0x26, "LD H, n8");
}
#endif // CPU_LD_H_N8

#ifndef CPU_DAA
void boyboy::cpu::Cpu::daa() {
    throw boyboy::errors::UnimplementedOpcode(0x27, "DAA");
}
#endif // CPU_DAA

#ifndef CPU_JR_Z_E8
void boyboy::cpu::Cpu::jr_z_e8() {
    throw boyboy::errors::UnimplementedOpcode(0x28, "JR Z, e8");
}
#endif // CPU_JR_Z_E8

#ifndef CPU_ADD_HL_HL
void boyboy::cpu::Cpu::add_hl_hl() {
    throw boyboy::errors::UnimplementedOpcode(0x29, "ADD HL, HL");
}
#endif // CPU_ADD_HL_HL

#ifndef CPU_LD_A_AT_HL_INC
void boyboy::cpu::Cpu::ld_a_at_hl_inc() {
    throw boyboy::errors::UnimplementedOpcode(0x2A, "LD A, [HL+]");
}
#endif // CPU_LD_A_AT_HL_INC

#ifndef CPU_DEC_HL
void boyboy::cpu::Cpu::dec_hl() {
    throw boyboy::errors::UnimplementedOpcode(0x2B, "DEC HL");
}
#endif // CPU_DEC_HL

#ifndef CPU_INC_L
void boyboy::cpu::Cpu::inc_l() {
    throw boyboy::errors::UnimplementedOpcode(0x2C, "INC L");
}
#endif // CPU_INC_L

#ifndef CPU_DEC_L
void boyboy::cpu::Cpu::dec_l() {
    throw boyboy::errors::UnimplementedOpcode(0x2D, "DEC L");
}
#endif // CPU_DEC_L

#ifndef CPU_LD_L_N8
void boyboy::cpu::Cpu::ld_l_n8() {
    throw boyboy::errors::UnimplementedOpcode(0x2E, "LD L, n8");
}
#endif // CPU_LD_L_N8

#ifndef CPU_CPL
void boyboy::cpu::Cpu::cpl() {
    throw boyboy::errors::UnimplementedOpcode(0x2F, "CPL");
}
#endif // CPU_CPL

#ifndef CPU_JR_NC_E8
void boyboy::cpu::Cpu::jr_nc_e8() {
    throw boyboy::errors::UnimplementedOpcode(0x30, "JR NC, e8");
}
#endif // CPU_JR_NC_E8

#ifndef CPU_LD_SP_N16
void boyboy::cpu::Cpu::ld_sp_n16() {
    throw boyboy::errors::UnimplementedOpcode(0x31, "LD SP, n16");
}
#endif // CPU_LD_SP_N16

#ifndef CPU_LD_AT_HL_DEC_A
void boyboy::cpu::Cpu::ld_at_hl_dec_a() {
    throw boyboy::errors::UnimplementedOpcode(0x32, "LD [HL-], A");
}
#endif // CPU_LD_AT_HL_DEC_A

#ifndef CPU_INC_SP
void boyboy::cpu::Cpu::inc_sp() {
    throw boyboy::errors::UnimplementedOpcode(0x33, "INC SP");
}
#endif // CPU_INC_SP

#ifndef CPU_INC_AT_HL
void boyboy::cpu::Cpu::inc_at_hl() {
    throw boyboy::errors::UnimplementedOpcode(0x34, "INC [HL]");
}
#endif // CPU_INC_AT_HL

#ifndef CPU_DEC_AT_HL
void boyboy::cpu::Cpu::dec_at_hl() {
    throw boyboy::errors::UnimplementedOpcode(0x35, "DEC [HL]");
}
#endif // CPU_DEC_AT_HL

#ifndef CPU_LD_AT_HL_N8
void boyboy::cpu::Cpu::ld_at_hl_n8() {
    throw boyboy::errors::UnimplementedOpcode(0x36, "LD [HL], n8");
}
#endif // CPU_LD_AT_HL_N8

#ifndef CPU_SCF
void boyboy::cpu::Cpu::scf() {
    throw boyboy::errors::UnimplementedOpcode(0x37, "SCF");
}
#endif // CPU_SCF

#ifndef CPU_JR_C_E8
void boyboy::cpu::Cpu::jr_c_e8() {
    throw boyboy::errors::UnimplementedOpcode(0x38, "JR C, e8");
}
#endif // CPU_JR_C_E8

#ifndef CPU_ADD_HL_SP
void boyboy::cpu::Cpu::add_hl_sp() {
    throw boyboy::errors::UnimplementedOpcode(0x39, "ADD HL, SP");
}
#endif // CPU_ADD_HL_SP

#ifndef CPU_LD_A_AT_HL_DEC
void boyboy::cpu::Cpu::ld_a_at_hl_dec() {
    throw boyboy::errors::UnimplementedOpcode(0x3A, "LD A, [HL-]");
}
#endif // CPU_LD_A_AT_HL_DEC

#ifndef CPU_DEC_SP
void boyboy::cpu::Cpu::dec_sp() {
    throw boyboy::errors::UnimplementedOpcode(0x3B, "DEC SP");
}
#endif // CPU_DEC_SP

#ifndef CPU_INC_A
void boyboy::cpu::Cpu::inc_a() {
    throw boyboy::errors::UnimplementedOpcode(0x3C, "INC A");
}
#endif // CPU_INC_A

#ifndef CPU_DEC_A
void boyboy::cpu::Cpu::dec_a() {
    throw boyboy::errors::UnimplementedOpcode(0x3D, "DEC A");
}
#endif // CPU_DEC_A

#ifndef CPU_LD_A_N8
void boyboy::cpu::Cpu::ld_a_n8() {
    throw boyboy::errors::UnimplementedOpcode(0x3E, "LD A, n8");
}
#endif // CPU_LD_A_N8

#ifndef CPU_CCF
void boyboy::cpu::Cpu::ccf() {
    throw boyboy::errors::UnimplementedOpcode(0x3F, "CCF");
}
#endif // CPU_CCF

#ifndef CPU_LD_B_B
void boyboy::cpu::Cpu::ld_b_b() {
    throw boyboy::errors::UnimplementedOpcode(0x40, "LD B, B");
}
#endif // CPU_LD_B_B

#ifndef CPU_LD_B_C
void boyboy::cpu::Cpu::ld_b_c() {
    throw boyboy::errors::UnimplementedOpcode(0x41, "LD B, C");
}
#endif // CPU_LD_B_C

#ifndef CPU_LD_B_D
void boyboy::cpu::Cpu::ld_b_d() {
    throw boyboy::errors::UnimplementedOpcode(0x42, "LD B, D");
}
#endif // CPU_LD_B_D

#ifndef CPU_LD_B_E
void boyboy::cpu::Cpu::ld_b_e() {
    throw boyboy::errors::UnimplementedOpcode(0x43, "LD B, E");
}
#endif // CPU_LD_B_E

#ifndef CPU_LD_B_H
void boyboy::cpu::Cpu::ld_b_h() {
    throw boyboy::errors::UnimplementedOpcode(0x44, "LD B, H");
}
#endif // CPU_LD_B_H

#ifndef CPU_LD_B_L
void boyboy::cpu::Cpu::ld_b_l() {
    throw boyboy::errors::UnimplementedOpcode(0x45, "LD B, L");
}
#endif // CPU_LD_B_L

#ifndef CPU_LD_B_AT_HL
void boyboy::cpu::Cpu::ld_b_at_hl() {
    throw boyboy::errors::UnimplementedOpcode(0x46, "LD B, [HL]");
}
#endif // CPU_LD_B_AT_HL

#ifndef CPU_LD_B_A
void boyboy::cpu::Cpu::ld_b_a() {
    throw boyboy::errors::UnimplementedOpcode(0x47, "LD B, A");
}
#endif // CPU_LD_B_A

#ifndef CPU_LD_C_B
void boyboy::cpu::Cpu::ld_c_b() {
    throw boyboy::errors::UnimplementedOpcode(0x48, "LD C, B");
}
#endif // CPU_LD_C_B

#ifndef CPU_LD_C_C
void boyboy::cpu::Cpu::ld_c_c() {
    throw boyboy::errors::UnimplementedOpcode(0x49, "LD C, C");
}
#endif // CPU_LD_C_C

#ifndef CPU_LD_C_D
void boyboy::cpu::Cpu::ld_c_d() {
    throw boyboy::errors::UnimplementedOpcode(0x4A, "LD C, D");
}
#endif // CPU_LD_C_D

#ifndef CPU_LD_C_E
void boyboy::cpu::Cpu::ld_c_e() {
    throw boyboy::errors::UnimplementedOpcode(0x4B, "LD C, E");
}
#endif // CPU_LD_C_E

#ifndef CPU_LD_C_H
void boyboy::cpu::Cpu::ld_c_h() {
    throw boyboy::errors::UnimplementedOpcode(0x4C, "LD C, H");
}
#endif // CPU_LD_C_H

#ifndef CPU_LD_C_L
void boyboy::cpu::Cpu::ld_c_l() {
    throw boyboy::errors::UnimplementedOpcode(0x4D, "LD C, L");
}
#endif // CPU_LD_C_L

#ifndef CPU_LD_C_AT_HL
void boyboy::cpu::Cpu::ld_c_at_hl() {
    throw boyboy::errors::UnimplementedOpcode(0x4E, "LD C, [HL]");
}
#endif // CPU_LD_C_AT_HL

#ifndef CPU_LD_C_A
void boyboy::cpu::Cpu::ld_c_a() {
    throw boyboy::errors::UnimplementedOpcode(0x4F, "LD C, A");
}
#endif // CPU_LD_C_A

#ifndef CPU_LD_D_B
void boyboy::cpu::Cpu::ld_d_b() {
    throw boyboy::errors::UnimplementedOpcode(0x50, "LD D, B");
}
#endif // CPU_LD_D_B

#ifndef CPU_LD_D_C
void boyboy::cpu::Cpu::ld_d_c() {
    throw boyboy::errors::UnimplementedOpcode(0x51, "LD D, C");
}
#endif // CPU_LD_D_C

#ifndef CPU_LD_D_D
void boyboy::cpu::Cpu::ld_d_d() {
    throw boyboy::errors::UnimplementedOpcode(0x52, "LD D, D");
}
#endif // CPU_LD_D_D

#ifndef CPU_LD_D_E
void boyboy::cpu::Cpu::ld_d_e() {
    throw boyboy::errors::UnimplementedOpcode(0x53, "LD D, E");
}
#endif // CPU_LD_D_E

#ifndef CPU_LD_D_H
void boyboy::cpu::Cpu::ld_d_h() {
    throw boyboy::errors::UnimplementedOpcode(0x54, "LD D, H");
}
#endif // CPU_LD_D_H

#ifndef CPU_LD_D_L
void boyboy::cpu::Cpu::ld_d_l() {
    throw boyboy::errors::UnimplementedOpcode(0x55, "LD D, L");
}
#endif // CPU_LD_D_L

#ifndef CPU_LD_D_AT_HL
void boyboy::cpu::Cpu::ld_d_at_hl() {
    throw boyboy::errors::UnimplementedOpcode(0x56, "LD D, [HL]");
}
#endif // CPU_LD_D_AT_HL

#ifndef CPU_LD_D_A
void boyboy::cpu::Cpu::ld_d_a() {
    throw boyboy::errors::UnimplementedOpcode(0x57, "LD D, A");
}
#endif // CPU_LD_D_A

#ifndef CPU_LD_E_B
void boyboy::cpu::Cpu::ld_e_b() {
    throw boyboy::errors::UnimplementedOpcode(0x58, "LD E, B");
}
#endif // CPU_LD_E_B

#ifndef CPU_LD_E_C
void boyboy::cpu::Cpu::ld_e_c() {
    throw boyboy::errors::UnimplementedOpcode(0x59, "LD E, C");
}
#endif // CPU_LD_E_C

#ifndef CPU_LD_E_D
void boyboy::cpu::Cpu::ld_e_d() {
    throw boyboy::errors::UnimplementedOpcode(0x5A, "LD E, D");
}
#endif // CPU_LD_E_D

#ifndef CPU_LD_E_E
void boyboy::cpu::Cpu::ld_e_e() {
    throw boyboy::errors::UnimplementedOpcode(0x5B, "LD E, E");
}
#endif // CPU_LD_E_E

#ifndef CPU_LD_E_H
void boyboy::cpu::Cpu::ld_e_h() {
    throw boyboy::errors::UnimplementedOpcode(0x5C, "LD E, H");
}
#endif // CPU_LD_E_H

#ifndef CPU_LD_E_L
void boyboy::cpu::Cpu::ld_e_l() {
    throw boyboy::errors::UnimplementedOpcode(0x5D, "LD E, L");
}
#endif // CPU_LD_E_L

#ifndef CPU_LD_E_AT_HL
void boyboy::cpu::Cpu::ld_e_at_hl() {
    throw boyboy::errors::UnimplementedOpcode(0x5E, "LD E, [HL]");
}
#endif // CPU_LD_E_AT_HL

#ifndef CPU_LD_E_A
void boyboy::cpu::Cpu::ld_e_a() {
    throw boyboy::errors::UnimplementedOpcode(0x5F, "LD E, A");
}
#endif // CPU_LD_E_A

#ifndef CPU_LD_H_B
void boyboy::cpu::Cpu::ld_h_b() {
    throw boyboy::errors::UnimplementedOpcode(0x60, "LD H, B");
}
#endif // CPU_LD_H_B

#ifndef CPU_LD_H_C
void boyboy::cpu::Cpu::ld_h_c() {
    throw boyboy::errors::UnimplementedOpcode(0x61, "LD H, C");
}
#endif // CPU_LD_H_C

#ifndef CPU_LD_H_D
void boyboy::cpu::Cpu::ld_h_d() {
    throw boyboy::errors::UnimplementedOpcode(0x62, "LD H, D");
}
#endif // CPU_LD_H_D

#ifndef CPU_LD_H_E
void boyboy::cpu::Cpu::ld_h_e() {
    throw boyboy::errors::UnimplementedOpcode(0x63, "LD H, E");
}
#endif // CPU_LD_H_E

#ifndef CPU_LD_H_H
void boyboy::cpu::Cpu::ld_h_h() {
    throw boyboy::errors::UnimplementedOpcode(0x64, "LD H, H");
}
#endif // CPU_LD_H_H

#ifndef CPU_LD_H_L
void boyboy::cpu::Cpu::ld_h_l() {
    throw boyboy::errors::UnimplementedOpcode(0x65, "LD H, L");
}
#endif // CPU_LD_H_L

#ifndef CPU_LD_H_AT_HL
void boyboy::cpu::Cpu::ld_h_at_hl() {
    throw boyboy::errors::UnimplementedOpcode(0x66, "LD H, [HL]");
}
#endif // CPU_LD_H_AT_HL

#ifndef CPU_LD_H_A
void boyboy::cpu::Cpu::ld_h_a() {
    throw boyboy::errors::UnimplementedOpcode(0x67, "LD H, A");
}
#endif // CPU_LD_H_A

#ifndef CPU_LD_L_B
void boyboy::cpu::Cpu::ld_l_b() {
    throw boyboy::errors::UnimplementedOpcode(0x68, "LD L, B");
}
#endif // CPU_LD_L_B

#ifndef CPU_LD_L_C
void boyboy::cpu::Cpu::ld_l_c() {
    throw boyboy::errors::UnimplementedOpcode(0x69, "LD L, C");
}
#endif // CPU_LD_L_C

#ifndef CPU_LD_L_D
void boyboy::cpu::Cpu::ld_l_d() {
    throw boyboy::errors::UnimplementedOpcode(0x6A, "LD L, D");
}
#endif // CPU_LD_L_D

#ifndef CPU_LD_L_E
void boyboy::cpu::Cpu::ld_l_e() {
    throw boyboy::errors::UnimplementedOpcode(0x6B, "LD L, E");
}
#endif // CPU_LD_L_E

#ifndef CPU_LD_L_H
void boyboy::cpu::Cpu::ld_l_h() {
    throw boyboy::errors::UnimplementedOpcode(0x6C, "LD L, H");
}
#endif // CPU_LD_L_H

#ifndef CPU_LD_L_L
void boyboy::cpu::Cpu::ld_l_l() {
    throw boyboy::errors::UnimplementedOpcode(0x6D, "LD L, L");
}
#endif // CPU_LD_L_L

#ifndef CPU_LD_L_AT_HL
void boyboy::cpu::Cpu::ld_l_at_hl() {
    throw boyboy::errors::UnimplementedOpcode(0x6E, "LD L, [HL]");
}
#endif // CPU_LD_L_AT_HL

#ifndef CPU_LD_L_A
void boyboy::cpu::Cpu::ld_l_a() {
    throw boyboy::errors::UnimplementedOpcode(0x6F, "LD L, A");
}
#endif // CPU_LD_L_A

#ifndef CPU_LD_AT_HL_B
void boyboy::cpu::Cpu::ld_at_hl_b() {
    throw boyboy::errors::UnimplementedOpcode(0x70, "LD [HL], B");
}
#endif // CPU_LD_AT_HL_B

#ifndef CPU_LD_AT_HL_C
void boyboy::cpu::Cpu::ld_at_hl_c() {
    throw boyboy::errors::UnimplementedOpcode(0x71, "LD [HL], C");
}
#endif // CPU_LD_AT_HL_C

#ifndef CPU_LD_AT_HL_D
void boyboy::cpu::Cpu::ld_at_hl_d() {
    throw boyboy::errors::UnimplementedOpcode(0x72, "LD [HL], D");
}
#endif // CPU_LD_AT_HL_D

#ifndef CPU_LD_AT_HL_E
void boyboy::cpu::Cpu::ld_at_hl_e() {
    throw boyboy::errors::UnimplementedOpcode(0x73, "LD [HL], E");
}
#endif // CPU_LD_AT_HL_E

#ifndef CPU_LD_AT_HL_H
void boyboy::cpu::Cpu::ld_at_hl_h() {
    throw boyboy::errors::UnimplementedOpcode(0x74, "LD [HL], H");
}
#endif // CPU_LD_AT_HL_H

#ifndef CPU_LD_AT_HL_L
void boyboy::cpu::Cpu::ld_at_hl_l() {
    throw boyboy::errors::UnimplementedOpcode(0x75, "LD [HL], L");
}
#endif // CPU_LD_AT_HL_L

#ifndef CPU_HALT
void boyboy::cpu::Cpu::halt() {
    throw boyboy::errors::UnimplementedOpcode(0x76, "HALT");
}
#endif // CPU_HALT

#ifndef CPU_LD_AT_HL_A
void boyboy::cpu::Cpu::ld_at_hl_a() {
    throw boyboy::errors::UnimplementedOpcode(0x77, "LD [HL], A");
}
#endif // CPU_LD_AT_HL_A

#ifndef CPU_LD_A_B
void boyboy::cpu::Cpu::ld_a_b() {
    throw boyboy::errors::UnimplementedOpcode(0x78, "LD A, B");
}
#endif // CPU_LD_A_B

#ifndef CPU_LD_A_C
void boyboy::cpu::Cpu::ld_a_c() {
    throw boyboy::errors::UnimplementedOpcode(0x79, "LD A, C");
}
#endif // CPU_LD_A_C

#ifndef CPU_LD_A_D
void boyboy::cpu::Cpu::ld_a_d() {
    throw boyboy::errors::UnimplementedOpcode(0x7A, "LD A, D");
}
#endif // CPU_LD_A_D

#ifndef CPU_LD_A_E
void boyboy::cpu::Cpu::ld_a_e() {
    throw boyboy::errors::UnimplementedOpcode(0x7B, "LD A, E");
}
#endif // CPU_LD_A_E

#ifndef CPU_LD_A_H
void boyboy::cpu::Cpu::ld_a_h() {
    throw boyboy::errors::UnimplementedOpcode(0x7C, "LD A, H");
}
#endif // CPU_LD_A_H

#ifndef CPU_LD_A_L
void boyboy::cpu::Cpu::ld_a_l() {
    throw boyboy::errors::UnimplementedOpcode(0x7D, "LD A, L");
}
#endif // CPU_LD_A_L

#ifndef CPU_LD_A_AT_HL
void boyboy::cpu::Cpu::ld_a_at_hl() {
    throw boyboy::errors::UnimplementedOpcode(0x7E, "LD A, [HL]");
}
#endif // CPU_LD_A_AT_HL

#ifndef CPU_LD_A_A
void boyboy::cpu::Cpu::ld_a_a() {
    throw boyboy::errors::UnimplementedOpcode(0x7F, "LD A, A");
}
#endif // CPU_LD_A_A

#ifndef CPU_ADD_A_B
void boyboy::cpu::Cpu::add_a_b() {
    throw boyboy::errors::UnimplementedOpcode(0x80, "ADD A, B");
}
#endif // CPU_ADD_A_B

#ifndef CPU_ADD_A_C
void boyboy::cpu::Cpu::add_a_c() {
    throw boyboy::errors::UnimplementedOpcode(0x81, "ADD A, C");
}
#endif // CPU_ADD_A_C

#ifndef CPU_ADD_A_D
void boyboy::cpu::Cpu::add_a_d() {
    throw boyboy::errors::UnimplementedOpcode(0x82, "ADD A, D");
}
#endif // CPU_ADD_A_D

#ifndef CPU_ADD_A_E
void boyboy::cpu::Cpu::add_a_e() {
    throw boyboy::errors::UnimplementedOpcode(0x83, "ADD A, E");
}
#endif // CPU_ADD_A_E

#ifndef CPU_ADD_A_H
void boyboy::cpu::Cpu::add_a_h() {
    throw boyboy::errors::UnimplementedOpcode(0x84, "ADD A, H");
}
#endif // CPU_ADD_A_H

#ifndef CPU_ADD_A_L
void boyboy::cpu::Cpu::add_a_l() {
    throw boyboy::errors::UnimplementedOpcode(0x85, "ADD A, L");
}
#endif // CPU_ADD_A_L

#ifndef CPU_ADD_A_AT_HL
void boyboy::cpu::Cpu::add_a_at_hl() {
    throw boyboy::errors::UnimplementedOpcode(0x86, "ADD A, [HL]");
}
#endif // CPU_ADD_A_AT_HL

#ifndef CPU_ADD_A_A
void boyboy::cpu::Cpu::add_a_a() {
    throw boyboy::errors::UnimplementedOpcode(0x87, "ADD A, A");
}
#endif // CPU_ADD_A_A

#ifndef CPU_ADC_A_B
void boyboy::cpu::Cpu::adc_a_b() {
    throw boyboy::errors::UnimplementedOpcode(0x88, "ADC A, B");
}
#endif // CPU_ADC_A_B

#ifndef CPU_ADC_A_C
void boyboy::cpu::Cpu::adc_a_c() {
    throw boyboy::errors::UnimplementedOpcode(0x89, "ADC A, C");
}
#endif // CPU_ADC_A_C

#ifndef CPU_ADC_A_D
void boyboy::cpu::Cpu::adc_a_d() {
    throw boyboy::errors::UnimplementedOpcode(0x8A, "ADC A, D");
}
#endif // CPU_ADC_A_D

#ifndef CPU_ADC_A_E
void boyboy::cpu::Cpu::adc_a_e() {
    throw boyboy::errors::UnimplementedOpcode(0x8B, "ADC A, E");
}
#endif // CPU_ADC_A_E

#ifndef CPU_ADC_A_H
void boyboy::cpu::Cpu::adc_a_h() {
    throw boyboy::errors::UnimplementedOpcode(0x8C, "ADC A, H");
}
#endif // CPU_ADC_A_H

#ifndef CPU_ADC_A_L
void boyboy::cpu::Cpu::adc_a_l() {
    throw boyboy::errors::UnimplementedOpcode(0x8D, "ADC A, L");
}
#endif // CPU_ADC_A_L

#ifndef CPU_ADC_A_AT_HL
void boyboy::cpu::Cpu::adc_a_at_hl() {
    throw boyboy::errors::UnimplementedOpcode(0x8E, "ADC A, [HL]");
}
#endif // CPU_ADC_A_AT_HL

#ifndef CPU_ADC_A_A
void boyboy::cpu::Cpu::adc_a_a() {
    throw boyboy::errors::UnimplementedOpcode(0x8F, "ADC A, A");
}
#endif // CPU_ADC_A_A

#ifndef CPU_SUB_A_B
void boyboy::cpu::Cpu::sub_a_b() {
    throw boyboy::errors::UnimplementedOpcode(0x90, "SUB A, B");
}
#endif // CPU_SUB_A_B

#ifndef CPU_SUB_A_C
void boyboy::cpu::Cpu::sub_a_c() {
    throw boyboy::errors::UnimplementedOpcode(0x91, "SUB A, C");
}
#endif // CPU_SUB_A_C

#ifndef CPU_SUB_A_D
void boyboy::cpu::Cpu::sub_a_d() {
    throw boyboy::errors::UnimplementedOpcode(0x92, "SUB A, D");
}
#endif // CPU_SUB_A_D

#ifndef CPU_SUB_A_E
void boyboy::cpu::Cpu::sub_a_e() {
    throw boyboy::errors::UnimplementedOpcode(0x93, "SUB A, E");
}
#endif // CPU_SUB_A_E

#ifndef CPU_SUB_A_H
void boyboy::cpu::Cpu::sub_a_h() {
    throw boyboy::errors::UnimplementedOpcode(0x94, "SUB A, H");
}
#endif // CPU_SUB_A_H

#ifndef CPU_SUB_A_L
void boyboy::cpu::Cpu::sub_a_l() {
    throw boyboy::errors::UnimplementedOpcode(0x95, "SUB A, L");
}
#endif // CPU_SUB_A_L

#ifndef CPU_SUB_A_AT_HL
void boyboy::cpu::Cpu::sub_a_at_hl() {
    throw boyboy::errors::UnimplementedOpcode(0x96, "SUB A, [HL]");
}
#endif // CPU_SUB_A_AT_HL

#ifndef CPU_SUB_A_A
void boyboy::cpu::Cpu::sub_a_a() {
    throw boyboy::errors::UnimplementedOpcode(0x97, "SUB A, A");
}
#endif // CPU_SUB_A_A

#ifndef CPU_SBC_A_B
void boyboy::cpu::Cpu::sbc_a_b() {
    throw boyboy::errors::UnimplementedOpcode(0x98, "SBC A, B");
}
#endif // CPU_SBC_A_B

#ifndef CPU_SBC_A_C
void boyboy::cpu::Cpu::sbc_a_c() {
    throw boyboy::errors::UnimplementedOpcode(0x99, "SBC A, C");
}
#endif // CPU_SBC_A_C

#ifndef CPU_SBC_A_D
void boyboy::cpu::Cpu::sbc_a_d() {
    throw boyboy::errors::UnimplementedOpcode(0x9A, "SBC A, D");
}
#endif // CPU_SBC_A_D

#ifndef CPU_SBC_A_E
void boyboy::cpu::Cpu::sbc_a_e() {
    throw boyboy::errors::UnimplementedOpcode(0x9B, "SBC A, E");
}
#endif // CPU_SBC_A_E

#ifndef CPU_SBC_A_H
void boyboy::cpu::Cpu::sbc_a_h() {
    throw boyboy::errors::UnimplementedOpcode(0x9C, "SBC A, H");
}
#endif // CPU_SBC_A_H

#ifndef CPU_SBC_A_L
void boyboy::cpu::Cpu::sbc_a_l() {
    throw boyboy::errors::UnimplementedOpcode(0x9D, "SBC A, L");
}
#endif // CPU_SBC_A_L

#ifndef CPU_SBC_A_AT_HL
void boyboy::cpu::Cpu::sbc_a_at_hl() {
    throw boyboy::errors::UnimplementedOpcode(0x9E, "SBC A, [HL]");
}
#endif // CPU_SBC_A_AT_HL

#ifndef CPU_SBC_A_A
void boyboy::cpu::Cpu::sbc_a_a() {
    throw boyboy::errors::UnimplementedOpcode(0x9F, "SBC A, A");
}
#endif // CPU_SBC_A_A

#ifndef CPU_AND_A_B
void boyboy::cpu::Cpu::and_a_b() {
    throw boyboy::errors::UnimplementedOpcode(0xA0, "AND A, B");
}
#endif // CPU_AND_A_B

#ifndef CPU_AND_A_C
void boyboy::cpu::Cpu::and_a_c() {
    throw boyboy::errors::UnimplementedOpcode(0xA1, "AND A, C");
}
#endif // CPU_AND_A_C

#ifndef CPU_AND_A_D
void boyboy::cpu::Cpu::and_a_d() {
    throw boyboy::errors::UnimplementedOpcode(0xA2, "AND A, D");
}
#endif // CPU_AND_A_D

#ifndef CPU_AND_A_E
void boyboy::cpu::Cpu::and_a_e() {
    throw boyboy::errors::UnimplementedOpcode(0xA3, "AND A, E");
}
#endif // CPU_AND_A_E

#ifndef CPU_AND_A_H
void boyboy::cpu::Cpu::and_a_h() {
    throw boyboy::errors::UnimplementedOpcode(0xA4, "AND A, H");
}
#endif // CPU_AND_A_H

#ifndef CPU_AND_A_L
void boyboy::cpu::Cpu::and_a_l() {
    throw boyboy::errors::UnimplementedOpcode(0xA5, "AND A, L");
}
#endif // CPU_AND_A_L

#ifndef CPU_AND_A_AT_HL
void boyboy::cpu::Cpu::and_a_at_hl() {
    throw boyboy::errors::UnimplementedOpcode(0xA6, "AND A, [HL]");
}
#endif // CPU_AND_A_AT_HL

#ifndef CPU_AND_A_A
void boyboy::cpu::Cpu::and_a_a() {
    throw boyboy::errors::UnimplementedOpcode(0xA7, "AND A, A");
}
#endif // CPU_AND_A_A

#ifndef CPU_XOR_A_B
void boyboy::cpu::Cpu::xor_a_b() {
    throw boyboy::errors::UnimplementedOpcode(0xA8, "XOR A, B");
}
#endif // CPU_XOR_A_B

#ifndef CPU_XOR_A_C
void boyboy::cpu::Cpu::xor_a_c() {
    throw boyboy::errors::UnimplementedOpcode(0xA9, "XOR A, C");
}
#endif // CPU_XOR_A_C

#ifndef CPU_XOR_A_D
void boyboy::cpu::Cpu::xor_a_d() {
    throw boyboy::errors::UnimplementedOpcode(0xAA, "XOR A, D");
}
#endif // CPU_XOR_A_D

#ifndef CPU_XOR_A_E
void boyboy::cpu::Cpu::xor_a_e() {
    throw boyboy::errors::UnimplementedOpcode(0xAB, "XOR A, E");
}
#endif // CPU_XOR_A_E

#ifndef CPU_XOR_A_H
void boyboy::cpu::Cpu::xor_a_h() {
    throw boyboy::errors::UnimplementedOpcode(0xAC, "XOR A, H");
}
#endif // CPU_XOR_A_H

#ifndef CPU_XOR_A_L
void boyboy::cpu::Cpu::xor_a_l() {
    throw boyboy::errors::UnimplementedOpcode(0xAD, "XOR A, L");
}
#endif // CPU_XOR_A_L

#ifndef CPU_XOR_A_AT_HL
void boyboy::cpu::Cpu::xor_a_at_hl() {
    throw boyboy::errors::UnimplementedOpcode(0xAE, "XOR A, [HL]");
}
#endif // CPU_XOR_A_AT_HL

#ifndef CPU_XOR_A_A
void boyboy::cpu::Cpu::xor_a_a() {
    throw boyboy::errors::UnimplementedOpcode(0xAF, "XOR A, A");
}
#endif // CPU_XOR_A_A

#ifndef CPU_OR_A_B
void boyboy::cpu::Cpu::or_a_b() {
    throw boyboy::errors::UnimplementedOpcode(0xB0, "OR A, B");
}
#endif // CPU_OR_A_B

#ifndef CPU_OR_A_C
void boyboy::cpu::Cpu::or_a_c() {
    throw boyboy::errors::UnimplementedOpcode(0xB1, "OR A, C");
}
#endif // CPU_OR_A_C

#ifndef CPU_OR_A_D
void boyboy::cpu::Cpu::or_a_d() {
    throw boyboy::errors::UnimplementedOpcode(0xB2, "OR A, D");
}
#endif // CPU_OR_A_D

#ifndef CPU_OR_A_E
void boyboy::cpu::Cpu::or_a_e() {
    throw boyboy::errors::UnimplementedOpcode(0xB3, "OR A, E");
}
#endif // CPU_OR_A_E

#ifndef CPU_OR_A_H
void boyboy::cpu::Cpu::or_a_h() {
    throw boyboy::errors::UnimplementedOpcode(0xB4, "OR A, H");
}
#endif // CPU_OR_A_H

#ifndef CPU_OR_A_L
void boyboy::cpu::Cpu::or_a_l() {
    throw boyboy::errors::UnimplementedOpcode(0xB5, "OR A, L");
}
#endif // CPU_OR_A_L

#ifndef CPU_OR_A_AT_HL
void boyboy::cpu::Cpu::or_a_at_hl() {
    throw boyboy::errors::UnimplementedOpcode(0xB6, "OR A, [HL]");
}
#endif // CPU_OR_A_AT_HL

#ifndef CPU_OR_A_A
void boyboy::cpu::Cpu::or_a_a() {
    throw boyboy::errors::UnimplementedOpcode(0xB7, "OR A, A");
}
#endif // CPU_OR_A_A

#ifndef CPU_CP_A_B
void boyboy::cpu::Cpu::cp_a_b() {
    throw boyboy::errors::UnimplementedOpcode(0xB8, "CP A, B");
}
#endif // CPU_CP_A_B

#ifndef CPU_CP_A_C
void boyboy::cpu::Cpu::cp_a_c() {
    throw boyboy::errors::UnimplementedOpcode(0xB9, "CP A, C");
}
#endif // CPU_CP_A_C

#ifndef CPU_CP_A_D
void boyboy::cpu::Cpu::cp_a_d() {
    throw boyboy::errors::UnimplementedOpcode(0xBA, "CP A, D");
}
#endif // CPU_CP_A_D

#ifndef CPU_CP_A_E
void boyboy::cpu::Cpu::cp_a_e() {
    throw boyboy::errors::UnimplementedOpcode(0xBB, "CP A, E");
}
#endif // CPU_CP_A_E

#ifndef CPU_CP_A_H
void boyboy::cpu::Cpu::cp_a_h() {
    throw boyboy::errors::UnimplementedOpcode(0xBC, "CP A, H");
}
#endif // CPU_CP_A_H

#ifndef CPU_CP_A_L
void boyboy::cpu::Cpu::cp_a_l() {
    throw boyboy::errors::UnimplementedOpcode(0xBD, "CP A, L");
}
#endif // CPU_CP_A_L

#ifndef CPU_CP_A_AT_HL
void boyboy::cpu::Cpu::cp_a_at_hl() {
    throw boyboy::errors::UnimplementedOpcode(0xBE, "CP A, [HL]");
}
#endif // CPU_CP_A_AT_HL

#ifndef CPU_CP_A_A
void boyboy::cpu::Cpu::cp_a_a() {
    throw boyboy::errors::UnimplementedOpcode(0xBF, "CP A, A");
}
#endif // CPU_CP_A_A

#ifndef CPU_RET_NZ
void boyboy::cpu::Cpu::ret_nz() {
    throw boyboy::errors::UnimplementedOpcode(0xC0, "RET NZ");
}
#endif // CPU_RET_NZ

#ifndef CPU_POP_BC
void boyboy::cpu::Cpu::pop_bc() {
    throw boyboy::errors::UnimplementedOpcode(0xC1, "POP BC");
}
#endif // CPU_POP_BC

#ifndef CPU_JP_NZ_A16
void boyboy::cpu::Cpu::jp_nz_a16() {
    throw boyboy::errors::UnimplementedOpcode(0xC2, "JP NZ, a16");
}
#endif // CPU_JP_NZ_A16

#ifndef CPU_JP_A16
void boyboy::cpu::Cpu::jp_a16() {
    throw boyboy::errors::UnimplementedOpcode(0xC3, "JP a16");
}
#endif // CPU_JP_A16

#ifndef CPU_CALL_NZ_A16
void boyboy::cpu::Cpu::call_nz_a16() {
    throw boyboy::errors::UnimplementedOpcode(0xC4, "CALL NZ, a16");
}
#endif // CPU_CALL_NZ_A16

#ifndef CPU_PUSH_BC
void boyboy::cpu::Cpu::push_bc() {
    throw boyboy::errors::UnimplementedOpcode(0xC5, "PUSH BC");
}
#endif // CPU_PUSH_BC

#ifndef CPU_ADD_A_N8
void boyboy::cpu::Cpu::add_a_n8() {
    throw boyboy::errors::UnimplementedOpcode(0xC6, "ADD A, n8");
}
#endif // CPU_ADD_A_N8

#ifndef CPU_RST_00
void boyboy::cpu::Cpu::rst_00() {
    throw boyboy::errors::UnimplementedOpcode(0xC7, "RST $00");
}
#endif // CPU_RST_00

#ifndef CPU_RET_Z
void boyboy::cpu::Cpu::ret_z() {
    throw boyboy::errors::UnimplementedOpcode(0xC8, "RET Z");
}
#endif // CPU_RET_Z

#ifndef CPU_RET
void boyboy::cpu::Cpu::ret() {
    throw boyboy::errors::UnimplementedOpcode(0xC9, "RET");
}
#endif // CPU_RET

#ifndef CPU_JP_Z_A16
void boyboy::cpu::Cpu::jp_z_a16() {
    throw boyboy::errors::UnimplementedOpcode(0xCA, "JP Z, a16");
}
#endif // CPU_JP_Z_A16

#ifndef CPU_PREFIX
void boyboy::cpu::Cpu::prefix() {
    throw boyboy::errors::UnimplementedOpcode(0xCB, "PREFIX");
}
#endif // CPU_PREFIX

#ifndef CPU_CALL_Z_A16
void boyboy::cpu::Cpu::call_z_a16() {
    throw boyboy::errors::UnimplementedOpcode(0xCC, "CALL Z, a16");
}
#endif // CPU_CALL_Z_A16

#ifndef CPU_CALL_A16
void boyboy::cpu::Cpu::call_a16() {
    throw boyboy::errors::UnimplementedOpcode(0xCD, "CALL a16");
}
#endif // CPU_CALL_A16

#ifndef CPU_ADC_A_N8
void boyboy::cpu::Cpu::adc_a_n8() {
    throw boyboy::errors::UnimplementedOpcode(0xCE, "ADC A, n8");
}
#endif // CPU_ADC_A_N8

#ifndef CPU_RST_08
void boyboy::cpu::Cpu::rst_08() {
    throw boyboy::errors::UnimplementedOpcode(0xCF, "RST $08");
}
#endif // CPU_RST_08

#ifndef CPU_RET_NC
void boyboy::cpu::Cpu::ret_nc() {
    throw boyboy::errors::UnimplementedOpcode(0xD0, "RET NC");
}
#endif // CPU_RET_NC

#ifndef CPU_POP_DE
void boyboy::cpu::Cpu::pop_de() {
    throw boyboy::errors::UnimplementedOpcode(0xD1, "POP DE");
}
#endif // CPU_POP_DE

#ifndef CPU_JP_NC_A16
void boyboy::cpu::Cpu::jp_nc_a16() {
    throw boyboy::errors::UnimplementedOpcode(0xD2, "JP NC, a16");
}
#endif // CPU_JP_NC_A16

#ifndef CPU_ILLEGAL_D3
void boyboy::cpu::Cpu::illegal_d3() {
    throw boyboy::errors::UnimplementedOpcode(0xD3, "ILLEGAL_D3");
}
#endif // CPU_ILLEGAL_D3

#ifndef CPU_CALL_NC_A16
void boyboy::cpu::Cpu::call_nc_a16() {
    throw boyboy::errors::UnimplementedOpcode(0xD4, "CALL NC, a16");
}
#endif // CPU_CALL_NC_A16

#ifndef CPU_PUSH_DE
void boyboy::cpu::Cpu::push_de() {
    throw boyboy::errors::UnimplementedOpcode(0xD5, "PUSH DE");
}
#endif // CPU_PUSH_DE

#ifndef CPU_SUB_A_N8
void boyboy::cpu::Cpu::sub_a_n8() {
    throw boyboy::errors::UnimplementedOpcode(0xD6, "SUB A, n8");
}
#endif // CPU_SUB_A_N8

#ifndef CPU_RST_10
void boyboy::cpu::Cpu::rst_10() {
    throw boyboy::errors::UnimplementedOpcode(0xD7, "RST $10");
}
#endif // CPU_RST_10

#ifndef CPU_RET_C
void boyboy::cpu::Cpu::ret_c() {
    throw boyboy::errors::UnimplementedOpcode(0xD8, "RET C");
}
#endif // CPU_RET_C

#ifndef CPU_RETI
void boyboy::cpu::Cpu::reti() {
    throw boyboy::errors::UnimplementedOpcode(0xD9, "RETI");
}
#endif // CPU_RETI

#ifndef CPU_JP_C_A16
void boyboy::cpu::Cpu::jp_c_a16() {
    throw boyboy::errors::UnimplementedOpcode(0xDA, "JP C, a16");
}
#endif // CPU_JP_C_A16

#ifndef CPU_ILLEGAL_DB
void boyboy::cpu::Cpu::illegal_db() {
    throw boyboy::errors::UnimplementedOpcode(0xDB, "ILLEGAL_DB");
}
#endif // CPU_ILLEGAL_DB

#ifndef CPU_CALL_C_A16
void boyboy::cpu::Cpu::call_c_a16() {
    throw boyboy::errors::UnimplementedOpcode(0xDC, "CALL C, a16");
}
#endif // CPU_CALL_C_A16

#ifndef CPU_ILLEGAL_DD
void boyboy::cpu::Cpu::illegal_dd() {
    throw boyboy::errors::UnimplementedOpcode(0xDD, "ILLEGAL_DD");
}
#endif // CPU_ILLEGAL_DD

#ifndef CPU_SBC_A_N8
void boyboy::cpu::Cpu::sbc_a_n8() {
    throw boyboy::errors::UnimplementedOpcode(0xDE, "SBC A, n8");
}
#endif // CPU_SBC_A_N8

#ifndef CPU_RST_18
void boyboy::cpu::Cpu::rst_18() {
    throw boyboy::errors::UnimplementedOpcode(0xDF, "RST $18");
}
#endif // CPU_RST_18

#ifndef CPU_LDH_AT_A8_A
void boyboy::cpu::Cpu::ldh_at_a8_a() {
    throw boyboy::errors::UnimplementedOpcode(0xE0, "LDH [a8], A");
}
#endif // CPU_LDH_AT_A8_A

#ifndef CPU_POP_HL
void boyboy::cpu::Cpu::pop_hl() {
    throw boyboy::errors::UnimplementedOpcode(0xE1, "POP HL");
}
#endif // CPU_POP_HL

#ifndef CPU_LDH_AT_C_A
void boyboy::cpu::Cpu::ldh_at_c_a() {
    throw boyboy::errors::UnimplementedOpcode(0xE2, "LDH [C], A");
}
#endif // CPU_LDH_AT_C_A

#ifndef CPU_ILLEGAL_E3
void boyboy::cpu::Cpu::illegal_e3() {
    throw boyboy::errors::UnimplementedOpcode(0xE3, "ILLEGAL_E3");
}
#endif // CPU_ILLEGAL_E3

#ifndef CPU_ILLEGAL_E4
void boyboy::cpu::Cpu::illegal_e4() {
    throw boyboy::errors::UnimplementedOpcode(0xE4, "ILLEGAL_E4");
}
#endif // CPU_ILLEGAL_E4

#ifndef CPU_PUSH_HL
void boyboy::cpu::Cpu::push_hl() {
    throw boyboy::errors::UnimplementedOpcode(0xE5, "PUSH HL");
}
#endif // CPU_PUSH_HL

#ifndef CPU_AND_A_N8
void boyboy::cpu::Cpu::and_a_n8() {
    throw boyboy::errors::UnimplementedOpcode(0xE6, "AND A, n8");
}
#endif // CPU_AND_A_N8

#ifndef CPU_RST_20
void boyboy::cpu::Cpu::rst_20() {
    throw boyboy::errors::UnimplementedOpcode(0xE7, "RST $20");
}
#endif // CPU_RST_20

#ifndef CPU_ADD_SP_E8
void boyboy::cpu::Cpu::add_sp_e8() {
    throw boyboy::errors::UnimplementedOpcode(0xE8, "ADD SP, e8");
}
#endif // CPU_ADD_SP_E8

#ifndef CPU_JP_HL
void boyboy::cpu::Cpu::jp_hl() {
    throw boyboy::errors::UnimplementedOpcode(0xE9, "JP HL");
}
#endif // CPU_JP_HL

#ifndef CPU_LD_AT_A16_A
void boyboy::cpu::Cpu::ld_at_a16_a() {
    throw boyboy::errors::UnimplementedOpcode(0xEA, "LD [a16], A");
}
#endif // CPU_LD_AT_A16_A

#ifndef CPU_ILLEGAL_EB
void boyboy::cpu::Cpu::illegal_eb() {
    throw boyboy::errors::UnimplementedOpcode(0xEB, "ILLEGAL_EB");
}
#endif // CPU_ILLEGAL_EB

#ifndef CPU_ILLEGAL_EC
void boyboy::cpu::Cpu::illegal_ec() {
    throw boyboy::errors::UnimplementedOpcode(0xEC, "ILLEGAL_EC");
}
#endif // CPU_ILLEGAL_EC

#ifndef CPU_ILLEGAL_ED
void boyboy::cpu::Cpu::illegal_ed() {
    throw boyboy::errors::UnimplementedOpcode(0xED, "ILLEGAL_ED");
}
#endif // CPU_ILLEGAL_ED

#ifndef CPU_XOR_A_N8
void boyboy::cpu::Cpu::xor_a_n8() {
    throw boyboy::errors::UnimplementedOpcode(0xEE, "XOR A, n8");
}
#endif // CPU_XOR_A_N8

#ifndef CPU_RST_28
void boyboy::cpu::Cpu::rst_28() {
    throw boyboy::errors::UnimplementedOpcode(0xEF, "RST $28");
}
#endif // CPU_RST_28

#ifndef CPU_LDH_A_AT_A8
void boyboy::cpu::Cpu::ldh_a_at_a8() {
    throw boyboy::errors::UnimplementedOpcode(0xF0, "LDH A, [a8]");
}
#endif // CPU_LDH_A_AT_A8

#ifndef CPU_POP_AF
void boyboy::cpu::Cpu::pop_af() {
    throw boyboy::errors::UnimplementedOpcode(0xF1, "POP AF");
}
#endif // CPU_POP_AF

#ifndef CPU_LDH_A_AT_C
void boyboy::cpu::Cpu::ldh_a_at_c() {
    throw boyboy::errors::UnimplementedOpcode(0xF2, "LDH A, [C]");
}
#endif // CPU_LDH_A_AT_C

#ifndef CPU_DI
void boyboy::cpu::Cpu::di() {
    throw boyboy::errors::UnimplementedOpcode(0xF3, "DI");
}
#endif // CPU_DI

#ifndef CPU_ILLEGAL_F4
void boyboy::cpu::Cpu::illegal_f4() {
    throw boyboy::errors::UnimplementedOpcode(0xF4, "ILLEGAL_F4");
}
#endif // CPU_ILLEGAL_F4

#ifndef CPU_PUSH_AF
void boyboy::cpu::Cpu::push_af() {
    throw boyboy::errors::UnimplementedOpcode(0xF5, "PUSH AF");
}
#endif // CPU_PUSH_AF

#ifndef CPU_OR_A_N8
void boyboy::cpu::Cpu::or_a_n8() {
    throw boyboy::errors::UnimplementedOpcode(0xF6, "OR A, n8");
}
#endif // CPU_OR_A_N8

#ifndef CPU_RST_30
void boyboy::cpu::Cpu::rst_30() {
    throw boyboy::errors::UnimplementedOpcode(0xF7, "RST $30");
}
#endif // CPU_RST_30

#ifndef CPU_LD_HL_SP_INC_E8
void boyboy::cpu::Cpu::ld_hl_sp_inc_e8() {
    throw boyboy::errors::UnimplementedOpcode(0xF8, "LD HL, SP+, e8");
}
#endif // CPU_LD_HL_SP_INC_E8

#ifndef CPU_LD_SP_HL
void boyboy::cpu::Cpu::ld_sp_hl() {
    throw boyboy::errors::UnimplementedOpcode(0xF9, "LD SP, HL");
}
#endif // CPU_LD_SP_HL

#ifndef CPU_LD_A_AT_A16
void boyboy::cpu::Cpu::ld_a_at_a16() {
    throw boyboy::errors::UnimplementedOpcode(0xFA, "LD A, [a16]");
}
#endif // CPU_LD_A_AT_A16

#ifndef CPU_EI
void boyboy::cpu::Cpu::ei() {
    throw boyboy::errors::UnimplementedOpcode(0xFB, "EI");
}
#endif // CPU_EI

#ifndef CPU_ILLEGAL_FC
void boyboy::cpu::Cpu::illegal_fc() {
    throw boyboy::errors::UnimplementedOpcode(0xFC, "ILLEGAL_FC");
}
#endif // CPU_ILLEGAL_FC

#ifndef CPU_ILLEGAL_FD
void boyboy::cpu::Cpu::illegal_fd() {
    throw boyboy::errors::UnimplementedOpcode(0xFD, "ILLEGAL_FD");
}
#endif // CPU_ILLEGAL_FD

#ifndef CPU_CP_A_N8
void boyboy::cpu::Cpu::cp_a_n8() {
    throw boyboy::errors::UnimplementedOpcode(0xFE, "CP A, n8");
}
#endif // CPU_CP_A_N8

#ifndef CPU_RST_38
void boyboy::cpu::Cpu::rst_38() {
    throw boyboy::errors::UnimplementedOpcode(0xFF, "RST $38");
}
#endif // CPU_RST_38

