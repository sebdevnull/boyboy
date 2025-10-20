/**
 * @file config_utils.h
 * @brief Configuration loading and saving utilities.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <filesystem>
#include <optional>

#include "boyboy/common/config/config.h"

namespace boyboy::common::config {

// alias for shorter signature
using OptionalPath = std::optional<std::filesystem::path>;

Config load_config(const OptionalPath& path = {}, bool normalize = true);
void save_config(const Config& config, const OptionalPath& path = {});
void validate_config(Config& config, bool normalize = true);

} // namespace boyboy::common::config