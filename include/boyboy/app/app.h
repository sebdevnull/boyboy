/**
 * @file app.h
 * @brief Main application class for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

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
    int run(std::string_view rom_path, std::string_view config_path = "");

    void load_config(std::string_view config_path = "");
    [[nodiscard]] const common::config::Config& get_config() const;

private:
    common::config::Config config_;
    core::emulator::Emulator emulator_;
};

} // namespace boyboy::app