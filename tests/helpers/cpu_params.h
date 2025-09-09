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

struct R8Param {
    boyboy::cpu::Opcode opcode;
    boyboy::cpu::Reg8Name src;
    std::optional<boyboy::cpu::Reg8Name> dst;
    uint8_t src_value;
    uint8_t expected_value;
    bool expect_z, expect_n, expect_h, expect_c;
    std::string name;

    // Return the target register of the test
    [[nodiscard]] boyboy::cpu::Reg8Name target() const { return dst.value_or(src); }

    // For better test case naming in GTest output
    friend std::ostream& operator<<(std::ostream& os, const R8Param& p)
    {
        // clang-format off
        os << p.name
           << " [reg=" << static_cast<int>(p.src)
           << " init=0x" << std::hex << +p.src_value
           << " -> exp=0x" << +p.expected_value
           << " Z=" << std::dec << p.expect_z
           << " N=" << p.expect_n
           << " H=" << p.expect_h
           << " C=" << p.expect_c
           << "]";
        // clang-format on

        return os;
    }
};

} // namespace boyboy::test::cpu