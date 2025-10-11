/**
 * @file config_utils.h
 * @brief Configuration loading and saving utilities.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <filesystem>
#include <optional>
#include <string_view>

#include "boyboy/common/config/config.h"

namespace boyboy::common::config {

inline constexpr std::string_view DefaultConfigDir = "boyboy";
inline constexpr std::string_view DefaultConfigFile = "config.toml";

Config
load_config(const std::optional<std::filesystem::path>& path = std::nullopt, bool normalize = true);
void save_config(
    const Config& config, const std::optional<std::filesystem::path>& path = std::nullopt
);

std::filesystem::path default_config_path();

} // namespace boyboy::common::config