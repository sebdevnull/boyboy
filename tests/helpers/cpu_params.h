/**
 * @file cpu_params.h
 * @brief CPU instruction test parameters.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <gtest/gtest.h>

#include <algorithm>
#include <cstdint>
#include <expected>
#include <optional>
#include <ostream>
#include <string>
#include <variant>

#include "boyboy/common/utils.h"
#include "boyboy/cpu/cpu.h"
#include "boyboy/cpu/opcodes.h"
#include "boyboy/cpu/registers.h"

namespace boyboy::test::cpu {

enum class OperandType : uint8_t {
    Register,
    Immediate,
    Indirect,
    Memory,
    HighRAM,
};

inline const char* to_string(OperandType op)
{
    switch (op) {
    case OperandType::Register:
        return "Register";
    case OperandType::Immediate:
        return "Immediate";
    case OperandType::Indirect:
        return "Indirect";
    case OperandType::Memory:
        return "Memory";
    case OperandType::HighRAM:
        return "HighRAM";
    default:
        return "Unknown";
    }
}

inline std::ostream& operator<<(std::ostream& os, OperandType op) { return os << to_string(op); }

// TODO: not used for now, but could be a cleaner alternative to RegParam
struct Operand {
    using Variant = std::variant<boyboy::cpu::Reg8Name,
                                 boyboy::cpu::Reg16Name,
                                 uint16_t>; // for immediates/addresses

    OperandType type             = OperandType::Register;
    std::optional<Variant> value = std::nullopt;

    Operand() = default;
    Operand(OperandType t, std::nullopt_t nopt) : type(t), value(nopt) {}
    Operand(OperandType t, Variant v) : type(t), value(v) {}
};

class RegParam {
public:
    using Variant = std::variant<boyboy::cpu::Reg8Name, boyboy::cpu::Reg16Name>;

    // constructors
    RegParam() = default;
    RegParam(boyboy::cpu::Reg8Name r8) : value_(r8) {}
    RegParam(boyboy::cpu::Reg16Name r16) : value_(r16) {}

    // getter
    [[nodiscard]] const Variant& get_value() const { return value_; }

    // assignment operators
    RegParam& operator=(boyboy::cpu::Reg8Name r8)
    {
        value_ = r8;
        return *this;
    }
    RegParam& operator=(boyboy::cpu::Reg16Name r16)
    {
        value_ = r16;
        return *this;
    }

    // equality
    bool operator==(const RegParam& other) const noexcept { return value_ == other.value_; }
    bool operator!=(const RegParam& other) const noexcept { return !(*this == other); }

    // helpers
    [[nodiscard]] bool is_r8() const
    {
        return std::holds_alternative<boyboy::cpu::Reg8Name>(value_);
    }
    [[nodiscard]] bool is_r16() const
    {
        return std::holds_alternative<boyboy::cpu::Reg16Name>(value_);
    }
    [[nodiscard]] boyboy::cpu::Reg8Name get_r8() const
    {
        return std::get<boyboy::cpu::Reg8Name>(value_);
    }
    [[nodiscard]] boyboy::cpu::Reg16Name get_r16() const
    {
        return std::get<boyboy::cpu::Reg16Name>(value_);
    }

    [[nodiscard]] bool overlaps(const RegParam& other) const
    {
        // Compare register pairs
        if (std::holds_alternative<boyboy::cpu::Reg16Name>(value_) &&
            std::holds_alternative<boyboy::cpu::Reg16Name>(other.value_)) {
            return std::get<boyboy::cpu::Reg16Name>(value_) ==
                   std::get<boyboy::cpu::Reg16Name>(other.value_);
        }

        // Compare register 8-bit halves with 16-bit full
        if (std::holds_alternative<boyboy::cpu::Reg8Name>(value_) &&
            std::holds_alternative<boyboy::cpu::Reg16Name>(other.value_)) {
            return half_matches(std::get<boyboy::cpu::Reg8Name>(value_),
                                std::get<boyboy::cpu::Reg16Name>(other.value_));
        }
        if (std::holds_alternative<boyboy::cpu::Reg16Name>(value_) &&
            std::holds_alternative<boyboy::cpu::Reg8Name>(other.value_)) {
            return half_matches(std::get<boyboy::cpu::Reg8Name>(other.value_),
                                std::get<boyboy::cpu::Reg16Name>(value_));
        }

        // Both 8-bit
        return std::get<boyboy::cpu::Reg8Name>(value_) ==
               std::get<boyboy::cpu::Reg8Name>(other.value_);
    }

private:
    Variant value_;

    static bool half_matches(boyboy::cpu::Reg8Name r8, boyboy::cpu::Reg16Name r16)
    {
        switch (r16) {
        case boyboy::cpu::Reg16Name::HL:
            return (r8 == boyboy::cpu::Reg8Name::H || r8 == boyboy::cpu::Reg8Name::L);
        case boyboy::cpu::Reg16Name::BC:
            return (r8 == boyboy::cpu::Reg8Name::B || r8 == boyboy::cpu::Reg8Name::C);
        case boyboy::cpu::Reg16Name::DE:
            return (r8 == boyboy::cpu::Reg8Name::D || r8 == boyboy::cpu::Reg8Name::E);
        case boyboy::cpu::Reg16Name::AF:
            return (r8 == boyboy::cpu::Reg8Name::A /* || r8 == Reg8Name::F */);
        default:
            return false;
        }
    }
};

struct InstrParam {
    boyboy::cpu::Opcode opcode;

    OperandType src_op_type                = OperandType::Register;
    std::optional<OperandType> dst_op_type = OperandType::Register;

    std::optional<RegParam> src = std::nullopt;
    std::optional<RegParam> dst = std::nullopt;

    std::optional<uint16_t> src_addr = std::nullopt;
    std::optional<uint16_t> dst_addr = std::nullopt;

    std::optional<uint8_t> initial_a   = std::nullopt; // initial value of A register
    std::optional<uint16_t> initial_pc = std::nullopt; // initial value of PC register
    std::optional<uint16_t> initial_sp = std::nullopt; // initial value of SP register

    std::optional<uint16_t> stack_value = std::nullopt;

    std::optional<bool> carry_in = std::nullopt; // for ADC and SBC instructions

    std::variant<uint8_t, uint16_t> src_value;
    std::variant<uint8_t, uint16_t> expected_value;

    bool expect_z = false;
    bool expect_n = false;
    bool expect_h = false;
    bool expect_c = false;

    std::string name;

    // Some operations behave as NOPs, but we still might want to execute them to test that the
    // corresponding functions are implemented (e.g. LD A, A)
    bool skip_assert = false;

    // Extra validators to run at the assert stage
    // NOLINTNEXTLINE(readability-redundant-member-init)
    std::vector<std::function<void(const boyboy::cpu::Cpu&, const InstrParam&)>> validators{};

    // Return the target register of the test
    [[nodiscard]] RegParam target() const { return dst.value_or(*src); }
    [[nodiscard]] uint16_t target_addr() const { return dst_addr.value_or(*src_addr); }
    [[nodiscard]] OperandType target_operand() const { return dst_op_type.value_or(src_op_type); }

    // Accessors
    [[nodiscard]] uint8_t src_value8() const { return std::get<uint8_t>(src_value); }
    [[nodiscard]] uint16_t src_value16() const { return std::get<uint16_t>(src_value); }
    [[nodiscard]] uint8_t expected_value8() const { return std::get<uint8_t>(expected_value); }
    [[nodiscard]] uint16_t expected_value16() const { return std::get<uint16_t>(expected_value); }

    // For better test case naming in GTest output
    friend std::ostream& operator<<(std::ostream& os, const InstrParam& p)
    {
        // clang-format off
        os << p.name
           << " [opcode=" << boyboy::utils::PrettyHex{static_cast<uint8_t>(p.opcode)}
           << ", src_type=" << p.src_op_type;
           
        if (p.dst_op_type)
        {
            os << ", dst_type=" << *p.dst_op_type;
        }

        if (p.src)
        {
            std::visit([&](auto r){ os << ", src=" << r; }, p.src->get_value());
        }
        if (p.dst)
        {
            std::visit([&](auto r){ os << ", dst=" << r; }, p.dst->get_value());
        }

        auto print_opt = [&](auto&& opt, const char* name) {
            if (opt) {
                os << ", " << name << "=" << boyboy::utils::PrettyHex{*opt};
            }
        };

        print_opt(p.src_addr, "src_addr");
        print_opt(p.dst_addr, "dst_addr");
        print_opt(p.initial_a, "initial_a");
        print_opt(p.initial_pc, "initial_pc");
        print_opt(p.initial_sp, "initial_sp");
        print_opt(p.stack_value, "stack_value");

        if (p.carry_in)
        {
            os << ", carry_in=" << (*p.carry_in ? 1 : 0);
        }

        os << ", src_value=" 
           << std::visit([](auto&& val){return boyboy::utils::PrettyHex{val};},p.src_value)
           << ", expected_value="
           << std::visit([](auto&& val){return boyboy::utils::PrettyHex{val};},p.expected_value)
           << ", Z=" << (p.expect_z ? 1 : 0)
           << ", N=" << (p.expect_n ? 1 : 0)
           << ", H=" << (p.expect_h ? 1 : 0)
           << ", C=" << (p.expect_c ? 1 : 0)
           << "]";
        // clang-format on

        return os;
    }
};

// Parameter name generator (removes non-alphanumeric characters from name)
template <typename ParamType>
std::string param_name(const ::testing::TestParamInfo<ParamType>& info)
{
    std::string name = info.param.name;
    name.erase(std::remove_if(name.begin(),
                              name.end(),
                              [](char c) { return !std::isalnum(static_cast<unsigned char>(c)); }),
               name.end());
    return name;
}

} // namespace boyboy::test::cpu