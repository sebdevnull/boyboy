/**
 * @file config.h
 * @brief Configuration structure for BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <string>
#include <string_view>

#include "boyboy/common/config/config_limits.h"

namespace boyboy::common::config {

struct ConfigKeys {
    struct Emulator {
        static constexpr std::string_view Section = "emulator";
        static constexpr std::string_view Speed = "speed";
    };
    struct Video {
        static constexpr std::string_view Section = "video";
        static constexpr std::string_view Scale = "scale";
        static constexpr std::string_view VSync = "vsync";
    };
    struct Debug {
        static constexpr std::string_view Section = "debug";
        static constexpr std::string_view LogLevel = "log_level";
    };
};

struct Config {

    struct Emulator {
        int speed = ConfigLimits::Emulator::SpeedRange.default_value;
        // std::string save_path = "saves/"; // directory for save files
        // bool auto_save = true;            // auto-save on exit
        // bool auto_load = true;            // auto-load on start
        // int auto_save_interval = 300;     // auto-save interval in seconds, 0 = disabled
        // bool skip_boot = false;           // skip boot ROM
        // std::string boot_rom_path;        // path to custom boot ROM, empty = default
    } emulator;

    // struct Input {
    //     struct Keymap {
    //         // Game Boy buttons
    //         std::string up = "up";
    //         std::string down = "down";
    //         std::string left = "left";
    //         std::string right = "right";
    //         std::string a = "z";
    //         std::string b = "x";
    //         std::string start = "enter";
    //         std::string select = "backspace";

    //         // Emulator controls
    //         std::string pause = "space";
    //         std::string reset = "r";
    //         std::string fullscreen = "f11";
    //         std::string screenshot = "f12";
    //         std::string turbo = "lshift";
    //         std::string increase_speed = "pageup";
    //         std::string decrease_speed = "pagedown";
    //         std::string quick_save = "f5";
    //         std::string quick_load = "f8";
    //     } keymap;
    // } input;

    struct Video {
        int scale = ConfigLimits::Video::ScaleRange.default_value;
        bool vsync = true;
        // bool fullscreen = false;
        // int frame_limit = 60; // max FPS: 0 = uncapped

    } video;

    struct Debug {
        std::string log_level = std::string(ConfigLimits::Debug::LogLevelOptions.default_value);
        // bool profiling = false;         // enable performance profiling
        // bool trace_cpu = false;         // enable CPU instruction tracing
    } debug;

    static Config default_config() { return Config{}; }
};

} // namespace boyboy::common::config