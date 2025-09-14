/**
 * @file registers.h
 * @brief CPU registers for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */
#pragma once

#include <bit>
#include <concepts>
#include <cstdint>
#include <ostream>
#include <string>

namespace boyboy::cpu {

struct Flag {
    static constexpr uint8_t Zero = 0x80;      // Zero flag (Z)
    static constexpr uint8_t Substract = 0x40; // Subtraction flag (N) (BCD)
    static constexpr uint8_t HalfCarry = 0x20; // Half Carry flag (H) (BCD)
    static constexpr uint8_t Carry = 0x10;     // Carry flag (CY)

    // string conversion for test and debugging
    static std::string to_string(uint8_t flag)
    {
        switch (flag) {
        case cpu::Flag::Carry:
            return "Carry";
        case cpu::Flag::HalfCarry:
            return "Half Carry";
        case cpu::Flag::Substract:
            return "Substract";
        case cpu::Flag::Zero:
            return "Zero";
        default:
            return "Unknown Flag";
        }
    }
};

enum class Reg8Name : uint8_t { A, F, B, C, D, E, H, L };
enum class Reg16Name : uint8_t { AF, BC, DE, HL, SP, PC };

inline const char* to_string(Reg8Name r)
{
    switch (r) {
    case Reg8Name::A:
        return "A";
    case Reg8Name::F:
        return "F";
    case Reg8Name::B:
        return "B";
    case Reg8Name::C:
        return "C";
    case Reg8Name::D:
        return "D";
    case Reg8Name::E:
        return "E";
    case Reg8Name::H:
        return "H";
    case Reg8Name::L:
        return "L";
    default:
        return "Unknown";
    }
}

inline const char* to_string(Reg16Name r)
{
    switch (r) {
    case Reg16Name::AF:
        return "AF";
    case Reg16Name::BC:
        return "BC";
    case Reg16Name::DE:
        return "DE";
    case Reg16Name::HL:
        return "HL";
    case Reg16Name::SP:
        return "SP";
    case Reg16Name::PC:
        return "PC";
    default:
        return "Unknown";
    }
}

template <typename RegName>
concept RegNameEnum = std::same_as<RegName, Reg8Name> || std::same_as<RegName, Reg16Name>;

template <RegNameEnum RegName>
inline std::ostream& operator<<(std::ostream& os, RegName r)
{
    return os << to_string(r);
}

class Register16 {
    static_assert(std::endian::native == std::endian::little,
                  "This implementation assumes little-endian architecture");

public:
    constexpr Register16(uint16_t val = 0) : value_(val) {}

    // 16 bit access
    constexpr operator uint16_t() const { return value_; }
    constexpr Register16& operator=(uint16_t new_val)
    {
        value_ = new_val;
        return *this;
    }

    // High byte access (le: upper 8 bits)
    [[nodiscard]] constexpr uint8_t high() const { return value_ >> 8; }
    constexpr void high(uint8_t h) { value_ = (value_ & 0x00FF) | (static_cast<uint16_t>(h) << 8); }

    // Low byte access (le: lower 8 bits)
    [[nodiscard]] constexpr uint8_t low() const { return value_ & 0xFF; }
    constexpr void low(uint8_t l) { value_ = (value_ & 0xFF00) | l; }

    // Postfix operations
    constexpr Register16 operator++(int)
    {
        Register16 old = *this;
        ++value_;
        return old;
    }
    constexpr Register16 operator--(int)
    {
        Register16 old = *this;
        --value_;
        return old;
    }

    // Prefix operations
    constexpr Register16& operator++()
    {
        ++value_;
        return *this;
    }
    constexpr Register16& operator--()
    {
        --value_;
        return *this;
    }

protected:
    [[nodiscard]] constexpr uint16_t get_value() const { return value_; }
    constexpr void set_value(uint16_t val) { value_ = val; }

private:
    uint16_t value_;
};

// Special handling for AF (Accumulator & Flags) register (F uses only upper 4 bits)
class AFRegister : public Register16 {
public:
    constexpr AFRegister(uint16_t val = 0) : Register16(val & 0xFFF0) {}

    constexpr AFRegister& operator=(uint16_t new_val)
    {
        set_value(new_val & 0xFFF0);
        return *this;
    }

    // Add const version of low() to prevent hiding the base class method
    using Register16::low;

    // Override the non-const version to mask the lower 4 bits
    constexpr void low(uint8_t l) { set_value((get_value() & 0xFF00) | (l & 0xF0)); }

    // Flag access methods
    [[nodiscard]] constexpr bool zero_flag() const { return get_flag(Flag::Zero); }
    constexpr void zero_flag(bool set) { set_flag(Flag::Zero, set); }

    [[nodiscard]] constexpr bool carry_flag() const { return get_flag(Flag::Carry); }
    constexpr void carry_flag(bool set) { set_flag(Flag::Carry, set); }

    [[nodiscard]] constexpr bool substract_flag() const { return get_flag(Flag::Substract); }
    constexpr void substract_flag(bool set) { set_flag(Flag::Substract, set); }

    [[nodiscard]] constexpr bool half_carry_flag() const { return get_flag(Flag::HalfCarry); }
    constexpr void half_carry_flag(bool set) { set_flag(Flag::HalfCarry, set); }

// private:
    // Generic flag access
    [[nodiscard]] constexpr bool get_flag(uint8_t flag) const { return (get_value() & flag) != 0; }
    constexpr void set_flag(uint8_t flag, bool set)
    {
        uint8_t f = low();
        low(set ? (f | flag) : (f & ~flag));
    }
};

struct Registers {
    AFRegister af;
    Register16 bc, de, hl, sp, pc;

    // 8-bit register accessors for convenience
    [[nodiscard]] uint8_t a() const { return af.high(); }
    void a(uint8_t val) { af.high(val); }

    [[nodiscard]] uint8_t f() const { return af.low(); }
    void f(uint8_t val) { af.low(val); }

    [[nodiscard]] uint8_t b() const { return bc.high(); }
    void b(uint8_t val) { bc.high(val); }

    [[nodiscard]] uint8_t c() const { return bc.low(); }
    void c(uint8_t val) { bc.low(val); }

    [[nodiscard]] uint8_t d() const { return de.high(); }
    void d(uint8_t val) { de.high(val); }

    [[nodiscard]] uint8_t e() const { return de.low(); }
    void e(uint8_t val) { de.low(val); }

    [[nodiscard]] uint8_t h() const { return hl.high(); }
    void h(uint8_t val) { hl.high(val); }

    [[nodiscard]] uint8_t l() const { return hl.low(); }
    void l(uint8_t val) { hl.low(val); }
};

} // namespace boyboy::cpu