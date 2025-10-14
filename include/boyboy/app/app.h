/**
 * @file app.h
 * @brief Main application class for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <optional>
#include <string_view>

#include "boyboy/common/config/config.h"
#include "boyboy/core/emulator/emulator.h"

namespace boyboy::common::config {
struct Config;
}

namespace boyboy::core::emulator {
class Emulator;
}

namespace boyboy::app {

class App {
public:
    // Main application operations
    int run(std::string_view rom_path);

    // Configuration management
    common::config::Config& load_config(std::optional<std::string_view> config_path = std::nullopt);
    [[nodiscard]] const common::config::Config& get_config() const { return config_; }
    [[nodiscard]] common::config::Config& get_config() { return config_; }

    // ROM information
    [[nodiscard]] static std::string rom_info(std::string_view rom_path);

    // Version information
    [[nodiscard]] static std::string version();
    [[nodiscard]] static std::string build_info();

private:
    common::config::Config config_ = common::config::Config::default_config();
    core::emulator::Emulator emulator_;
};

} // namespace boyboy::app