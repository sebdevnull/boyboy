/**
 * @file common.h
 * @brief Common test utilities and definitions.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <gtest/gtest.h>

#include <algorithm>
#include <concepts>
#include <string>

namespace boyboy::test::common {

template <typename T>
concept HasName = requires(T t) {
    { t.name } -> std::convertible_to<std::string>;
};

// Parameter name generator (removes non-alphanumeric characters from name)
template <HasName ParamType>
std::string param_name(const ::testing::TestParamInfo<ParamType>& info)
{
    std::string name = info.param.name;
    name.erase(
        std::remove_if(
            name.begin(),
            name.end(),
            [](char c) { return !std::isalnum(static_cast<unsigned char>(c)); }
        ),
        name.end()
    );
    return name;
}

} // namespace boyboy::test::common