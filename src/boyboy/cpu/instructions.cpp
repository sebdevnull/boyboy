/**
 * @file instructions.cpp
 * @brief CPU instructions implementation for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/cpu/instructions.h"

#include <cstdint>

#include "boyboy/common/utils.h"
#include "boyboy/cpu/cpu.h"
#include "boyboy/cpu/cpu_constants.h"
#include "boyboy/cpu/registers.h"

namespace boyboy::cpu {

// Generic CPU instruction implementations (unprefixed)
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

// void Cpu::add_hl_r16(Reg16Name r16) {}

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

// void Cpu::pop_r16(Reg16Name r16) {}
// void Cpu::push_r16(Reg16Name r16) {}

// Generic CPU instruction implementations (CB-prefixed)

// Individual CPU instruction implementations (unprefixed)
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

// LD A, [a16]
void Cpu::ld_a_at_a16()
{
    uint8_t lsb = fetch();
    uint8_t msb = fetch();
    uint16_t addr = utils::to_u16(msb, lsb);
    uint8_t value = read_byte(addr);
    set_register(Reg8Name::A, value);
}

// LD [a16], A
void Cpu::ld_at_a16_a()
{
    uint8_t lsb = fetch();
    uint8_t msb = fetch();
    uint16_t addr = utils::to_u16(msb, lsb);
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
// clang-format on


// Individual CPU instruction implementations (CB-prefixed)

} // namespace boyboy::cpu

// Autogenerated CPU instruction stubs for unimplemented opcodes from Opcodes.json
#ifdef CPU_STUBS
// clang-format off
#include "generated/cpu_opcodes_impl.inc"
#include "generated/cpu_cbopcodes_impl.inc"
// clang-format on
#endif