/**
 * @file app.cpp
 * @brief Main application class for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/app/app.h"

#include <filesystem>

#include "boyboy/common/config/config_utils.h"
#include "boyboy/common/log/logging.h"

namespace boyboy::app {

using namespace boyboy::common;

int App::run(std::string_view rom_path, std::string_view config_path)
{
    log::info("Running BoyBoy emulator...");

    // Load configuration
    load_config(config_path);

    // Apply configuration
    emulator_.apply_config(config_);

    // Load ROM
    try {
        emulator_.load(std::string(rom_path));
    }
    catch (const std::runtime_error& e) {
        log::error("Failed to load ROM: {}", e.what());
        return 1;
    }

    // Run emulator
    emulator_.run();

    log::info("Exiting BoyBoy emulator");

    return 0;
}

void App::load_config(std::string_view config_path)
{
    // Load configuration from file if provided, otherwise use defaults
    std::optional<std::filesystem::path> path;
    if (!config_path.empty()) {
        path = std::filesystem::path(config_path);
    }

    config_ = config::load_config(path);
}

[[nodiscard]] const config::Config& App::get_config() const
{
    return config_;
}

} // namespace boyboy::app