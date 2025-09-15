/**
 * @file instructions.cpp
 * @brief CPU instructions implementation for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

// TODO: repeated code for some instructions (e.g. rotate), refactor

#include "boyboy/cpu/instructions.h"

#include <cstdint>

#include "boyboy/common/errors.h"
#include "boyboy/common/utils.h"
#include "boyboy/cpu/cpu.h"
#include "boyboy/cpu/cpu_constants.h"
#include "boyboy/cpu/registers.h"

namespace boyboy::cpu {

// ---------- Generic CPU instruction implementations (unprefixed) ----------
void Cpu::ld_r8_n8(Reg8Name r8)
{
    uint8_t n8 = fetch();
    set_register(r8, n8);
}

void Cpu::ld_r8_r8(Reg8Name dst, Reg8Name src)
{
    if (dst == src) {
        // If same register do nothing (NOP)
        nop();
        return;
    }
    set_register(dst, get_register(src));
}

void Cpu::ld_r8_at_r16(Reg8Name dst, Reg16Name src)
{
    uint16_t addr = get_register(src);
    uint8_t value = read_byte(addr);
    set_register(dst, value);
}

void Cpu::ld_at_r16_r8(Reg16Name dst, Reg8Name src)
{
    uint16_t addr = get_register(dst);
    uint8_t value = get_register(src);
    write_byte(addr, value);
}

void Cpu::ld_r16_n16(Reg16Name r16)
{
    uint8_t lsb = fetch();
    uint8_t msb = fetch();
    uint16_t n16 = utils::to_u16(msb, lsb);
    set_register(r16, n16);
}

void Cpu::inc_r16(Reg16Name r16)
{
    set_register(r16, get_register(r16) + 1);
}

void Cpu::dec_r16(Reg16Name r16)
{
    set_register(r16, get_register(r16) - 1);
}

void Cpu::add_hl_r16(Reg16Name r16)
{
    uint16_t r16_val = get_register(r16);
    uint16_t hl = registers_.hl;
    uint32_t sum = uint32_t(hl) + uint32_t(r16_val);
    registers_.hl = sum & 0xFFFF;

    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, ((hl & 0x0FFF) + (r16_val & 0x0FFF)) > 0x0FFF);
    set_flag(Flag::Carry, sum > 0xFFFF);
}

void Cpu::inc_r8(Reg8Name r8)
{
    uint8_t res = get_register(r8) + 1;
    set_register(r8, res);
    set_flag(Flag::Zero, res == 0);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, (res & 0x0F) == 0x00);
}

void Cpu::dec_r8(Reg8Name r8)
{
    uint8_t res = get_register(r8) - 1;
    set_register(r8, res);
    set_flag(Flag::Zero, res == 0);
    set_flag(Flag::Substract, true);
    set_flag(Flag::HalfCarry, (res & 0x0F) == 0x0F);
}

// clang-format off
void Cpu::add_a_r8(Reg8Name r8) { add(get_register(r8), false); }
void Cpu::adc_a_r8(Reg8Name r8) { add(get_register(r8), true); }
void Cpu::sub_a_r8(Reg8Name r8) { sub(get_register(r8), false); }
void Cpu::sbc_a_r8(Reg8Name r8) { sub(get_register(r8), true); }
void Cpu::and_a_r8(Reg8Name r8) { aand(get_register(r8)); }
void Cpu::xor_a_r8(Reg8Name r8) { xxor(get_register(r8)); }
void Cpu::or_a_r8(Reg8Name r8) { oor(get_register(r8)); }
void Cpu::cp_a_r8(Reg8Name r8) { cp(get_register(r8)); }
// clang-format on

void Cpu::pop_r16(Reg16Name r16)
{
    uint16_t sp = get_sp();
    uint8_t lsb = read_byte(sp++);
    uint8_t msb = read_byte(sp++);
    set_register(r16, utils::to_u16(msb, lsb));
    set_sp(sp);
}

void Cpu::push_r16(Reg16Name r16)
{
    uint16_t r16_val = get_register(r16);
    uint16_t sp = get_sp();
    write_byte(--sp, utils::msb(r16_val));
    write_byte(--sp, utils::lsb(r16_val));
    set_sp(sp);
}

inline void Cpu::jp(uint16_t addr)
{
    set_pc(addr);
}
inline void Cpu::jp_z(uint16_t addr)
{
    if (get_flag(Flag::Zero)) {
        jp(addr);
    }
}
inline void Cpu::jp_nz(uint16_t addr)
{
    if (!get_flag(Flag::Zero)) {
        jp(addr);
    }
}
inline void Cpu::jp_c(uint16_t addr)
{
    if (get_flag(Flag::Carry)) {
        jp(addr);
    }
}
inline void Cpu::jp_nc(uint16_t addr)
{
    if (!get_flag(Flag::Carry)) {
        jp(addr);
    }
}

void Cpu::rst(uint8_t vector)
{
    push_r16(Reg16Name::PC);
    set_pc(vector);
}

// ---------- Generic CPU instruction implementations (CB-prefixed) ----------
void Cpu::rlc_r8(Reg8Name r8)
{
    uint8_t val = get_register(r8);
    bool new_carry = (val & 0x80) != 0;
    uint8_t result = (val << 1) | (new_carry ? 1 : 0);

    set_register(r8, result);

    set_flag(Flag::Zero, result == 0);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, false);
    set_flag(Flag::Carry, new_carry);
}

void Cpu::rrc_r8(Reg8Name r8)
{
    uint8_t val = get_register(r8);
    bool new_carry = (val & 0x01) != 0;
    uint8_t result = (val >> 1) | (new_carry ? 0x80 : 0);

    set_register(r8, result);

    set_flag(Flag::Zero, result == 0);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, false);
    set_flag(Flag::Carry, new_carry);
}

void Cpu::rl_r8(Reg8Name r8)
{
    uint8_t val = get_register(r8);
    bool old_carry = get_flag(Flag::Carry);
    bool new_carry = (val & 0x80) != 0;
    uint8_t result = (val << 1) | (old_carry ? 1 : 0);

    set_register(r8, result);

    set_flag(Flag::Zero, result == 0);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, false);
    set_flag(Flag::Carry, new_carry);
}

void Cpu::rr_r8(Reg8Name r8)
{
    uint8_t val = get_register(r8);
    bool old_carry = get_flag(Flag::Carry);
    bool new_carry = (val & 0x01) != 0;
    uint8_t result = (val >> 1) | (old_carry ? 0x80 : 0);

    set_register(r8, result);

    set_flag(Flag::Zero, result == 0);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, false);
    set_flag(Flag::Carry, new_carry);
}

void Cpu::sla_r8(Reg8Name r8)
{
    uint8_t val = get_register(r8);
    bool new_carry = (val & 0x80) != 0;
    uint8_t result = (val << 1) & 0xFE; // LSB is always 0

    set_register(r8, result);

    set_flag(Flag::Zero, result == 0);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, false);
    set_flag(Flag::Carry, new_carry);
}

void Cpu::sra_r8(Reg8Name r8)
{
    uint8_t val = get_register(r8);
    bool new_carry = (val & 0x01) != 0;
    uint8_t result = (val >> 1) | (val & 0x80); // MSB stays the same

    set_register(r8, result);

    set_flag(Flag::Zero, result == 0);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, false);
    set_flag(Flag::Carry, new_carry);
}

void Cpu::srl_r8(Reg8Name r8)
{
    uint8_t val = get_register(r8);
    bool new_carry = (val & 0x01) != 0;
    uint8_t result = (val >> 1) & 0x7F; // MSB is always 0

    set_register(r8, result);

    set_flag(Flag::Zero, result == 0);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, false);
    set_flag(Flag::Carry, new_carry);
}

void Cpu::swap_r8(Reg8Name r8)
{
    uint8_t val = get_register(r8);
    uint8_t result = (val << 4) | (val >> 4);

    set_register(r8, result);

    set_flag(Flag::Zero, result == 0);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, false);
    set_flag(Flag::Carry, false);
}

void Cpu::bit_b_r8(uint8_t bit, Reg8Name r8)
{
    uint8_t val = get_register(r8);
    bool bit_set = (val & (1 << bit)) != 0;

    set_flag(Flag::Zero, !bit_set);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, true);
}

void Cpu::bit_b_at_hl(uint8_t bit)
{
    uint16_t addr = registers_.hl;
    uint8_t val = read_byte(addr);
    bool bit_set = (val & (1 << bit)) != 0;

    set_flag(Flag::Zero, !bit_set);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, true);
}

void Cpu::res_b_r8(uint8_t bit, Reg8Name r8)
{
    uint8_t val = get_register(r8);
    uint8_t result = val & ~(1 << bit);

    set_register(r8, result);
}

void Cpu::res_b_at_hl(uint8_t bit)
{
    uint16_t addr = registers_.hl;
    uint8_t val = read_byte(addr);
    uint8_t result = val & ~(1 << bit);

    write_byte(addr, result);
}

void Cpu::set_b_r8(uint8_t bit, Reg8Name r8)
{
    uint8_t val = get_register(r8);
    uint8_t result = val | (1 << bit);

    set_register(r8, result);
}

void Cpu::set_b_at_hl(uint8_t bit)
{
    uint16_t addr = registers_.hl;
    uint8_t val = read_byte(addr);
    uint8_t result = val | (1 << bit);

    write_byte(addr, result);
}

// ---------- Individual CPU instruction implementations (unprefixed) ----------
// clang-format off
void Cpu::nop() { (void)*this; }

// INC r8
void Cpu::inc_a() { inc_r8(Reg8Name::A); }
void Cpu::inc_b() { inc_r8(Reg8Name::B); }
void Cpu::inc_c() { inc_r8(Reg8Name::C); }
void Cpu::inc_d() { inc_r8(Reg8Name::D); }
void Cpu::inc_e() { inc_r8(Reg8Name::E); }
void Cpu::inc_h() { inc_r8(Reg8Name::H); }
void Cpu::inc_l() { inc_r8(Reg8Name::L); }

// DEC r8
void Cpu::dec_a() { dec_r8(Reg8Name::A); }
void Cpu::dec_b() { dec_r8(Reg8Name::B); }
void Cpu::dec_c() { dec_r8(Reg8Name::C); }
void Cpu::dec_d() { dec_r8(Reg8Name::D); }
void Cpu::dec_e() { dec_r8(Reg8Name::E); }
void Cpu::dec_h() { dec_r8(Reg8Name::H); }
void Cpu::dec_l() { dec_r8(Reg8Name::L); }

// ADD A, r8
void Cpu::add_a_a() { add_a_r8(Reg8Name::A); }
void Cpu::add_a_b() { add_a_r8(Reg8Name::B); }
void Cpu::add_a_c() { add_a_r8(Reg8Name::C); }
void Cpu::add_a_d() { add_a_r8(Reg8Name::D); }
void Cpu::add_a_e() { add_a_r8(Reg8Name::E); }
void Cpu::add_a_h() { add_a_r8(Reg8Name::H); }
void Cpu::add_a_l() { add_a_r8(Reg8Name::L); }

// SUB A, r8
void Cpu::sub_a_a() { sub_a_r8(Reg8Name::A); }
void Cpu::sub_a_b() { sub_a_r8(Reg8Name::B); }
void Cpu::sub_a_c() { sub_a_r8(Reg8Name::C); }
void Cpu::sub_a_d() { sub_a_r8(Reg8Name::D); }
void Cpu::sub_a_e() { sub_a_r8(Reg8Name::E); }
void Cpu::sub_a_h() { sub_a_r8(Reg8Name::H); }
void Cpu::sub_a_l() { sub_a_r8(Reg8Name::L); }

// AND A, r8
void Cpu::and_a_a() { and_a_r8(Reg8Name::A); }
void Cpu::and_a_b() { and_a_r8(Reg8Name::B); }
void Cpu::and_a_c() { and_a_r8(Reg8Name::C); }
void Cpu::and_a_d() { and_a_r8(Reg8Name::D); }
void Cpu::and_a_e() { and_a_r8(Reg8Name::E); }
void Cpu::and_a_h() { and_a_r8(Reg8Name::H); }
void Cpu::and_a_l() { and_a_r8(Reg8Name::L); }

// OR A, r8
void Cpu::or_a_a() { or_a_r8(Reg8Name::A); }
void Cpu::or_a_b() { or_a_r8(Reg8Name::B); }
void Cpu::or_a_c() { or_a_r8(Reg8Name::C); }
void Cpu::or_a_d() { or_a_r8(Reg8Name::D); }
void Cpu::or_a_e() { or_a_r8(Reg8Name::E); }
void Cpu::or_a_h() { or_a_r8(Reg8Name::H); }
void Cpu::or_a_l() { or_a_r8(Reg8Name::L); }

// XOR A, r8
void Cpu::xor_a_a() { xor_a_r8(Reg8Name::A); }
void Cpu::xor_a_b() { xor_a_r8(Reg8Name::B); }
void Cpu::xor_a_c() { xor_a_r8(Reg8Name::C); }
void Cpu::xor_a_d() { xor_a_r8(Reg8Name::D); }
void Cpu::xor_a_e() { xor_a_r8(Reg8Name::E); }
void Cpu::xor_a_h() { xor_a_r8(Reg8Name::H); }
void Cpu::xor_a_l() { xor_a_r8(Reg8Name::L); }

// CP A, r8
void Cpu::cp_a_a() { cp_a_r8(Reg8Name::A); }
void Cpu::cp_a_b() { cp_a_r8(Reg8Name::B); }
void Cpu::cp_a_c() { cp_a_r8(Reg8Name::C); }
void Cpu::cp_a_d() { cp_a_r8(Reg8Name::D); }
void Cpu::cp_a_e() { cp_a_r8(Reg8Name::E); }
void Cpu::cp_a_h() { cp_a_r8(Reg8Name::H); }
void Cpu::cp_a_l() { cp_a_r8(Reg8Name::L); }

// ADC A, r8
void Cpu::adc_a_a() { adc_a_r8(Reg8Name::A); }
void Cpu::adc_a_b() { adc_a_r8(Reg8Name::B); }
void Cpu::adc_a_c() { adc_a_r8(Reg8Name::C); }
void Cpu::adc_a_d() { adc_a_r8(Reg8Name::D); }
void Cpu::adc_a_e() { adc_a_r8(Reg8Name::E); }
void Cpu::adc_a_h() { adc_a_r8(Reg8Name::H); }
void Cpu::adc_a_l() { adc_a_r8(Reg8Name::L); }

// SBC A, r8
void Cpu::sbc_a_a() { sbc_a_r8(Reg8Name::A); }
void Cpu::sbc_a_b() { sbc_a_r8(Reg8Name::B); }
void Cpu::sbc_a_c() { sbc_a_r8(Reg8Name::C); }
void Cpu::sbc_a_d() { sbc_a_r8(Reg8Name::D); }
void Cpu::sbc_a_e() { sbc_a_r8(Reg8Name::E); }
void Cpu::sbc_a_h() { sbc_a_r8(Reg8Name::H); }
void Cpu::sbc_a_l() { sbc_a_r8(Reg8Name::L); }
// clang-format on

// INC [HL]
void Cpu::inc_at_hl()
{
    uint16_t addr = get_register(Reg16Name::HL);
    uint8_t res = read_byte(addr) + 1;

    write_byte(addr, res);

    set_flag(Flag::Zero, res == 0);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, (res & 0x0F) == 0x00);
}

// DEC [HL]
void Cpu::dec_at_hl()
{
    uint16_t addr = get_register(Reg16Name::HL);
    uint8_t res = read_byte(addr) - 1;

    write_byte(addr, res);

    set_flag(Flag::Zero, res == 0);
    set_flag(Flag::Substract, true);
    set_flag(Flag::HalfCarry, (res & 0x0F) == 0x0F);
}

// ADD A, [HL]
void Cpu::add_a_at_hl()
{
    uint16_t addr = get_register(Reg16Name::HL);
    uint8_t value = read_byte(addr);

    add(value, false);
}

// ADC A, [HL]
void Cpu::adc_a_at_hl()
{
    uint16_t addr = get_register(Reg16Name::HL);
    uint8_t value = read_byte(addr);

    add(value, true);
}

// SUB A, [HL]
void Cpu::sub_a_at_hl()
{
    uint16_t addr = get_register(Reg16Name::HL);
    uint8_t value = read_byte(addr);

    sub(value, false);
}

// SBC A, [HL]
void Cpu::sbc_a_at_hl()
{
    uint16_t addr = get_register(Reg16Name::HL);
    uint8_t value = read_byte(addr);

    sub(value, true);
}

// AND A, [HL]
void Cpu::and_a_at_hl()
{
    uint16_t addr = get_register(Reg16Name::HL);
    uint8_t value = read_byte(addr);

    aand(value);
}

// XOR A, [HL]
void Cpu::xor_a_at_hl()
{
    uint16_t addr = get_register(Reg16Name::HL);
    uint8_t value = read_byte(addr);

    xxor(value);
}

// OR A, [HL]
void Cpu::or_a_at_hl()
{
    uint16_t addr = get_register(Reg16Name::HL);
    uint8_t value = read_byte(addr);

    oor(value);
}

// CP A, [HL]
void Cpu::cp_a_at_hl()
{
    uint16_t addr = get_register(Reg16Name::HL);
    uint8_t value = read_byte(addr);

    cp(value);
}

// ADD A, n8
void Cpu::add_a_n8()
{
    uint8_t value = fetch();
    add(value, false);
}

// ADC A, n8
void Cpu::adc_a_n8()
{
    uint8_t value = fetch();
    add(value, true);
}

// SUB A, n8
void Cpu::sub_a_n8()
{
    uint8_t value = fetch();
    sub(value, false);
}

// SBC A, n8
void Cpu::sbc_a_n8()
{
    uint8_t value = fetch();
    sub(value, true);
}

// AND A, n8
void Cpu::and_a_n8()
{
    uint8_t value = fetch();
    aand(value);
}

// XOR A, n8
void Cpu::xor_a_n8()
{
    uint8_t value = fetch();
    xxor(value);
}

// OR A, n8
void Cpu::or_a_n8()
{
    uint8_t value = fetch();
    oor(value);
}

// CP A, n8
void Cpu::cp_a_n8()
{
    uint8_t value = fetch();
    cp(value);
}

// clang-format off

// LD r8, r8'
// A
void Cpu::ld_a_a() { ld_r8_r8(Reg8Name::A, Reg8Name::A); }
void Cpu::ld_a_b() { ld_r8_r8(Reg8Name::A, Reg8Name::B); }
void Cpu::ld_a_c() { ld_r8_r8(Reg8Name::A, Reg8Name::C); }
void Cpu::ld_a_d() { ld_r8_r8(Reg8Name::A, Reg8Name::D); }
void Cpu::ld_a_e() { ld_r8_r8(Reg8Name::A, Reg8Name::E); }
void Cpu::ld_a_h() { ld_r8_r8(Reg8Name::A, Reg8Name::H); }
void Cpu::ld_a_l() { ld_r8_r8(Reg8Name::A, Reg8Name::L); }
// B
void Cpu::ld_b_a() { ld_r8_r8(Reg8Name::B, Reg8Name::A); }
void Cpu::ld_b_b() { ld_r8_r8(Reg8Name::B, Reg8Name::B); }
void Cpu::ld_b_c() { ld_r8_r8(Reg8Name::B, Reg8Name::C); }
void Cpu::ld_b_d() { ld_r8_r8(Reg8Name::B, Reg8Name::D); }
void Cpu::ld_b_e() { ld_r8_r8(Reg8Name::B, Reg8Name::E); }
void Cpu::ld_b_h() { ld_r8_r8(Reg8Name::B, Reg8Name::H); }
void Cpu::ld_b_l() { ld_r8_r8(Reg8Name::B, Reg8Name::L); }
// C
void Cpu::ld_c_a() { ld_r8_r8(Reg8Name::C, Reg8Name::A); }
void Cpu::ld_c_b() { ld_r8_r8(Reg8Name::C, Reg8Name::B); }
void Cpu::ld_c_c() { ld_r8_r8(Reg8Name::C, Reg8Name::C); }
void Cpu::ld_c_d() { ld_r8_r8(Reg8Name::C, Reg8Name::D); }
void Cpu::ld_c_e() { ld_r8_r8(Reg8Name::C, Reg8Name::E); }
void Cpu::ld_c_h() { ld_r8_r8(Reg8Name::C, Reg8Name::H); }
void Cpu::ld_c_l() { ld_r8_r8(Reg8Name::C, Reg8Name::L); }
// D
void Cpu::ld_d_a() { ld_r8_r8(Reg8Name::D, Reg8Name::A); }
void Cpu::ld_d_b() { ld_r8_r8(Reg8Name::D, Reg8Name::B); }
void Cpu::ld_d_c() { ld_r8_r8(Reg8Name::D, Reg8Name::C); }
void Cpu::ld_d_d() { ld_r8_r8(Reg8Name::D, Reg8Name::D); }
void Cpu::ld_d_e() { ld_r8_r8(Reg8Name::D, Reg8Name::E); }
void Cpu::ld_d_h() { ld_r8_r8(Reg8Name::D, Reg8Name::H); }
void Cpu::ld_d_l() { ld_r8_r8(Reg8Name::D, Reg8Name::L); }
// E
void Cpu::ld_e_a() { ld_r8_r8(Reg8Name::E, Reg8Name::A); }
void Cpu::ld_e_b() { ld_r8_r8(Reg8Name::E, Reg8Name::B); }
void Cpu::ld_e_c() { ld_r8_r8(Reg8Name::E, Reg8Name::C); }
void Cpu::ld_e_d() { ld_r8_r8(Reg8Name::E, Reg8Name::D); }
void Cpu::ld_e_e() { ld_r8_r8(Reg8Name::E, Reg8Name::E); }
void Cpu::ld_e_h() { ld_r8_r8(Reg8Name::E, Reg8Name::H); }
void Cpu::ld_e_l() { ld_r8_r8(Reg8Name::E, Reg8Name::L); }
// H
void Cpu::ld_h_a() { ld_r8_r8(Reg8Name::H, Reg8Name::A); }
void Cpu::ld_h_b() { ld_r8_r8(Reg8Name::H, Reg8Name::B); }
void Cpu::ld_h_c() { ld_r8_r8(Reg8Name::H, Reg8Name::C); }
void Cpu::ld_h_d() { ld_r8_r8(Reg8Name::H, Reg8Name::D); }
void Cpu::ld_h_e() { ld_r8_r8(Reg8Name::H, Reg8Name::E); }
void Cpu::ld_h_h() { ld_r8_r8(Reg8Name::H, Reg8Name::H); }
void Cpu::ld_h_l() { ld_r8_r8(Reg8Name::H, Reg8Name::L); }
// L
void Cpu::ld_l_a() { ld_r8_r8(Reg8Name::L, Reg8Name::A); }
void Cpu::ld_l_b() { ld_r8_r8(Reg8Name::L, Reg8Name::B); }
void Cpu::ld_l_c() { ld_r8_r8(Reg8Name::L, Reg8Name::C); }
void Cpu::ld_l_d() { ld_r8_r8(Reg8Name::L, Reg8Name::D); }
void Cpu::ld_l_e() { ld_r8_r8(Reg8Name::L, Reg8Name::E); }
void Cpu::ld_l_h() { ld_r8_r8(Reg8Name::L, Reg8Name::H); }
void Cpu::ld_l_l() { ld_r8_r8(Reg8Name::L, Reg8Name::L); }

// LD r8, n8
void Cpu::ld_a_n8() { ld_r8_n8(Reg8Name::A); }
void Cpu::ld_b_n8() { ld_r8_n8(Reg8Name::B); }
void Cpu::ld_c_n8() { ld_r8_n8(Reg8Name::C); }
void Cpu::ld_d_n8() { ld_r8_n8(Reg8Name::D); }
void Cpu::ld_e_n8() { ld_r8_n8(Reg8Name::E); }
void Cpu::ld_h_n8() { ld_r8_n8(Reg8Name::H); }
void Cpu::ld_l_n8() { ld_r8_n8(Reg8Name::L); }

// LD r8, [HL]
void Cpu::ld_a_at_hl() { ld_r8_at_r16(Reg8Name::A, Reg16Name::HL); }
void Cpu::ld_b_at_hl() { ld_r8_at_r16(Reg8Name::B, Reg16Name::HL); }
void Cpu::ld_c_at_hl() { ld_r8_at_r16(Reg8Name::C, Reg16Name::HL); }
void Cpu::ld_d_at_hl() { ld_r8_at_r16(Reg8Name::D, Reg16Name::HL); }
void Cpu::ld_e_at_hl() { ld_r8_at_r16(Reg8Name::E, Reg16Name::HL); }
void Cpu::ld_h_at_hl() { ld_r8_at_r16(Reg8Name::H, Reg16Name::HL); }
void Cpu::ld_l_at_hl() { ld_r8_at_r16(Reg8Name::L, Reg16Name::HL); }

// LD [HL], r8
void Cpu::ld_at_hl_a() { ld_at_r16_r8(Reg16Name::HL, Reg8Name::A); }
void Cpu::ld_at_hl_b() { ld_at_r16_r8(Reg16Name::HL, Reg8Name::B); }
void Cpu::ld_at_hl_c() { ld_at_r16_r8(Reg16Name::HL, Reg8Name::C); }
void Cpu::ld_at_hl_d() { ld_at_r16_r8(Reg16Name::HL, Reg8Name::D); }
void Cpu::ld_at_hl_e() { ld_at_r16_r8(Reg16Name::HL, Reg8Name::E); }
void Cpu::ld_at_hl_h() { ld_at_r16_r8(Reg16Name::HL, Reg8Name::H); }
void Cpu::ld_at_hl_l() { ld_at_r16_r8(Reg16Name::HL, Reg8Name::L); }

// LD A, [BC/DE] and LD [BC/DE], A
void Cpu::ld_a_at_bc() { ld_r8_at_r16(Reg8Name::A, Reg16Name::BC); }
void Cpu::ld_a_at_de() { ld_r8_at_r16(Reg8Name::A, Reg16Name::DE); }
void Cpu::ld_at_bc_a() { ld_at_r16_r8(Reg16Name::BC, Reg8Name::A); }
void Cpu::ld_at_de_a() { ld_at_r16_r8(Reg16Name::DE, Reg8Name::A); }
// clang-format on

// LD [HL], n8
void Cpu::ld_at_hl_n8()
{
    uint16_t addr = get_register(Reg16Name::HL);
    uint8_t n8 = fetch();
    write_byte(addr, n8);
}

// LD A, [a16]
void Cpu::ld_a_at_a16()
{
    uint16_t addr = fetch_n16();
    uint8_t value = read_byte(addr);
    set_register(Reg8Name::A, value);
}

// LD [a16], A
void Cpu::ld_at_a16_a()
{
    uint16_t addr = fetch_n16();
    uint8_t value = get_register(Reg8Name::A);
    write_byte(addr, value);
}

// LD A, [HL+]
void Cpu::ld_a_at_hl_inc()
{
    uint16_t addr = get_register(Reg16Name::HL);
    uint8_t value = read_byte(addr);
    set_register(Reg8Name::A, value);
    set_register(Reg16Name::HL, addr + 1);
}

// LD A, [HL-]
void Cpu::ld_a_at_hl_dec()
{
    uint16_t addr = get_register(Reg16Name::HL);
    uint8_t value = read_byte(addr);
    set_register(Reg8Name::A, value);
    set_register(Reg16Name::HL, addr - 1);
}

// LD [HL+], A
void Cpu::ld_at_hl_inc_a()
{
    uint16_t addr = get_register(Reg16Name::HL);
    uint8_t value = get_register(Reg8Name::A);
    write_byte(addr, value);
    set_register(Reg16Name::HL, addr + 1);
}

// LD [HL-], A
void Cpu::ld_at_hl_dec_a()
{
    uint16_t addr = get_register(Reg16Name::HL);
    uint8_t value = get_register(Reg8Name::A);
    write_byte(addr, value);
    set_register(Reg16Name::HL, addr - 1);
}

// LDH A, [C]
void Cpu::ldh_a_at_c()
{
    uint8_t c = get_register(Reg8Name::C);
    uint16_t addr = HighRAMOffset + c;
    uint8_t value = read_byte(addr);
    set_register(Reg8Name::A, value);
}

// LDH [C], A
void Cpu::ldh_at_c_a()
{
    uint8_t c = get_register(Reg8Name::C);
    uint16_t addr = HighRAMOffset + c;
    uint8_t value = get_register(Reg8Name::A);
    write_byte(addr, value);
}

// LDH A, [a8]
void Cpu::ldh_a_at_a8()
{
    uint8_t n8 = fetch();
    uint16_t addr = HighRAMOffset + n8;
    uint8_t value = read_byte(addr);
    set_register(Reg8Name::A, value);
}

// LDH [a8], A
void Cpu::ldh_at_a8_a()
{
    uint8_t n8 = fetch();
    uint16_t addr = HighRAMOffset + n8;
    uint8_t value = get_register(Reg8Name::A);
    write_byte(addr, value);
}

// clang-format off
// INC r16
void Cpu::inc_bc() { inc_r16(Reg16Name::BC); }
void Cpu::inc_de() { inc_r16(Reg16Name::DE); }
void Cpu::inc_hl() { inc_r16(Reg16Name::HL); }
void Cpu::inc_sp() { inc_r16(Reg16Name::SP); }
// DEC r6
void Cpu::dec_bc() { dec_r16(Reg16Name::BC); }
void Cpu::dec_de() { dec_r16(Reg16Name::DE); }
void Cpu::dec_hl() { dec_r16(Reg16Name::HL); }
void Cpu::dec_sp() { dec_r16(Reg16Name::SP); }
// ADD HL, r16
void Cpu::add_hl_bc() { add_hl_r16(Reg16Name::BC); }
void Cpu::add_hl_de() { add_hl_r16(Reg16Name::DE); }
void Cpu::add_hl_hl() { add_hl_r16(Reg16Name::HL); }
void Cpu::add_hl_sp() { add_hl_r16(Reg16Name::SP); }
// clang-format on

// ADD SP, e8
void Cpu::add_sp_e8()
{
    auto e8 = static_cast<int8_t>(fetch());
    uint16_t sp = get_sp();
    uint32_t sum = uint32_t(sp) + int32_t(e8);

    set_sp(sum & 0xFFFF);

    set_flag(Flag::Zero, false);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, ((sp & 0x0F) + (e8 & 0x0F)) > 0x0F);
    set_flag(Flag::Carry, ((sp & 0xFF) + (e8 & 0xFF)) > 0xFF);
}

// clang-format off
// LD r16, n16
void Cpu::ld_bc_n16() { ld_r16_n16(Reg16Name::BC); }
void Cpu::ld_de_n16() { ld_r16_n16(Reg16Name::DE); }
void Cpu::ld_hl_n16() { ld_r16_n16(Reg16Name::HL); }
void Cpu::ld_sp_n16() { ld_r16_n16(Reg16Name::SP); }
// clang-format on

// LD [a16], SP
void Cpu::ld_at_a16_sp()
{
    uint16_t addr = fetch_n16();
    uint16_t sp = get_sp();
    write_byte(addr, utils::lsb(sp));
    write_byte(addr + 1, utils::msb(sp));
}

// LD HL, SP+e8
void Cpu::ld_hl_sp_inc_e8()
{
    auto e8 = static_cast<int8_t>(fetch());
    uint16_t sp = get_sp();
    uint32_t sum = uint32_t(sp) + int32_t(e8);

    registers_.hl = sum & 0xFFFF;

    set_flag(Flag::Zero, false);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, ((sp & 0x0F) + (e8 & 0x0F)) > 0x0F);
    set_flag(Flag::Carry, ((sp & 0xFF) + (e8 & 0xFF)) > 0xFF);
}

// LD SP, HL
void Cpu::ld_sp_hl()
{
    set_sp(get_register(Reg16Name::HL));
}

// clang-format off
// POP r16
void Cpu::pop_bc() { pop_r16(Reg16Name::BC); }
void Cpu::pop_de() { pop_r16(Reg16Name::DE); }
void Cpu::pop_hl() { pop_r16(Reg16Name::HL); }
void Cpu::pop_af() { pop_r16(Reg16Name::AF); }
// PUSH r16
void Cpu::push_bc() { push_r16(Reg16Name::BC); }
void Cpu::push_de() { push_r16(Reg16Name::DE); }
void Cpu::push_hl() { push_r16(Reg16Name::HL); }
void Cpu::push_af() { push_r16(Reg16Name::AF); }
// clang-format on

// JP a16
void Cpu::jp_a16()
{
    uint16_t addr = fetch_n16();
    jp(addr);
}
// JP Z, a16
void Cpu::jp_z_a16()
{
    uint16_t addr = fetch_n16();
    jp_z(addr);
}
// JP NZ, a16
void Cpu::jp_nz_a16()
{
    uint16_t addr = fetch_n16();
    jp_nz(addr);
}
// JP C, a16
void Cpu::jp_c_a16()
{
    uint16_t addr = fetch_n16();
    jp_c(addr);
}
// JP NC, a16
void Cpu::jp_nc_a16()
{
    uint16_t addr = fetch_n16();
    jp_nc(addr);
}
// JP HL
void Cpu::jp_hl()
{
    uint16_t hl = get_register(Reg16Name::HL);
    jp(hl);
}
// JR e8
void Cpu::jr_e8()
{
    auto e8 = static_cast<int8_t>(fetch());
    jp(get_pc() + e8);
}
// JR Z, e8
void Cpu::jr_z_e8()
{
    auto e8 = static_cast<int8_t>(fetch());
    jp_z(get_pc() + e8);
}
// JR NZ, e8
void Cpu::jr_nz_e8()
{
    auto e8 = static_cast<int8_t>(fetch());
    jp_nz(get_pc() + e8);
}
// JR C, e8
void Cpu::jr_c_e8()
{
    auto e8 = static_cast<int8_t>(fetch());
    jp_c(get_pc() + e8);
}
// JR NC, e8
void Cpu::jr_nc_e8()
{
    auto e8 = static_cast<int8_t>(fetch());
    jp_nc(get_pc() + e8);
}

// CALL
void Cpu::call_a16()
{
    uint16_t addr = fetch_n16();
    uint16_t pc = get_pc();
    uint16_t sp = get_sp();
    write_byte(--sp, utils::msb(pc));
    write_byte(--sp, utils::lsb(pc));
    set_sp(sp);
    set_pc(addr);
}
// CALL Z, a16
void Cpu::call_z_a16()
{
    if (get_flag(Flag::Zero)) {
        call_a16();
    }
    else {
        // If not taken, need to discard the fetched address
        fetch_n16();
    }
}
// CALL NZ, a16
void Cpu::call_nz_a16()
{
    if (!get_flag(Flag::Zero)) {
        call_a16();
    }
    else {
        // If not taken, need to discard the fetched address
        fetch_n16();
    }
}
// CALL C, a16
void Cpu::call_c_a16()
{
    if (get_flag(Flag::Carry)) {
        call_a16();
    }
    else {
        // If not taken, need to discard the fetched address
        fetch_n16();
    }
}
// CALL NC, a16
void Cpu::call_nc_a16()
{
    if (!get_flag(Flag::Carry)) {
        call_a16();
    }
    else {
        // If not taken, need to discard the fetched address
        fetch_n16();
    }
}

// RET
void Cpu::ret()
{
    uint16_t sp = get_sp();
    uint8_t lsb = read_byte(sp++);
    uint8_t msb = read_byte(sp++);
    set_pc(utils::to_u16(msb, lsb));
    set_sp(sp);
}
// RET Z
void Cpu::ret_z()
{
    if (get_flag(Flag::Zero)) {
        ret();
    }
}
// RET NZ
void Cpu::ret_nz()
{
    if (!get_flag(Flag::Zero)) {
        ret();
    }
}
// RET C
void Cpu::ret_c()
{
    if (get_flag(Flag::Carry)) {
        ret();
    }
}
// RET NC
void Cpu::ret_nc()
{
    if (!get_flag(Flag::Carry)) {
        ret();
    }
}
// RETI
void Cpu::reti()
{
    ret();
    ime_ = true;
}
// RST n
// clang-format off
void Cpu::rst_00() { rst(0x00); }
void Cpu::rst_08() { rst(0x08); }
void Cpu::rst_10() { rst(0x10); }
void Cpu::rst_18() { rst(0x18); }
void Cpu::rst_20() { rst(0x20); }
void Cpu::rst_28() { rst(0x28); }
void Cpu::rst_30() { rst(0x30); }
void Cpu::rst_38() { rst(0x38); }
// clang-format on

// DAA
void Cpu::daa()
{
    uint8_t adjustment = 0;
    bool carry = get_flag(Flag::Carry);
    bool half_carry = get_flag(Flag::HalfCarry);
    bool subtract = get_flag(Flag::Substract);
    uint8_t a = get_register(Reg8Name::A);

    if (subtract) {
        if (half_carry) {
            adjustment += 0x06;
        }
        if (carry) {
            adjustment += 0x60;
        }
        a -= adjustment;
    }
    else {
        if (half_carry || (a & 0x0F) > 0x09) {
            adjustment += 0x06;
        }
        if (carry || a > 0x99) {
            adjustment += 0x60;
            carry = true;
        }
        a += adjustment;
    }

    set_register(Reg8Name::A, a);
    set_flag(Flag::Zero, a == 0);
    set_flag(Flag::Carry, carry);
    set_flag(Flag::HalfCarry, false);
}

// CPL
void Cpu::cpl()
{
    uint8_t a = get_register(Reg8Name::A);
    a = ~a;
    set_register(Reg8Name::A, a);
    set_flag(Flag::Substract, true);
    set_flag(Flag::HalfCarry, true);
}
// SCF
void Cpu::scf()
{
    set_flag(Flag::Carry, true);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, false);
}
// CCF
void Cpu::ccf()
{
    bool carry = get_flag(Flag::Carry);
    set_flag(Flag::Carry, !carry);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, false);
}

// RLA
void Cpu::rla()
{
    uint8_t a = get_register(Reg8Name::A);
    bool carry = get_flag(Flag::Carry);
    bool new_carry = (a & 0x80) != 0;

    a = (a << 1) | (carry ? 1 : 0);
    set_register(Reg8Name::A, a);

    set_flag(Flag::Zero, false);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, false);
    set_flag(Flag::Carry, new_carry);
}
// RRA
void Cpu::rra()
{
    uint8_t a = get_register(Reg8Name::A);
    bool carry = get_flag(Flag::Carry);
    bool new_carry = (a & 0x01) != 0;

    a = (a >> 1) | (carry ? 0x80 : 0);
    set_register(Reg8Name::A, a);

    set_flag(Flag::Zero, false);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, false);
    set_flag(Flag::Carry, new_carry);
}
// RLCA
void Cpu::rlca()
{
    uint8_t a = get_register(Reg8Name::A);
    bool new_carry = (a & 0x80) != 0;

    a = (a << 1) | (new_carry ? 1 : 0);
    set_register(Reg8Name::A, a);

    set_flag(Flag::Zero, false);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, false);
    set_flag(Flag::Carry, new_carry);
}
// RRCA
void Cpu::rrca()
{
    uint8_t a = get_register(Reg8Name::A);
    bool new_carry = (a & 0x01) != 0;

    a = (a >> 1) | (new_carry ? 0x80 : 0);
    set_register(Reg8Name::A, a);

    set_flag(Flag::Zero, false);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, false);
    set_flag(Flag::Carry, new_carry);
}

// TODO: implement properly when interrupts are handled
// EI
void Cpu::ei()
{
    ime_next_ = true;
}
// DI
void Cpu::di()
{
    ime_ = false;
}

// HALT
void Cpu::halt()
{
    halted_ = true;
}
// STOP
void Cpu::stop_n8()
{
    (void)fetch(); // Discard the next byte (should be 0x00)
    // We throw temporarily to see if we hit this instruction
    throw std::runtime_error("STOP instruction not implemented");
}

// Illegal opcodes
void Cpu::illegal_opcode(uint8_t opcode)
{
    throw errors::IllegalOpcode(opcode);
}

// clang-format off
// NOLINTBEGIN(readability-convert-member-functions-to-static)
void Cpu::prefix() { illegal_opcode(0xCB); } // CB-prefixed opcodes should be handled separately
void Cpu::illegal_d3() { illegal_opcode(0xD3); }
void Cpu::illegal_db() { illegal_opcode(0xDB); }
void Cpu::illegal_dd() { illegal_opcode(0xDD); }
void Cpu::illegal_e3() { illegal_opcode(0xE3); }
void Cpu::illegal_e4() { illegal_opcode(0xE4); }
void Cpu::illegal_eb() { illegal_opcode(0xEB); }
void Cpu::illegal_ec() { illegal_opcode(0xEC); }
void Cpu::illegal_ed() { illegal_opcode(0xED); }
void Cpu::illegal_f4() { illegal_opcode(0xF4); }
void Cpu::illegal_fc() { illegal_opcode(0xFC); }
void Cpu::illegal_fd() { illegal_opcode(0xFD); }
// NOLINTEND(readability-convert-member-functions-to-static)
// clang-format on

// ---------- Individual CPU instruction implementations (CB-prefixed) ----------
// clang-format off
// RLC r8
void Cpu::rlc_a() { rlc_r8(Reg8Name::A); }
void Cpu::rlc_b() { rlc_r8(Reg8Name::B); }
void Cpu::rlc_c() { rlc_r8(Reg8Name::C); }
void Cpu::rlc_d() { rlc_r8(Reg8Name::D); }
void Cpu::rlc_e() { rlc_r8(Reg8Name::E); }
void Cpu::rlc_h() { rlc_r8(Reg8Name::H); }
void Cpu::rlc_l() { rlc_r8(Reg8Name::L); }
// RRC r8
void Cpu::rrc_a() { rrc_r8(Reg8Name::A); }
void Cpu::rrc_b() { rrc_r8(Reg8Name::B); }
void Cpu::rrc_c() { rrc_r8(Reg8Name::C); }
void Cpu::rrc_d() { rrc_r8(Reg8Name::D); }
void Cpu::rrc_e() { rrc_r8(Reg8Name::E); }
void Cpu::rrc_h() { rrc_r8(Reg8Name::H); }
void Cpu::rrc_l() { rrc_r8(Reg8Name::L); }
// RL r8
void Cpu::rl_a() { rl_r8(Reg8Name::A); }
void Cpu::rl_b() { rl_r8(Reg8Name::B); }
void Cpu::rl_c() { rl_r8(Reg8Name::C); }
void Cpu::rl_d() { rl_r8(Reg8Name::D); }
void Cpu::rl_e() { rl_r8(Reg8Name::E); }
void Cpu::rl_h() { rl_r8(Reg8Name::H); }
void Cpu::rl_l() { rl_r8(Reg8Name::L); }
// RR r8
void Cpu::rr_a() { rr_r8(Reg8Name::A); }
void Cpu::rr_b() { rr_r8(Reg8Name::B); }
void Cpu::rr_c() { rr_r8(Reg8Name::C); }
void Cpu::rr_d() { rr_r8(Reg8Name::D); }
void Cpu::rr_e() { rr_r8(Reg8Name::E); }
void Cpu::rr_h() { rr_r8(Reg8Name::H); }
void Cpu::rr_l() { rr_r8(Reg8Name::L); }
// clang-format on

// RLC [HL]
void Cpu::rlc_at_hl()
{
    uint16_t addr = get_register(Reg16Name::HL);
    uint8_t value = read_byte(addr);
    bool new_carry = (value & 0x80) != 0;

    value = (value << 1) | (new_carry ? 1 : 0);
    write_byte(addr, value);

    set_flag(Flag::Zero, value == 0);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, false);
    set_flag(Flag::Carry, new_carry);
}
// RRC [HL]
void Cpu::rrc_at_hl()
{
    uint16_t addr = get_register(Reg16Name::HL);
    uint8_t value = read_byte(addr);
    bool new_carry = (value & 0x01) != 0;

    value = (value >> 1) | (new_carry ? 0x80 : 0);
    write_byte(addr, value);

    set_flag(Flag::Zero, value == 0);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, false);
    set_flag(Flag::Carry, new_carry);
}
// RL [HL]
void Cpu::rl_at_hl()
{
    uint16_t addr = get_register(Reg16Name::HL);
    uint8_t value = read_byte(addr);
    bool carry = get_flag(Flag::Carry);
    bool new_carry = (value & 0x80) != 0;

    value = (value << 1) | (carry ? 1 : 0);
    write_byte(addr, value);

    set_flag(Flag::Zero, value == 0);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, false);
    set_flag(Flag::Carry, new_carry);
}
// RR [HL]
void Cpu::rr_at_hl()
{
    uint16_t addr = get_register(Reg16Name::HL);
    uint8_t value = read_byte(addr);
    bool carry = get_flag(Flag::Carry);
    bool new_carry = (value & 0x01) != 0;

    value = (value >> 1) | (carry ? 0x80 : 0);
    write_byte(addr, value);

    set_flag(Flag::Zero, value == 0);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, false);
    set_flag(Flag::Carry, new_carry);
}

// clang-format off
// SLA r8
void Cpu::sla_a() { sla_r8(Reg8Name::A); }
void Cpu::sla_b() { sla_r8(Reg8Name::B); }
void Cpu::sla_c() { sla_r8(Reg8Name::C); }
void Cpu::sla_d() { sla_r8(Reg8Name::D); }
void Cpu::sla_e() { sla_r8(Reg8Name::E); }
void Cpu::sla_h() { sla_r8(Reg8Name::H); }
void Cpu::sla_l() { sla_r8(Reg8Name::L); }
// SRA r8
void Cpu::sra_a() { sra_r8(Reg8Name::A); }
void Cpu::sra_b() { sra_r8(Reg8Name::B); }
void Cpu::sra_c() { sra_r8(Reg8Name::C); }
void Cpu::sra_d() { sra_r8(Reg8Name::D); }
void Cpu::sra_e() { sra_r8(Reg8Name::E); }
void Cpu::sra_h() { sra_r8(Reg8Name::H); }
void Cpu::sra_l() { sra_r8(Reg8Name::L); }
// SRL r8
void Cpu::srl_a() { srl_r8(Reg8Name::A); }
void Cpu::srl_b() { srl_r8(Reg8Name::B); }
void Cpu::srl_c() { srl_r8(Reg8Name::C); }
void Cpu::srl_d() { srl_r8(Reg8Name::D); }
void Cpu::srl_e() { srl_r8(Reg8Name::E); }
void Cpu::srl_h() { srl_r8(Reg8Name::H); }
void Cpu::srl_l() { srl_r8(Reg8Name::L); }
// clang-format on

// SLA [HL]
void Cpu::sla_at_hl()
{
    uint16_t addr = get_register(Reg16Name::HL);
    uint8_t value = read_byte(addr);
    bool new_carry = (value & 0x80) != 0;

    value = (value << 1) & 0xFE; // LSB is set to 0
    write_byte(addr, value);

    set_flag(Flag::Zero, value == 0);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, false);
    set_flag(Flag::Carry, new_carry);
}
// SRA [HL]
void Cpu::sra_at_hl()
{
    uint16_t addr = get_register(Reg16Name::HL);
    uint8_t value = read_byte(addr);
    bool new_carry = (value & 0x01) != 0;
    bool msb = (value & 0x80) != 0;

    value = (value >> 1) | (msb ? 0x80 : 0); // MSB does not change
    write_byte(addr, value);

    set_flag(Flag::Zero, value == 0);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, false);
    set_flag(Flag::Carry, new_carry);
}
// SRL [HL]
void Cpu::srl_at_hl()
{
    uint16_t addr = get_register(Reg16Name::HL);
    uint8_t value = read_byte(addr);
    bool new_carry = (value & 0x01) != 0;

    value = (value >> 1) & 0x7F; // MSB is set to 0
    write_byte(addr, value);

    set_flag(Flag::Zero, value == 0);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, false);
    set_flag(Flag::Carry, new_carry);
}

// clang-format off
// SWAP r8
void Cpu::swap_a() { swap_r8(Reg8Name::A); }
void Cpu::swap_b() { swap_r8(Reg8Name::B); }
void Cpu::swap_c() { swap_r8(Reg8Name::C); }
void Cpu::swap_d() { swap_r8(Reg8Name::D); }    
void Cpu::swap_e() { swap_r8(Reg8Name::E); }
void Cpu::swap_h() { swap_r8(Reg8Name::H); }
void Cpu::swap_l() { swap_r8(Reg8Name::L); }
// clang-format on

// SWAP [HL]
void Cpu::swap_at_hl()
{
    uint16_t addr = get_register(Reg16Name::HL);
    uint8_t value = read_byte(addr);

    value = (value << 4) | (value >> 4);
    write_byte(addr, value);

    set_flag(Flag::Zero, value == 0);
    set_flag(Flag::Substract, false);
    set_flag(Flag::HalfCarry, false);
    set_flag(Flag::Carry, false);
}

// clang-format off
// BIT b, r8
void Cpu::bit_0_a() { bit_b_r8(0, Reg8Name::A); }
void Cpu::bit_0_b() { bit_b_r8(0, Reg8Name::B); }
void Cpu::bit_0_c() { bit_b_r8(0, Reg8Name::C); }
void Cpu::bit_0_d() { bit_b_r8(0, Reg8Name::D); }
void Cpu::bit_0_e() { bit_b_r8(0, Reg8Name::E); }
void Cpu::bit_0_h() { bit_b_r8(0, Reg8Name::H); }
void Cpu::bit_0_l() { bit_b_r8(0, Reg8Name::L); }
void Cpu::bit_1_a() { bit_b_r8(1, Reg8Name::A); }
void Cpu::bit_1_b() { bit_b_r8(1, Reg8Name::B); }
void Cpu::bit_1_c() { bit_b_r8(1, Reg8Name::C); }
void Cpu::bit_1_d() { bit_b_r8(1, Reg8Name::D); }
void Cpu::bit_1_e() { bit_b_r8(1, Reg8Name::E); }
void Cpu::bit_1_h() { bit_b_r8(1, Reg8Name::H); }
void Cpu::bit_1_l() { bit_b_r8(1, Reg8Name::L); }
void Cpu::bit_2_a() { bit_b_r8(2, Reg8Name::A); }
void Cpu::bit_2_b() { bit_b_r8(2, Reg8Name::B); }
void Cpu::bit_2_c() { bit_b_r8(2, Reg8Name::C); }
void Cpu::bit_2_d() { bit_b_r8(2, Reg8Name::D); }
void Cpu::bit_2_e() { bit_b_r8(2, Reg8Name::E); }
void Cpu::bit_2_h() { bit_b_r8(2, Reg8Name::H); }
void Cpu::bit_2_l() { bit_b_r8(2, Reg8Name::L); }
void Cpu::bit_3_a() { bit_b_r8(3, Reg8Name::A); }
void Cpu::bit_3_b() { bit_b_r8(3, Reg8Name::B); }
void Cpu::bit_3_c() { bit_b_r8(3, Reg8Name::C); }
void Cpu::bit_3_d() { bit_b_r8(3, Reg8Name::D); }
void Cpu::bit_3_e() { bit_b_r8(3, Reg8Name::E); }
void Cpu::bit_3_h() { bit_b_r8(3, Reg8Name::H); }
void Cpu::bit_3_l() { bit_b_r8(3, Reg8Name::L); }
void Cpu::bit_4_a() { bit_b_r8(4, Reg8Name::A); }
void Cpu::bit_4_b() { bit_b_r8(4, Reg8Name::B); }
void Cpu::bit_4_c() { bit_b_r8(4, Reg8Name::C); }
void Cpu::bit_4_d() { bit_b_r8(4, Reg8Name::D); }
void Cpu::bit_4_e() { bit_b_r8(4, Reg8Name::E); }
void Cpu::bit_4_h() { bit_b_r8(4, Reg8Name::H); }
void Cpu::bit_4_l() { bit_b_r8(4, Reg8Name::L); }
void Cpu::bit_5_a() { bit_b_r8(5, Reg8Name::A); }
void Cpu::bit_5_b() { bit_b_r8(5, Reg8Name::B); }
void Cpu::bit_5_c() { bit_b_r8(5, Reg8Name::C); }
void Cpu::bit_5_d() { bit_b_r8(5, Reg8Name::D); }
void Cpu::bit_5_e() { bit_b_r8(5, Reg8Name::E); }
void Cpu::bit_5_h() { bit_b_r8(5, Reg8Name::H); }
void Cpu::bit_5_l() { bit_b_r8(5, Reg8Name::L); }
void Cpu::bit_6_a() { bit_b_r8(6, Reg8Name::A); }
void Cpu::bit_6_b() { bit_b_r8(6, Reg8Name::B); }
void Cpu::bit_6_c() { bit_b_r8(6, Reg8Name::C); }
void Cpu::bit_6_d() { bit_b_r8(6, Reg8Name::D); }
void Cpu::bit_6_e() { bit_b_r8(6, Reg8Name::E); }
void Cpu::bit_6_h() { bit_b_r8(6, Reg8Name::H); }
void Cpu::bit_6_l() { bit_b_r8(6, Reg8Name::L); }
void Cpu::bit_7_a() { bit_b_r8(7, Reg8Name::A); }
void Cpu::bit_7_b() { bit_b_r8(7, Reg8Name::B); }
void Cpu::bit_7_c() { bit_b_r8(7, Reg8Name::C); }
void Cpu::bit_7_d() { bit_b_r8(7, Reg8Name::D); }
void Cpu::bit_7_e() { bit_b_r8(7, Reg8Name::E); }
void Cpu::bit_7_h() { bit_b_r8(7, Reg8Name::H); }
void Cpu::bit_7_l() { bit_b_r8(7, Reg8Name::L); }
// BIT b, [HL]
void Cpu::bit_0_at_hl() { bit_b_at_hl(0); }
void Cpu::bit_1_at_hl() { bit_b_at_hl(1); }
void Cpu::bit_2_at_hl() { bit_b_at_hl(2); }
void Cpu::bit_3_at_hl() { bit_b_at_hl(3); }
void Cpu::bit_4_at_hl() { bit_b_at_hl(4); }
void Cpu::bit_5_at_hl() { bit_b_at_hl(5); }
void Cpu::bit_6_at_hl() { bit_b_at_hl(6); }
void Cpu::bit_7_at_hl() { bit_b_at_hl(7); }
// RES b, r8
void Cpu::res_0_a() { res_b_r8(0, Reg8Name::A); }
void Cpu::res_0_b() { res_b_r8(0, Reg8Name::B); }
void Cpu::res_0_c() { res_b_r8(0, Reg8Name::C); }
void Cpu::res_0_d() { res_b_r8(0, Reg8Name::D); }
void Cpu::res_0_e() { res_b_r8(0, Reg8Name::E); }
void Cpu::res_0_h() { res_b_r8(0, Reg8Name::H); }
void Cpu::res_0_l() { res_b_r8(0, Reg8Name::L); }
void Cpu::res_1_a() { res_b_r8(1, Reg8Name::A); }
void Cpu::res_1_b() { res_b_r8(1, Reg8Name::B); }
void Cpu::res_1_c() { res_b_r8(1, Reg8Name::C); }
void Cpu::res_1_d() { res_b_r8(1, Reg8Name::D); }
void Cpu::res_1_e() { res_b_r8(1, Reg8Name::E); }
void Cpu::res_1_h() { res_b_r8(1, Reg8Name::H); }
void Cpu::res_1_l() { res_b_r8(1, Reg8Name::L); }
void Cpu::res_2_a() { res_b_r8(2, Reg8Name::A); }
void Cpu::res_2_b() { res_b_r8(2, Reg8Name::B); }
void Cpu::res_2_c() { res_b_r8(2, Reg8Name::C); }
void Cpu::res_2_d() { res_b_r8(2, Reg8Name::D); }
void Cpu::res_2_e() { res_b_r8(2, Reg8Name::E); }
void Cpu::res_2_h() { res_b_r8(2, Reg8Name::H); }
void Cpu::res_2_l() { res_b_r8(2, Reg8Name::L); }
void Cpu::res_3_a() { res_b_r8(3, Reg8Name::A); }
void Cpu::res_3_b() { res_b_r8(3, Reg8Name::B); }
void Cpu::res_3_c() { res_b_r8(3, Reg8Name::C); }
void Cpu::res_3_d() { res_b_r8(3, Reg8Name::D); }
void Cpu::res_3_e() { res_b_r8(3, Reg8Name::E); }
void Cpu::res_3_h() { res_b_r8(3, Reg8Name::H); }   
void Cpu::res_3_l() { res_b_r8(3, Reg8Name::L); }
void Cpu::res_4_a() { res_b_r8(4, Reg8Name::A); }
void Cpu::res_4_b() { res_b_r8(4, Reg8Name::B); }
void Cpu::res_4_c() { res_b_r8(4, Reg8Name::C); }
void Cpu::res_4_d() { res_b_r8(4, Reg8Name::D); }
void Cpu::res_4_e() { res_b_r8(4, Reg8Name::E); }
void Cpu::res_4_h() { res_b_r8(4, Reg8Name::H); }
void Cpu::res_4_l() { res_b_r8(4, Reg8Name::L); }
void Cpu::res_5_a() { res_b_r8(5, Reg8Name::A); }
void Cpu::res_5_b() { res_b_r8(5, Reg8Name::B); }
void Cpu::res_5_c() { res_b_r8(5, Reg8Name::C); }
void Cpu::res_5_d() { res_b_r8(5, Reg8Name::D); }
void Cpu::res_5_e() { res_b_r8(5, Reg8Name::E); }
void Cpu::res_5_h() { res_b_r8(5, Reg8Name::H); }   
void Cpu::res_5_l() { res_b_r8(5, Reg8Name::L); }
void Cpu::res_6_a() { res_b_r8(6, Reg8Name::A); }
void Cpu::res_6_b() { res_b_r8(6, Reg8Name::B); }
void Cpu::res_6_c() { res_b_r8(6, Reg8Name::C); }
void Cpu::res_6_d() { res_b_r8(6, Reg8Name::D); }
void Cpu::res_6_e() { res_b_r8(6, Reg8Name::E); }
void Cpu::res_6_h() { res_b_r8(6, Reg8Name::H); }
void Cpu::res_6_l() { res_b_r8(6, Reg8Name::L); }
void Cpu::res_7_a() { res_b_r8(7, Reg8Name::A); }
void Cpu::res_7_b() { res_b_r8(7, Reg8Name::B); }
void Cpu::res_7_c() { res_b_r8(7, Reg8Name::C); }
void Cpu::res_7_d() { res_b_r8(7, Reg8Name::D); }
void Cpu::res_7_e() { res_b_r8(7, Reg8Name::E); }
void Cpu::res_7_h() { res_b_r8(7, Reg8Name::H); }
void Cpu::res_7_l() { res_b_r8(7, Reg8Name::L); }
// RES b, [HL]
void Cpu::res_0_at_hl() { res_b_at_hl(0); }
void Cpu::res_1_at_hl() { res_b_at_hl(1); }
void Cpu::res_2_at_hl() { res_b_at_hl(2); }
void Cpu::res_3_at_hl() { res_b_at_hl(3); }
void Cpu::res_4_at_hl() { res_b_at_hl(4); }
void Cpu::res_5_at_hl() { res_b_at_hl(5); }
void Cpu::res_6_at_hl() { res_b_at_hl(6); }
void Cpu::res_7_at_hl() { res_b_at_hl(7); }
// SET b, r8
void Cpu::set_0_a() { set_b_r8(0, Reg8Name::A); }
void Cpu::set_0_b() { set_b_r8(0, Reg8Name::B); }
void Cpu::set_0_c() { set_b_r8(0, Reg8Name::C); }
void Cpu::set_0_d() { set_b_r8(0, Reg8Name::D); }
void Cpu::set_0_e() { set_b_r8(0, Reg8Name::E); }
void Cpu::set_0_h() { set_b_r8(0, Reg8Name::H); }
void Cpu::set_0_l() { set_b_r8(0, Reg8Name::L); }
void Cpu::set_1_a() { set_b_r8(1, Reg8Name::A); }
void Cpu::set_1_b() { set_b_r8(1, Reg8Name::B); }
void Cpu::set_1_c() { set_b_r8(1, Reg8Name::C); }
void Cpu::set_1_d() { set_b_r8(1, Reg8Name::D); }
void Cpu::set_1_e() { set_b_r8(1, Reg8Name::E); }
void Cpu::set_1_h() { set_b_r8(1, Reg8Name::H); }
void Cpu::set_1_l() { set_b_r8(1, Reg8Name::L); }
void Cpu::set_2_a() { set_b_r8(2, Reg8Name::A); }
void Cpu::set_2_b() { set_b_r8(2, Reg8Name::B); }
void Cpu::set_2_c() { set_b_r8(2, Reg8Name::C); }
void Cpu::set_2_d() { set_b_r8(2, Reg8Name::D); }
void Cpu::set_2_e() { set_b_r8(2, Reg8Name::E); }
void Cpu::set_2_h() { set_b_r8(2, Reg8Name::H); }
void Cpu::set_2_l() { set_b_r8(2, Reg8Name::L); }
void Cpu::set_3_a() { set_b_r8(3, Reg8Name::A); }
void Cpu::set_3_b() { set_b_r8(3, Reg8Name::B); }
void Cpu::set_3_c() { set_b_r8(3, Reg8Name::C); }
void Cpu::set_3_d() { set_b_r8(3, Reg8Name::D); }
void Cpu::set_3_e() { set_b_r8(3, Reg8Name::E); }
void Cpu::set_3_h() { set_b_r8(3, Reg8Name::H); }   
void Cpu::set_3_l() { set_b_r8(3, Reg8Name::L); }
void Cpu::set_4_a() { set_b_r8(4, Reg8Name::A); }
void Cpu::set_4_b() { set_b_r8(4, Reg8Name::B); }
void Cpu::set_4_c() { set_b_r8(4, Reg8Name::C); }
void Cpu::set_4_d() { set_b_r8(4, Reg8Name::D); }
void Cpu::set_4_e() { set_b_r8(4, Reg8Name::E); }
void Cpu::set_4_h() { set_b_r8(4, Reg8Name::H); }
void Cpu::set_4_l() { set_b_r8(4, Reg8Name::L); }
void Cpu::set_5_a() { set_b_r8(5, Reg8Name::A); }
void Cpu::set_5_b() { set_b_r8(5, Reg8Name::B); }
void Cpu::set_5_c() { set_b_r8(5, Reg8Name::C); }
void Cpu::set_5_d() { set_b_r8(5, Reg8Name::D); }
void Cpu::set_5_e() { set_b_r8(5, Reg8Name::E); }
void Cpu::set_5_h() { set_b_r8(5, Reg8Name::H); }   
void Cpu::set_5_l() { set_b_r8(5, Reg8Name::L); }
void Cpu::set_6_a() { set_b_r8(6, Reg8Name::A); }
void Cpu::set_6_b() { set_b_r8(6, Reg8Name::B); }
void Cpu::set_6_c() { set_b_r8(6, Reg8Name::C); }
void Cpu::set_6_d() { set_b_r8(6, Reg8Name::D); }
void Cpu::set_6_e() { set_b_r8(6, Reg8Name::E); }
void Cpu::set_6_h() { set_b_r8(6, Reg8Name::H); }   
void Cpu::set_6_l() { set_b_r8(6, Reg8Name::L); }
void Cpu::set_7_a() { set_b_r8(7, Reg8Name::A); }
void Cpu::set_7_b() { set_b_r8(7, Reg8Name::B); }
void Cpu::set_7_c() { set_b_r8(7, Reg8Name::C); }
void Cpu::set_7_d() { set_b_r8(7, Reg8Name::D); }
void Cpu::set_7_e() { set_b_r8(7, Reg8Name::E); }
void Cpu::set_7_h() { set_b_r8(7, Reg8Name::H); }   
void Cpu::set_7_l() { set_b_r8(7, Reg8Name::L); }
// SET b, [HL]
void Cpu::set_0_at_hl() { set_b_at_hl(0); }
void Cpu::set_1_at_hl() { set_b_at_hl(1); }
void Cpu::set_2_at_hl() { set_b_at_hl(2); }
void Cpu::set_3_at_hl() { set_b_at_hl(3); }
void Cpu::set_4_at_hl() { set_b_at_hl(4); }
void Cpu::set_5_at_hl() { set_b_at_hl(5); }
void Cpu::set_6_at_hl() { set_b_at_hl(6); }
void Cpu::set_7_at_hl() { set_b_at_hl(7); }
// clang-format on

} // namespace boyboy::cpu

// Autogenerated CPU instruction stubs for unimplemented opcodes from Opcodes.json
#ifdef CPU_STUBS
// clang-format off
#include "generated/cpu_opcodes_impl.inc"
#include "generated/cpu_cbopcodes_impl.inc"
// clang-format on
#endif