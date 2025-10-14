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
    int run(std::string_view rom_path);

    common::config::Config& load_config(std::optional<std::string_view> config_path = std::nullopt);
    [[nodiscard]] const common::config::Config& get_config() const { return config_; }
    [[nodiscard]] common::config::Config& get_config() { return config_; }

private:
    common::config::Config config_ = common::config::Config::default_config();
    core::emulator::Emulator emulator_;
};

} // namespace boyboy::app