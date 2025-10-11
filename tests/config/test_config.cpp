/**
 * @file test_config.cpp
 * @brief Unit tests for configuration loading and saving.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "boyboy/common/config/config.h"
#include "boyboy/common/config/config_utils.h"
#include "boyboy/common/config/config_validator.h"

using boyboy::common::config::Config;
using boyboy::common::config::ConfigLimits;
using boyboy::common::config::load_config;
using boyboy::common::config::save_config;

TEST(ConfigTest, DefaultConfig)
{
    Config config = Config::default_config();
    EXPECT_EQ(config.emulator.speed, 1);
    EXPECT_EQ(config.video.scale, 4);
    EXPECT_TRUE(config.video.vsync);
    EXPECT_EQ(config.debug.log_level, "info");
}

TEST(ConfigTest, ValidateConfig)
{
    Config config = Config::default_config();

    // Valid config should pass
    auto result = boyboy::common::config::ConfigValidator::validate(config, false);
    EXPECT_TRUE(result.valid);
    EXPECT_TRUE(result.warnings.empty());
    EXPECT_TRUE(result.errors.empty());

    // Invalid speed
    config.emulator.speed = -1;
    result                = boyboy::common::config::ConfigValidator::validate(config, false);
    EXPECT_FALSE(result.valid);
    EXPECT_FALSE(result.errors.empty());
    EXPECT_TRUE(result.warnings.empty());
    EXPECT_EQ(config.emulator.speed, -1); // unchanged

    // Invalid log level
    config.debug.log_level = "verbose";
    result                 = boyboy::common::config::ConfigValidator::validate(config, false);
    EXPECT_FALSE(result.valid);
    EXPECT_FALSE(result.errors.empty());
    EXPECT_TRUE(result.warnings.empty());
    EXPECT_EQ(config.debug.log_level, "verbose"); // unchanged
}

TEST(ConfigTest, NormalizeConfig)
{
    Config config = Config::default_config();

    // Valid config should pass and remain unchanged
    auto result = boyboy::common::config::ConfigValidator::validate(config, true);
    EXPECT_TRUE(result.valid);
    EXPECT_TRUE(result.warnings.empty());
    EXPECT_TRUE(result.errors.empty());

    // Invalid speed should be normalized to default
    config.emulator.speed = -1;
    result                = boyboy::common::config::ConfigValidator::validate(config, true);
    EXPECT_TRUE(result.valid);
    EXPECT_FALSE(result.warnings.empty());
    EXPECT_TRUE(result.errors.empty());
    EXPECT_EQ(config.emulator.speed, ConfigLimits::Emulator::SpeedRange.default_value);

    // Invalid log level should be normalized to default
    config.debug.log_level = "verbose";
    result                 = boyboy::common::config::ConfigValidator::validate(config, true);
    EXPECT_TRUE(result.valid);
    EXPECT_FALSE(result.warnings.empty());
    EXPECT_TRUE(result.errors.empty());
    EXPECT_EQ(config.debug.log_level, ConfigLimits::Debug::LogLevelOptions.default_value);
}

TEST(ConfigTest, LoadAndSaveConfig)
{
    namespace fs = std::filesystem;

    Config original_config          = Config::default_config();
    original_config.emulator.speed  = 2;
    original_config.video.scale     = 3;
    original_config.video.vsync     = false;
    original_config.debug.log_level = "debug";

    // Save to a temporary file
    fs::path temp_path("temp_config.toml");
    save_config(original_config, temp_path);

    // Load from the temporary file
    Config loaded_config = load_config(temp_path);

    // Verify loaded config matches original
    EXPECT_EQ(loaded_config.emulator.speed, original_config.emulator.speed);
    EXPECT_EQ(loaded_config.video.scale, original_config.video.scale);
    EXPECT_EQ(loaded_config.video.vsync, original_config.video.vsync);
    EXPECT_EQ(loaded_config.debug.log_level, original_config.debug.log_level);

    // Clean up temporary file
    fs::remove(temp_path);
}

TEST(ConfigTest, LoadInvalidConfig)
{
    namespace fs = std::filesystem;

    // Create a temporary invalid config file
    fs::path temp_path("invalid_config.toml");
    {
        std::ofstream ofs(temp_path);
        ofs << "emulator.speed = -5\n";         // Invalid speed
        ofs << "video.scale = -10\n";           // Invalid scale
        ofs << "debug.log_level = 'verbose'\n"; // Invalid log level
    }

    // Attempt to load invalid config with normalization
    Config loaded_config = load_config(temp_path, true);
    EXPECT_EQ(loaded_config.emulator.speed, ConfigLimits::Emulator::SpeedRange.default_value);
    EXPECT_EQ(loaded_config.video.scale, ConfigLimits::Video::ScaleRange.default_value);
    EXPECT_EQ(loaded_config.debug.log_level, ConfigLimits::Debug::LogLevelOptions.default_value);
    EXPECT_TRUE(loaded_config.video.vsync);

    // Attempt to load invalid config without normalization
    EXPECT_THROW(load_config(temp_path, false), std::runtime_error);

    // Clean up temporary file
    fs::remove(temp_path);
}