/**
 * @file config.h
 * @brief Configuration structure for BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

// TODO: improve config system
// Right now we have to touch 1000 places to add/modify a single config key

#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

#include "boyboy/common/config/config_limits.h"

namespace boyboy::common::config {

template <typename T>
concept ValidConfigType = std::same_as<T, int> || std::same_as<T, bool> ||
                          std::same_as<T, std::string>;

struct ConfigKeys {
    struct Emulator {
        static constexpr std::string_view Section = "emulator";
        static constexpr std::string_view Speed = "speed";
        static constexpr std::string_view TickMode = "tick_mode";
        static constexpr std::string_view FetchExecOverlap = "cpu_overlap";
    };
    struct Video {
        static constexpr std::string_view Section = "video";
        static constexpr std::string_view Scale = "scale";
        static constexpr std::string_view VSync = "vsync";
    };
    struct Saves {
        static constexpr std::string_view Section = "saves";
        static constexpr std::string_view Autosave = "autosave";
        static constexpr std::string_view SaveInterval = "save_interval";
    };
    struct Debug {
        static constexpr std::string_view Section = "debug";
        static constexpr std::string_view LogLevel = "log_level";
    };

    // Full keys for easy access
    inline static const std::string EmulatorSpeed = std::string(Emulator::Section) + "." +
                                                    std::string(Emulator::Speed);
    inline static const std::string EmulatorTickMode = std::string(Emulator::Section) + "." +
                                                       std::string(Emulator::TickMode);
    inline static const std::string EmulatorFEOverlap = std::string(Emulator::Section) + "." +
                                                        std::string(Emulator::FetchExecOverlap);
    inline static const std::string VideoScale = std::string(Video::Section) + "." +
                                                 std::string(Video::Scale);
    inline static const std::string VideoVSync = std::string(Video::Section) + "." +
                                                 std::string(Video::VSync);
    inline static const std::string SavesAutoSave = std::string(Saves::Section) + "." +
                                                    std::string(Saves::Autosave);
    inline static const std::string SavesSaveInterval = std::string(Saves::Section) + "." +
                                                        std::string(Saves::SaveInterval);
    inline static const std::string DebugLogLevel = std::string(Debug::Section) + "." +
                                                    std::string(Debug::LogLevel);

    inline static const std::vector<std::string> KeyList = {
        EmulatorSpeed,
        EmulatorTickMode,
        EmulatorFEOverlap,
        VideoScale,
        VideoVSync,
        SavesAutoSave,
        SavesSaveInterval,
        DebugLogLevel,
    };
};

struct ConfigMeta {
    enum class Type : uint8_t { Int, Bool, String, None };

    [[nodiscard]] static ConfigMeta::Type get_type(std::string_view key)
    {
        auto it = ConfigMetadata.find(std::string(key));
        return (it != ConfigMetadata.end()) ? it->second : Type::None;
    }

private:
    inline static const std::unordered_map<std::string, ConfigMeta::Type> ConfigMetadata = {
        {ConfigKeys::EmulatorSpeed, Type::Int},
        {ConfigKeys::EmulatorTickMode, Type::String},
        {ConfigKeys::EmulatorFEOverlap, Type::Bool},
        {ConfigKeys::VideoScale, Type::Int},
        {ConfigKeys::VideoVSync, Type::Bool},
        {ConfigKeys::SavesAutoSave, Type::Bool},
        {ConfigKeys::SavesSaveInterval, Type::Int},
        {ConfigKeys::DebugLogLevel, Type::String},
    };
};

struct Config {

    struct Emulator {
        int speed = ConfigLimits::Emulator::SpeedRange.default_value;
        std::string tick_mode = std::string(ConfigLimits::Emulator::TickModeOptions.default_value);
        bool fe_overlap = false;
    } emulator; // NOLINT

    struct Video {
        int scale = ConfigLimits::Video::ScaleRange.default_value;
        bool vsync = true;
    } video; // NOLINT

    struct Saves {
        bool autosave = true;
        int save_interval = ConfigLimits::Saves::SaveInterval.default_value;
    } saves; // NOLINT

    struct Debug {
        std::string log_level = std::string(ConfigLimits::Debug::LogLevelOptions.default_value);
    } debug; // NOLINT

    static Config default_config() { return Config{}; }

    template <ValidConfigType T>
    [[nodiscard]] T& get(std::string_view key)
    {
        std::string key_str(key);
        auto it = config_map_.find(key_str);
        if (it == config_map_.end()) {
            throw std::invalid_argument("Invalid config key: " + key_str);
        }

        bool type_mismatch = true;
        if constexpr (std::same_as<T, int>) {
            type_mismatch = ConfigMeta::get_type(key) != ConfigMeta::Type::Int;
        }
        else if constexpr (std::same_as<T, bool>) {
            type_mismatch = ConfigMeta::get_type(key) != ConfigMeta::Type::Bool;
        }
        else if constexpr (std::same_as<T, std::string>) {
            type_mismatch = ConfigMeta::get_type(key) != ConfigMeta::Type::String;
        }

        if (type_mismatch) {
            throw std::invalid_argument("Type mismatch for config key: " + key_str);
        }

        return it->second.get<T>(*this);
    }

    template <ValidConfigType T>
    void set(std::string_view key, const T& value)
    {
        get<T>(key) = value;
    }

    void set_string(std::string_view key, std::string_view value)
    {
        auto type = ConfigMeta::get_type(key);
        switch (type) {
            case ConfigMeta::Type::Int:
                set<int>(key, parse_int(value));
                break;
            case ConfigMeta::Type::Bool:
                set<bool>(key, parse_bool(value));
                break;
            case ConfigMeta::Type::String:
                set<std::string>(key, std::string(value));
                break;
            default:
                throw std::invalid_argument("Invalid config key: " + std::string(key));
        }
    }

    [[nodiscard]] std::string key_value_str(std::string_view key)
    {
        std::ostringstream oss;
        oss << key << ": ";
        auto type = ConfigMeta::get_type(key);
        switch (type) {
            case ConfigMeta::Type::Int:
                oss << get<int>(key);
                break;
            case ConfigMeta::Type::Bool:
                oss << get<bool>(key);
                break;
            case ConfigMeta::Type::String:
                oss << get<std::string>(key);
                break;
            default:
                oss << "Invalid key";
                break;
        }
        return oss.str();
    }

private:
    struct ConfigAccessor {
        std::function<void*(Config&)> getter;
        template <ValidConfigType T>
        T& get(Config& config) const
        {
            return *static_cast<T*>(getter(config));
        }
    };

    std::unordered_map<std::string, ConfigAccessor> config_map_{
        {ConfigKeys::EmulatorSpeed, ConfigAccessor{[](Config& c) {
             return &c.emulator.speed;
         }}},
        {ConfigKeys::EmulatorTickMode, ConfigAccessor{[](Config& c) {
             return &c.emulator.tick_mode;
         }}},
        {ConfigKeys::EmulatorFEOverlap, ConfigAccessor{[](Config& c) {
             return &c.emulator.fe_overlap;
         }}},
        {ConfigKeys::VideoScale, ConfigAccessor{[](Config& c) {
             return &c.video.scale;
         }}},
        {ConfigKeys::VideoVSync, ConfigAccessor{[](Config& c) {
             return &c.video.vsync;
         }}},
        {ConfigKeys::SavesAutoSave, ConfigAccessor{[](Config& c) {
             return &c.saves.autosave;
         }}},
        {ConfigKeys::SavesSaveInterval, ConfigAccessor{[](Config& c) {
             return &c.saves.save_interval;
         }}},
        {ConfigKeys::DebugLogLevel, ConfigAccessor{[](Config& c) {
             return &c.debug.log_level;
         }}},
    };

    static bool parse_bool(std::string_view value)
    {
        if (value == "true" || value == "1") {
            return true;
        }
        if (value == "false" || value == "0") {
            return false;
        }

        throw std::invalid_argument("Invalid bool value: " + std::string(value));
    }

    static int parse_int(std::string_view value)
    {
        size_t pos = 0;
        int int_val = std::stoi(std::string(value), &pos);
        if (pos != value.size()) {
            throw std::invalid_argument("Invalid integer value: " + std::string(value));
        }
        return int_val;
    }
};

} // namespace boyboy::common::config