/**
 * @file toml_config_loader.cpp
 * @brief TOML configuration loader for BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <iostream>
#include <ostream>
#include <string_view>
#include <toml++/toml.hpp>

#include "boyboy/common/config/config.h"
#include "boyboy/common/config/config_loader.h"
#include "boyboy/common/config/config_validator.h"
#include "boyboy/common/log/logging.h"

namespace boyboy::common::config {

namespace {
template <typename Field>
void load_field(
    Field& field, const toml::table& table, std::string_view key, std::string_view section
)
{
    if (auto node = table[key]; node) {
        field = node.value_or(field);
    }
    else {
        log::warn("Missing '{}.{}', using default", section, key);
    }
}

const toml::table& get_section(const toml::table& tbl, std::string_view section)
{
    if (auto sec_node = tbl[section]; sec_node && sec_node.is_table()) {
        return *sec_node.as_table();
    }

    log::warn("Missing [{}] section in config, using defaults", section);
    static const toml::table Empty; // empty table for safe access
    return Empty;
}

} // namespace

[[nodiscard]] Config TomlConfigLoader::load(std::istream& input, bool normalize) const
{
    Config config = Config::default_config();
    toml::table tbl;

    try {
        tbl = toml::parse(input);
    }
    catch (const toml::parse_error& err) {
        log::error("Failed to parse configuration file, using default config: {}", err.what());
        return config;
    }

    // Load Config from TOML table(s)
    auto emulator_tbl = get_section(tbl, ConfigKeys::Emulator::Section);
    load_field(
        config.emulator.speed,
        emulator_tbl,
        ConfigKeys::Emulator::Speed,
        ConfigKeys::Emulator::Section
    );

    auto video_tbl = get_section(tbl, ConfigKeys::Video::Section);
    load_field(config.video.scale, video_tbl, ConfigKeys::Video::Scale, ConfigKeys::Video::Section);
    load_field(config.video.vsync, video_tbl, ConfigKeys::Video::VSync, ConfigKeys::Video::Section);

    auto debug_tbl = get_section(tbl, ConfigKeys::Debug::Section);
    load_field(
        config.debug.log_level, debug_tbl, ConfigKeys::Debug::LogLevel, ConfigKeys::Debug::Section
    );

    // Validate and normalize config
    auto result = ConfigValidator::validate(config, normalize);
    if (!result.valid) {
        for (const auto& error : result.errors) {
            log::error("Config error: {}", error);
        }
        throw std::runtime_error("Configuration validation failed");
    }
    for (const auto& warning : result.warnings) {
        log::warn("Config warning: {}", warning);
    }

    return config;
}

void TomlConfigLoader::save(const Config& config, std::ostream& output) const
{
    // Load Config into TOML table(s)
    auto emulator_tbl = toml::table{
        {ConfigKeys::Emulator::Speed, config.emulator.speed},
    };
    auto video_tbl = toml::table{
        {ConfigKeys::Video::Scale, config.video.scale},
        {ConfigKeys::Video::VSync, config.video.vsync},
    };
    auto debug_tbl = toml::table{
        {ConfigKeys::Debug::LogLevel, std::string(config.debug.log_level)},
    };

    auto config_tbl = toml::table{
        {ConfigKeys::Emulator::Section, emulator_tbl},
        {ConfigKeys::Video::Section, video_tbl},
    };

    output << config_tbl << "\n\n";
    output << toml::table{{ConfigKeys::Debug::Section, debug_tbl}} << "\n";
}

} // namespace boyboy::common::config