/**
 * @file config_limits.h
 * @brief Configuration limits and options for BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <array>
#include <concepts>
#include <ranges>
#include <span>
#include <string_view>

namespace boyboy::common::config {

struct ConfigLimits {
    template <typename T>
        requires std::totally_ordered<T> // Ensures T supports comparison operators
    struct Range {
        T min;
        T max;
        T default_value;

        [[nodiscard]] bool in_range(T value) const { return value >= min && value <= max; }
    };

    template <typename T>
    struct Options {
        std::span<const T> options;
        T default_value;

        [[nodiscard]] bool valid(const T& value) const
        {
            return std::ranges::find(options.begin(), options.end(), value) != options.end();
        }
        [[nodiscard]] std::string option_list() const
        {
            auto joined = options | std::views::join_with(',');
            return {joined.begin(), joined.end()};
        }
    };

    struct Emulator {
        static constexpr Range<int> SpeedRange = {.min = 0, .max = 10, .default_value = 1};

        // Tick modes
        static constexpr std::string_view FastMode = "fast";
        static constexpr std::string_view NormalMode = "normal";
        static constexpr std::string_view PrecisionMode = "precision";
        static constexpr std::array<std::string_view, 3> TickModes = {
            FastMode, NormalMode, PrecisionMode
        };
        static constexpr Options<std::string_view> TickModeOptions = {
            .options = TickModes, .default_value = NormalMode
        };
    };

    struct Video {
        static constexpr Range<int> ScaleRange = {.min = 1, .max = 10, .default_value = 2};
    };

    struct Saves {
        static constexpr Range<int> SaveInterval = {
            .min = 0, .max = 3'600'000, .default_value = 5000
        };
    };

    struct Debug {
        static constexpr std::array<std::string_view, 7> LogLevels = {
            "trace", "debug", "info", "warn", "error", "critical", "off"
        };

        static constexpr Options<std::string_view> LogLevelOptions = {
            .options = LogLevels, .default_value = "info"
        };
    };
};

} // namespace boyboy::common::config