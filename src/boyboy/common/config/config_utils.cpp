/**
 * @file config_utils.cpp
 * @brief Configuration loading and saving utilities.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/common/config/config_utils.h"

#include <cstdlib>
#include <filesystem>

#include "boyboy/common/config/config.h"
#include "boyboy/common/config/config_loader.h"
#include "boyboy/common/config/config_validator.h"
#include "boyboy/common/files/errors.h"
#include "boyboy/common/files/io.h"
#include "boyboy/common/files/paths.h"
#include "boyboy/common/log/logging.h"

namespace boyboy::common::config {

constexpr std::string_view DefaultConfigFile = "config.toml";
const auto DefaultConfigPath = files::ConfigDir / DefaultConfigFile;

using ConfigLoader = TomlConfigLoader;

Config load_config(const OptionalPath& path, bool normalize)
{
    if (!path.has_value()) {
        log::info(
            "No configuration file provided, using default path: {}", DefaultConfigPath.string()
        );
    }

    auto file_path = path.value_or(DefaultConfigPath);
    auto file = files::input_stream(file_path);
    if (!file) {
        log::warn(
            "Failed to open configuration file, using default config: {}",
            file.error().error_message()
        );
        return Config::default_config();
    }

    ConfigLoader loader{};
    return loader.load(*file, normalize);
}

void save_config(const Config& config, const OptionalPath& path)
{
    namespace fs = std::filesystem;

    auto file_path = path.value_or(DefaultConfigPath);
    if (!fs::exists(file_path)) {
        log::info("Configuration file does not exist, creating new one: {}", file_path.string());
    }
    else {
        log::warn("Overwriting configuration file: {}", file_path.string());
    }

    auto file = files::output_stream(file_path);
    if (!file) {
        log::error("Failed to open configuration file: {}", file.error().error_message());
        return;
    }

    ConfigLoader loader{};
    loader.save(config, *file);
}

void validate_config(Config& config, bool normalize)
{
    auto result = ConfigValidator::validate(config, normalize);
    ConfigValidator::check_result(result);
}

} // namespace boyboy::common::config