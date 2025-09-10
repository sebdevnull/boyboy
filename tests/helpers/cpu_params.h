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
#include <iomanip>
#include <ios>
#include <ostream>
#include <string>

#include "boyboy/cpu/opcodes.h"
#include "boyboy/cpu/registers.h"

namespace boyboy::test::cpu {

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

struct R8ALUParam {
    boyboy::cpu::Opcode opcode;
    boyboy::cpu::Reg8Name src;
    std::optional<boyboy::cpu::Reg8Name> dst;

    std::optional<uint8_t> initial_a; // initial value of A register
    uint8_t src_value;
    std::optional<bool> carry_in; // for ADC and SBC instructions
    uint8_t expected_value;


    bool expect_z;
    bool expect_n;
    bool expect_h;
    bool expect_c;

    std::string name;

    // Return the target register of the test
    [[nodiscard]] boyboy::cpu::Reg8Name target() const { return dst.value_or(src); }

    // For better test case naming in GTest output
    friend std::ostream& operator<<(std::ostream& os, const R8ALUParam& p)
    {
        // clang-format off
        os << p.name
           << " [opcode=0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(p.opcode)
           << ", src=" << static_cast<int>(p.src);

        if (p.dst.has_value())
        {
            os << ", dst=" << static_cast<int>(p.dst.value());
        }

        if (p.initial_a.has_value())
        {
            os << ", initial_a=0x" << std::hex << std::setw(2) << std::setfill('0') << +p.initial_a.value();
        }

        os << ", src_value=0x" << std::hex << std::setw(2) << std::setfill('0') << +p.src_value
           << ", expected_value=0x" << std::hex << std::setw(2) << std::setfill('0') << +p.expected_value
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

} // namespace boyboy::test::cpu