/**
 * @file cpu.h
 * @brief CPU emulation for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstdint>
#include <memory>
#include <string_view>

#include "boyboy/common/utils.h"
#include "boyboy/cpu/cpu_constants.h"
#include "boyboy/cpu/instructions.h"
#include "boyboy/cpu/interrupt_handler.h"
#include "boyboy/cpu/opcodes.h"
#include "boyboy/cpu/registers.h"
#include "boyboy/mmu.h"

namespace boyboy::cpu {

class Cpu {
public:
    Cpu(std::shared_ptr<mmu::Mmu> mmu) : mmu_(std::move(mmu)), interrupt_handler_(*this, *mmu_)
    {
        reset();
    }
    ~Cpu() = default;

    // delete move and copy
    Cpu(const Cpu&) = delete;
    Cpu& operator=(const Cpu&) = delete;
    Cpu(Cpu&&) = delete;
    Cpu& operator=(Cpu&&) = delete;

    // Reset CPU state
    void reset()
    {
        // Registers
        registers_.af = 0;
        registers_.bc = 0;
        registers_.de = 0;
        registers_.hl = 0;
        registers_.sp = SPStartValue;
        registers_.pc = PCStartValue;

        // Reset flags and state
        ime_ = false;
        ime_scheduled_ = false;
        halted_ = false;
        cycles_ = 0;
    }

    // Register accessors
    [[nodiscard]] uint8_t get_register(Reg8Name reg) const;
    [[nodiscard]] uint16_t get_register(Reg16Name reg) const;
    void set_register(Reg8Name reg, uint8_t value);
    void set_register(Reg16Name reg, uint16_t value);
    [[nodiscard]] uint16_t get_sp() const { return registers_.sp; }
    [[nodiscard]] uint16_t get_pc() const { return registers_.pc; }
    void set_sp(uint16_t sp) { registers_.sp = sp; }
    void set_pc(uint16_t pc) { registers_.pc = pc; }
    void push_pc() { push_r16(Reg16Name::PC); } // Push current PC to stack

    // Flag accessors
    [[nodiscard]] bool get_flag(uint8_t flag) const { return registers_.af.get_flag(flag); }
    void set_flag(uint8_t flag, bool value) { registers_.af.set_flag(flag, value); }
    [[nodiscard]] uint8_t get_flags() const { return registers_.f(); }

    // State accessors
    [[nodiscard]] bool get_ime() const { return ime_; }
    void set_ime(bool ime) { ime_ = ime; }
    void schedule_ime() { ime_scheduled_ = true; }
    [[nodiscard]] bool is_halted() const { return halted_; }
    void set_halted(bool halted) { halted_ = halted; }
    [[nodiscard]] uint64_t get_cycles() const { return cycles_; }
    void set_cycles(uint64_t cycles) { cycles_ = cycles; }
    void add_cycles(uint8_t cycles) { cycles_ += cycles; }
    void reset_cycles() { cycles_ = 0; }

    // Execution functions
    void step();
    uint8_t fetch();
    [[nodiscard]] uint8_t peek() const; // fetch without PC increment
    void execute(uint8_t opcode, InstructionType instr_type = InstructionType::Unprefixed);

    // Execute aliases
    void execute(Opcode opcode)
    {
        execute(static_cast<uint8_t>(opcode), InstructionType::Unprefixed);
    }
    void execute(CBOpcode opcode)
    {
        execute(static_cast<uint8_t>(opcode), InstructionType::CBPrefixed);
    }

    // Memory access wrappers
    [[nodiscard]] uint8_t read_byte(uint16_t addr) const { return mmu_->read_byte(addr); }
    [[nodiscard]] uint16_t read_word(uint16_t addr) const { return mmu_->read_word(addr); }
    void write_byte(uint16_t addr, uint8_t value) { mmu_->write_byte(addr, value); }
    void write_word(uint16_t addr, uint16_t value) { mmu_->write_word(addr, value); }

    // Interrupt handling
    InterruptHandler& get_interrupt_handler() { return interrupt_handler_; }
    [[nodiscard]] const InterruptHandler& get_interrupt_handler() const
    {
        return interrupt_handler_;
    }
    void request_interrupt(uint8_t interrupt) { interrupt_handler_.request(interrupt); }
    void enable_interrupt(uint8_t interrupt) { interrupt_handler_.enable(interrupt); }

    // Helpers mainly for debugging and testing
    [[nodiscard]] std::string_view disassemble(uint16_t addr) const;

private:
    std::shared_ptr<mmu::Mmu> mmu_;
    Registers registers_;
    InterruptHandler interrupt_handler_;

    uint64_t cycles_{};
    bool ime_{false}; // Interrupt Master Enable flag
    bool ime_scheduled_{false};
    bool halted_{false};

    // Helper functions
    uint16_t fetch_n16()
    {
        uint8_t lsb = fetch();
        uint8_t msb = fetch();
        return utils::to_u16(msb, lsb);
    }
    void reset_flags() { registers_.f(0); }

    // ALU operations
    void add(uint8_t val, bool use_carry);
    void sub(uint8_t val, bool use_carry);
    void aand(uint8_t val);
    void xxor(uint8_t val);
    void oor(uint8_t val);
    void cp(uint8_t val);

    // ========== CPU Instructions definitions ==========
    // Generic unprefixed CPU instructions
    void ld_r8_n8(Reg8Name r8);
    void ld_r8_r8(Reg8Name dst, Reg8Name src);
    void ld_r8_at_r16(Reg8Name dst, Reg16Name src);
    void ld_at_r16_r8(Reg16Name dst, Reg8Name src);
    void ld_r16_n16(Reg16Name r16);

    void inc_r16(Reg16Name r16);
    void dec_r16(Reg16Name r16);
    void add_hl_r16(Reg16Name r16);

    void inc_r8(Reg8Name r8);
    void dec_r8(Reg8Name r8);

    void add_a_r8(Reg8Name r8);
    void adc_a_r8(Reg8Name r8);
    void sub_a_r8(Reg8Name r8);
    void sbc_a_r8(Reg8Name r8);
    void and_a_r8(Reg8Name r8);
    void xor_a_r8(Reg8Name r8);
    void or_a_r8(Reg8Name r8);
    void cp_a_r8(Reg8Name r8);

    void pop_r16(Reg16Name r16);
    void push_r16(Reg16Name r16);

    void jp(uint16_t addr);
    void jp_z(uint16_t addr);
    void jp_nz(uint16_t addr);
    void jp_c(uint16_t addr);
    void jp_nc(uint16_t addr);

    void rst(uint8_t vector);

    static void illegal_opcode(uint8_t opcode);

    // Generic CB-prefixed CPU instructions
    void rlc_r8(Reg8Name r8);
    void rrc_r8(Reg8Name r8);
    void rl_r8(Reg8Name r8);
    void rr_r8(Reg8Name r8);
    void sla_r8(Reg8Name r8);
    void sra_r8(Reg8Name r8);
    void srl_r8(Reg8Name r8);
    void swap_r8(Reg8Name r8);
    void bit_b_r8(uint8_t bit, Reg8Name r8);
    void bit_b_at_hl(uint8_t bit);
    void res_b_r8(uint8_t bit, Reg8Name r8);
    void res_b_at_hl(uint8_t bit);
    void set_b_r8(uint8_t bit, Reg8Name r8);
    void set_b_at_hl(uint8_t bit);

    // Autogenerated CPU instructions definitions from Opcodes.json
    // clang-format off
#include "boyboy/generated/cpu_opcodes.inc"
#include "boyboy/generated/cpu_cbopcodes.inc"
    // clang-format on

    friend class InstructionTable;
};

// ----- Stub disabling macros for implemented opcodes -----

// ----------- Unprefixed instructions macros ----------
#define CPU_NOP
#define CPU_INC_A
#define CPU_INC_B
#define CPU_INC_C
#define CPU_INC_D
#define CPU_INC_E
#define CPU_INC_H
#define CPU_INC_L
#define CPU_DEC_A
#define CPU_DEC_B
#define CPU_DEC_C
#define CPU_DEC_D
#define CPU_DEC_E
#define CPU_DEC_H
#define CPU_DEC_L
#define CPU_ADD_A_A
#define CPU_ADD_A_B
#define CPU_ADD_A_C
#define CPU_ADD_A_D
#define CPU_ADD_A_E
#define CPU_ADD_A_H
#define CPU_ADD_A_L
#define CPU_SUB_A_A
#define CPU_SUB_A_B
#define CPU_SUB_A_C
#define CPU_SUB_A_D
#define CPU_SUB_A_E
#define CPU_SUB_A_H
#define CPU_SUB_A_L
#define CPU_AND_A_A
#define CPU_AND_A_B
#define CPU_AND_A_C
#define CPU_AND_A_D
#define CPU_AND_A_E
#define CPU_AND_A_H
#define CPU_AND_A_L
#define CPU_OR_A_A
#define CPU_OR_A_B
#define CPU_OR_A_C
#define CPU_OR_A_D
#define CPU_OR_A_E
#define CPU_OR_A_H
#define CPU_OR_A_L
#define CPU_XOR_A_A
#define CPU_XOR_A_B
#define CPU_XOR_A_C
#define CPU_XOR_A_D
#define CPU_XOR_A_E
#define CPU_XOR_A_H
#define CPU_XOR_A_L
#define CPU_CP_A_A
#define CPU_CP_A_B
#define CPU_CP_A_C
#define CPU_CP_A_D
#define CPU_CP_A_E
#define CPU_CP_A_H
#define CPU_CP_A_L
#define CPU_ADC_A_A
#define CPU_ADC_A_B
#define CPU_ADC_A_C
#define CPU_ADC_A_D
#define CPU_ADC_A_E
#define CPU_ADC_A_H
#define CPU_ADC_A_L
#define CPU_SBC_A_A
#define CPU_SBC_A_B
#define CPU_SBC_A_C
#define CPU_SBC_A_D
#define CPU_SBC_A_E
#define CPU_SBC_A_H
#define CPU_SBC_A_L
#define CPU_INC_AT_HL
#define CPU_DEC_AT_HL
#define CPU_ADD_A_AT_HL
#define CPU_ADC_A_AT_HL
#define CPU_SUB_A_AT_HL
#define CPU_SBC_A_AT_HL
#define CPU_AND_A_AT_HL
#define CPU_OR_A_AT_HL
#define CPU_XOR_A_AT_HL
#define CPU_CP_A_AT_HL
#define CPU_ADD_A_N8
#define CPU_ADC_A_N8
#define CPU_SUB_A_N8
#define CPU_SBC_A_N8
#define CPU_AND_A_N8
#define CPU_OR_A_N8
#define CPU_XOR_A_N8
#define CPU_CP_A_N8

// LD r8, r8'
// A
#define CPU_LD_A_A
#define CPU_LD_A_B
#define CPU_LD_A_C
#define CPU_LD_A_D
#define CPU_LD_A_E
#define CPU_LD_A_H
#define CPU_LD_A_L
// B
#define CPU_LD_B_A
#define CPU_LD_B_B
#define CPU_LD_B_C
#define CPU_LD_B_D
#define CPU_LD_B_E
#define CPU_LD_B_H
#define CPU_LD_B_L
// C
#define CPU_LD_C_A
#define CPU_LD_C_B
#define CPU_LD_C_C
#define CPU_LD_C_D
#define CPU_LD_C_E
#define CPU_LD_C_H
#define CPU_LD_C_L
// D
#define CPU_LD_D_A
#define CPU_LD_D_B
#define CPU_LD_D_C
#define CPU_LD_D_D
#define CPU_LD_D_E
#define CPU_LD_D_H
#define CPU_LD_D_L
// E
#define CPU_LD_E_A
#define CPU_LD_E_B
#define CPU_LD_E_C
#define CPU_LD_E_D
#define CPU_LD_E_E
#define CPU_LD_E_H
#define CPU_LD_E_L
// H
#define CPU_LD_H_A
#define CPU_LD_H_B
#define CPU_LD_H_C
#define CPU_LD_H_D
#define CPU_LD_H_E
#define CPU_LD_H_H
#define CPU_LD_H_L
// L
#define CPU_LD_L_A
#define CPU_LD_L_B
#define CPU_LD_L_C
#define CPU_LD_L_D
#define CPU_LD_L_E
#define CPU_LD_L_H
#define CPU_LD_L_L

// LD r8, n8
#define CPU_LD_A_N8
#define CPU_LD_B_N8
#define CPU_LD_C_N8
#define CPU_LD_D_N8
#define CPU_LD_E_N8
#define CPU_LD_H_N8
#define CPU_LD_L_N8

// LD r8, [HL]
#define CPU_LD_A_AT_HL
#define CPU_LD_B_AT_HL
#define CPU_LD_C_AT_HL
#define CPU_LD_D_AT_HL
#define CPU_LD_E_AT_HL
#define CPU_LD_H_AT_HL
#define CPU_LD_L_AT_HL

// LD [HL], r8
#define CPU_LD_AT_HL_A
#define CPU_LD_AT_HL_B
#define CPU_LD_AT_HL_C
#define CPU_LD_AT_HL_D
#define CPU_LD_AT_HL_E
#define CPU_LD_AT_HL_H
#define CPU_LD_AT_HL_L

// LD [HL], n8
#define CPU_LD_AT_HL_N8

// LD A, [BC/DE] and LD [BC/DE], A
#define CPU_LD_A_AT_BC
#define CPU_LD_A_AT_DE
#define CPU_LD_AT_BC_A
#define CPU_LD_AT_DE_A

// LD A, [n16] and LD [n16], A
#define CPU_LD_A_AT_A16
#define CPU_LD_AT_A16_A

// LD A, [HL+/-]
#define CPU_LD_A_AT_HL_INC
#define CPU_LD_A_AT_HL_DEC

// LD [HL+/-], A
#define CPU_LD_AT_HL_INC_A
#define CPU_LD_AT_HL_DEC_A

// LDH
#define CPU_LDH_A_AT_C
#define CPU_LDH_A_AT_A8
#define CPU_LDH_AT_C_A
#define CPU_LDH_AT_A8_A

// INC r16
#define CPU_INC_BC
#define CPU_INC_DE
#define CPU_INC_HL
#define CPU_INC_SP

// DEC r16
#define CPU_DEC_BC
#define CPU_DEC_DE
#define CPU_DEC_HL
#define CPU_DEC_SP

// ADD HL, r16
#define CPU_ADD_HL_BC
#define CPU_ADD_HL_DE
#define CPU_ADD_HL_HL
#define CPU_ADD_HL_SP

// ADD SP, e8
#define CPU_ADD_SP_E8

// LD r16, n16
#define CPU_LD_BC_N16
#define CPU_LD_DE_N16
#define CPU_LD_HL_N16
#define CPU_LD_SP_N16

// LD [n16], SP
#define CPU_LD_AT_A16_SP

// LD HL, SP+e8
#define CPU_LD_HL_SP_INC_E8

// LD SP, HL
#define CPU_LD_SP_HL

// POP r16
#define CPU_POP_BC
#define CPU_POP_DE
#define CPU_POP_HL
#define CPU_POP_AF
// PUSH r16
#define CPU_PUSH_BC
#define CPU_PUSH_DE
#define CPU_PUSH_HL
#define CPU_PUSH_AF

// JP a16
#define CPU_JP_A16
// JP cc, a16
#define CPU_JP_Z_A16
#define CPU_JP_NZ_A16
#define CPU_JP_C_A16
#define CPU_JP_NC_A16
// JP HL
#define CPU_JP_HL
// JR e8
#define CPU_JR_E8
// JR cc, e8
#define CPU_JR_Z_E8
#define CPU_JR_NZ_E8
#define CPU_JR_C_E8
#define CPU_JR_NC_E8

// CALL a16
#define CPU_CALL_A16
// CALL cc, a16
#define CPU_CALL_Z_A16
#define CPU_CALL_NZ_A16
#define CPU_CALL_C_A16
#define CPU_CALL_NC_A16
// RET
#define CPU_RET
// RET cc
#define CPU_RET_Z
#define CPU_RET_NZ
#define CPU_RET_C
#define CPU_RET_NC
// RETI
#define CPU_RETI
// RST n
#define CPU_RST_00
#define CPU_RST_08
#define CPU_RST_10
#define CPU_RST_18
#define CPU_RST_20
#define CPU_RST_28
#define CPU_RST_30
#define CPU_RST_38

// DAA
#define CPU_DAA
// CPL
#define CPU_CPL
// CCF
#define CPU_CCF
// SCF
#define CPU_SCF

// RLA
#define CPU_RLA
// RRA
#define CPU_RRA
// RLCA
#define CPU_RLCA
// RRCA
#define CPU_RRCA

// EI
#define CPU_EI
// DI
#define CPU_DI
// HALT
#define CPU_HALT
// STOP
#define CPU_STOP_N8

// Illegal opcodes
#define CPU_PREFIX
#define CPU_ILLEGAL_D3
#define CPU_ILLEGAL_DB
#define CPU_ILLEGAL_DD
#define CPU_ILLEGAL_E3
#define CPU_ILLEGAL_E4
#define CPU_ILLEGAL_EB
#define CPU_ILLEGAL_EC
#define CPU_ILLEGAL_ED
#define CPU_ILLEGAL_F4
#define CPU_ILLEGAL_FC
#define CPU_ILLEGAL_FD

// ----------- CB-prefixed instructions macros ----------
// RLC r8
#define CPU_RLC_A
#define CPU_RLC_B
#define CPU_RLC_C
#define CPU_RLC_D
#define CPU_RLC_E
#define CPU_RLC_H
#define CPU_RLC_L
// RRC r8
#define CPU_RRC_A
#define CPU_RRC_B
#define CPU_RRC_C
#define CPU_RRC_D
#define CPU_RRC_E
#define CPU_RRC_H
#define CPU_RRC_L
// RL r8
#define CPU_RL_A
#define CPU_RL_B
#define CPU_RL_C
#define CPU_RL_D
#define CPU_RL_E
#define CPU_RL_H
#define CPU_RL_L
// RR r8
#define CPU_RR_A
#define CPU_RR_B
#define CPU_RR_C
#define CPU_RR_D
#define CPU_RR_E
#define CPU_RR_H
#define CPU_RR_L
// RLC [HL]
#define CPU_RLC_AT_HL
// RRC [HL]
#define CPU_RRC_AT_HL
// RL [HL]
#define CPU_RL_AT_HL
// RR [HL]
#define CPU_RR_AT_HL

// SLA r8
#define CPU_SLA_A
#define CPU_SLA_B
#define CPU_SLA_C
#define CPU_SLA_D
#define CPU_SLA_E
#define CPU_SLA_H
#define CPU_SLA_L
// SRA r8
#define CPU_SRA_A
#define CPU_SRA_B
#define CPU_SRA_C
#define CPU_SRA_D
#define CPU_SRA_E
#define CPU_SRA_H
#define CPU_SRA_L
// SRL r8
#define CPU_SRL_A
#define CPU_SRL_B
#define CPU_SRL_C
#define CPU_SRL_D
#define CPU_SRL_E
#define CPU_SRL_H
#define CPU_SRL_L
// SLA [HL]
#define CPU_SLA_AT_HL
// SRA [HL]
#define CPU_SRA_AT_HL
// SRL [HL]
#define CPU_SRL_AT_HL

// SWAP r8
#define CPU_SWAP_A
#define CPU_SWAP_B
#define CPU_SWAP_C
#define CPU_SWAP_D
#define CPU_SWAP_E
#define CPU_SWAP_H
#define CPU_SWAP_L
// SWAP [HL]
#define CPU_SWAP_AT_HL

// BIT b, r8
#define CPU_BIT_0_A
#define CPU_BIT_0_B
#define CPU_BIT_0_C
#define CPU_BIT_0_D
#define CPU_BIT_0_E
#define CPU_BIT_0_H
#define CPU_BIT_0_L
#define CPU_BIT_1_A
#define CPU_BIT_1_B
#define CPU_BIT_1_C
#define CPU_BIT_1_D
#define CPU_BIT_1_E
#define CPU_BIT_1_H
#define CPU_BIT_1_L
#define CPU_BIT_2_A
#define CPU_BIT_2_B
#define CPU_BIT_2_C
#define CPU_BIT_2_D
#define CPU_BIT_2_E
#define CPU_BIT_2_H
#define CPU_BIT_2_L
#define CPU_BIT_3_A
#define CPU_BIT_3_B
#define CPU_BIT_3_C
#define CPU_BIT_3_D
#define CPU_BIT_3_E
#define CPU_BIT_3_H
#define CPU_BIT_3_L
#define CPU_BIT_4_A
#define CPU_BIT_4_B
#define CPU_BIT_4_C
#define CPU_BIT_4_D
#define CPU_BIT_4_E
#define CPU_BIT_4_H
#define CPU_BIT_4_L
#define CPU_BIT_5_A
#define CPU_BIT_5_B
#define CPU_BIT_5_C
#define CPU_BIT_5_D
#define CPU_BIT_5_E
#define CPU_BIT_5_H
#define CPU_BIT_5_L
#define CPU_BIT_6_A
#define CPU_BIT_6_B
#define CPU_BIT_6_C
#define CPU_BIT_6_D
#define CPU_BIT_6_E
#define CPU_BIT_6_H
#define CPU_BIT_6_L
#define CPU_BIT_7_A
#define CPU_BIT_7_B
#define CPU_BIT_7_C
#define CPU_BIT_7_D
#define CPU_BIT_7_E
#define CPU_BIT_7_H
#define CPU_BIT_7_L
// BIT b, [HL]
#define CPU_BIT_0_AT_HL
#define CPU_BIT_1_AT_HL
#define CPU_BIT_2_AT_HL
#define CPU_BIT_3_AT_HL
#define CPU_BIT_4_AT_HL
#define CPU_BIT_5_AT_HL
#define CPU_BIT_6_AT_HL
#define CPU_BIT_7_AT_HL

// RES b, r8
#define CPU_RES_0_A
#define CPU_RES_0_B
#define CPU_RES_0_C
#define CPU_RES_0_D
#define CPU_RES_0_E
#define CPU_RES_0_H
#define CPU_RES_0_L
#define CPU_RES_1_A
#define CPU_RES_1_B
#define CPU_RES_1_C
#define CPU_RES_1_D
#define CPU_RES_1_E
#define CPU_RES_1_H
#define CPU_RES_1_L
#define CPU_RES_2_A
#define CPU_RES_2_B
#define CPU_RES_2_C
#define CPU_RES_2_D
#define CPU_RES_2_E
#define CPU_RES_2_H
#define CPU_RES_2_L
#define CPU_RES_3_A
#define CPU_RES_3_B
#define CPU_RES_3_C
#define CPU_RES_3_D
#define CPU_RES_3_E
#define CPU_RES_3_H
#define CPU_RES_3_L
#define CPU_RES_4_A
#define CPU_RES_4_B
#define CPU_RES_4_C
#define CPU_RES_4_D
#define CPU_RES_4_E
#define CPU_RES_4_H
#define CPU_RES_4_L
#define CPU_RES_5_A
#define CPU_RES_5_B
#define CPU_RES_5_C
#define CPU_RES_5_D
#define CPU_RES_5_E
#define CPU_RES_5_H
#define CPU_RES_5_L
#define CPU_RES_6_A
#define CPU_RES_6_B
#define CPU_RES_6_C
#define CPU_RES_6_D
#define CPU_RES_6_E
#define CPU_RES_6_H
#define CPU_RES_6_L
#define CPU_RES_7_A
#define CPU_RES_7_B
#define CPU_RES_7_C
#define CPU_RES_7_D
#define CPU_RES_7_E
#define CPU_RES_7_H
#define CPU_RES_7_L
// RES b, [HL]
#define CPU_RES_0_AT_HL
#define CPU_RES_1_AT_HL
#define CPU_RES_2_AT_HL
#define CPU_RES_3_AT_HL
#define CPU_RES_4_AT_HL
#define CPU_RES_5_AT_HL
#define CPU_RES_6_AT_HL
#define CPU_RES_7_AT_HL

// SET b, r8
#define CPU_SET_0_A
#define CPU_SET_0_B
#define CPU_SET_0_C
#define CPU_SET_0_D
#define CPU_SET_0_E
#define CPU_SET_0_H
#define CPU_SET_0_L
#define CPU_SET_1_A
#define CPU_SET_1_B
#define CPU_SET_1_C
#define CPU_SET_1_D
#define CPU_SET_1_E
#define CPU_SET_1_H
#define CPU_SET_1_L
#define CPU_SET_2_A
#define CPU_SET_2_B
#define CPU_SET_2_C
#define CPU_SET_2_D
#define CPU_SET_2_E
#define CPU_SET_2_H
#define CPU_SET_2_L
#define CPU_SET_3_A
#define CPU_SET_3_B
#define CPU_SET_3_C
#define CPU_SET_3_D
#define CPU_SET_3_E
#define CPU_SET_3_H
#define CPU_SET_3_L
#define CPU_SET_4_A
#define CPU_SET_4_B
#define CPU_SET_4_C
#define CPU_SET_4_D
#define CPU_SET_4_E
#define CPU_SET_4_H
#define CPU_SET_4_L
#define CPU_SET_5_A
#define CPU_SET_5_B
#define CPU_SET_5_C
#define CPU_SET_5_D
#define CPU_SET_5_E
#define CPU_SET_5_H
#define CPU_SET_5_L
#define CPU_SET_6_A
#define CPU_SET_6_B
#define CPU_SET_6_C
#define CPU_SET_6_D
#define CPU_SET_6_E
#define CPU_SET_6_H
#define CPU_SET_6_L
#define CPU_SET_7_A
#define CPU_SET_7_B
#define CPU_SET_7_C
#define CPU_SET_7_D
#define CPU_SET_7_E
#define CPU_SET_7_H
#define CPU_SET_7_L
// SET b, [HL]
#define CPU_SET_0_AT_HL
#define CPU_SET_1_AT_HL
#define CPU_SET_2_AT_HL
#define CPU_SET_3_AT_HL
#define CPU_SET_4_AT_HL
#define CPU_SET_5_AT_HL
#define CPU_SET_6_AT_HL
#define CPU_SET_7_AT_HL

} // namespace boyboy::cpu