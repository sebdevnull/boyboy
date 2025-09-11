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
#include <ios>
#include <optional>
#include <ostream>
#include <string>

#include "boyboy/common/utils.h"
#include "boyboy/cpu/opcodes.h"
#include "boyboy/cpu/registers.h"

namespace boyboy::test::cpu {

enum class OperandType : uint8_t { Reg8, Immediate, IndirectHL };

inline const char* to_string(OperandType op)
{
    switch (op) {
    case OperandType::Reg8:
        return "Reg8";
    case OperandType::Immediate:
        return "Immediate";
    case OperandType::IndirectHL:
        return "IndirectHL";
    default:
        return "Unknown";
    }
}

inline std::ostream& operator<<(std::ostream& os, OperandType op) { return os << to_string(op); }

struct R8Param {
    boyboy::cpu::Opcode opcode;
    OperandType operand_type = OperandType::Reg8;

    std::optional<boyboy::cpu::Reg8Name> src = std::nullopt;
    std::optional<boyboy::cpu::Reg8Name> dst = std::nullopt;

    std::optional<uint16_t> src_addr = std::nullopt;

    std::optional<uint8_t> initial_a = std::nullopt; // initial value of A register
    uint8_t src_value;
    std::optional<bool> carry_in = std::nullopt; // for ADC and SBC instructions
    uint8_t expected_value;

    bool expect_z = false;
    bool expect_n = false;
    bool expect_h = false;
    bool expect_c = false;

    std::string name;

    // Some operations behave as NOPs, but we still might want to execute them to test that the
    // corresponding functions are implemented (e.g. LD A, A)
    bool skip_assert = false;

    // Return the target register of the test
    [[nodiscard]] boyboy::cpu::Reg8Name target() const { return dst.value_or(*src); }

    // For better test case naming in GTest output
    friend std::ostream& operator<<(std::ostream& os, const R8Param& p)
    {
        // clang-format off
        os << p.name
           << " [opcode=" << boyboy::utils::PrettyHex{static_cast<uint8_t>(p.opcode)}
           << ", operand_type=" << p.operand_type;

        if (p.src.has_value())
        {
            os << ", src=" << *p.src;
        }

        if (p.dst.has_value())
        {
            os << ", dst=" << *p.dst;
        }

        if (p.src_addr.has_value())
        {
            os << ", src_addr=" << boyboy::utils::PrettyHex{*p.src_addr};
        }

        if (p.initial_a.has_value())
        {
            os << ", initial_a=" << boyboy::utils::PrettyHex{*p.initial_a};
        }

        os << ", src_value=" << boyboy::utils::PrettyHex{p.src_value}
           << ", expected_value=" << boyboy::utils::PrettyHex{p.expected_value}
           << std::dec  // switch back to decimal for flags
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