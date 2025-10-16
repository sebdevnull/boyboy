/**
 * @file config_utils.cpp
 * @brief Configuration loading and saving utilities.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/common/config/config_utils.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>

#include "boyboy/common/config/config.h"
#include "boyboy/common/config/config_loader.h"
#include "boyboy/common/config/config_validator.h"
#include "boyboy/common/log/logging.h"

namespace boyboy::common::config {

using ConfigLoader = TomlConfigLoader;

Config load_config(const std::optional<std::filesystem::path>& path, bool normalize)
{
    namespace fs = std::filesystem;

    if (!path.has_value()) {
        log::info(
            "No configuration file provided, using default path: {}", default_config_path().string()
        );
    }

    auto file_path = path.value_or(default_config_path());
    if (!fs::exists(file_path)) {
        log::warn("Configuration file not found, using default config: {}", file_path.string());
        return Config::default_config();
    }

    std::ifstream file(file_path);
    if (!file.is_open()) {
        log::error(
            "Failed to open configuration file, using default config: {}", file_path.string()
        );
        return Config::default_config();
    }

    ConfigLoader loader{};
    return loader.load(file, normalize);
}

void save_config(const Config& config, const std::optional<std::filesystem::path>& path)
{
    namespace fs = std::filesystem;

    auto file_path = path.value_or(default_config_path());
    if (auto parent_path = file_path.parent_path(); !parent_path.empty()) {
        // ensure parent dir exists
        fs::create_directories(parent_path);
    }

    if (!fs::exists(file_path)) {
        log::info("Configuration file does not exist, creating new one: {}", file_path.string());
    }
    else {
        log::info("Overwriting configuration file: {}", file_path.string());
    }

    std::ofstream file(file_path, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        log::error("Failed to open configuration file: {}", file_path.string());
        return;
    }

    ConfigLoader loader{};
    loader.save(config, file);
}

void validate_config(Config& config, bool normalize)
{
    auto result = ConfigValidator::validate(config, normalize);
    ConfigValidator::check_result(result);
}

std::filesystem::path default_config_path()
{
    if (auto* xdg = std::getenv("XDG_CONFIG_HOME")) {
        return std::filesystem::path(xdg) / DefaultConfigDir / DefaultConfigFile;
    }
    if (auto* home = std::getenv("HOME")) {
        return std::filesystem::path(home) / ".config" / DefaultConfigDir / DefaultConfigFile;
    }

    // if neither is set point to current dir
    return DefaultConfigFile;
}

} // namespace boyboy::common::config