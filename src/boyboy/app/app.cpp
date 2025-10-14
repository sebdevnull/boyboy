/**
 * @file app.cpp
 * @brief Main application class for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/app/app.h"

#include <filesystem>
#include <sstream>

#include "boyboy/common/config/config.h"
#include "boyboy/common/config/config_utils.h"
#include "boyboy/common/log/logging.h"
#include "boyboy/core/cartridge/cartridge.h"
#include "boyboy/core/cartridge/cartridge_loader.h"
#include "boyboy/version.h"

namespace boyboy::app {

using namespace boyboy::common;

int App::run(std::string_view rom_path)
{
    log::info("Running BoyBoy emulator...");

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

common::config::Config& App::load_config(std::optional<std::string_view> config_path)
{
    // Load configuration from file if provided, otherwise use defaults
    std::optional<std::filesystem::path> path = std::nullopt;
    if (config_path && !(*config_path).empty()) {
        path = std::filesystem::path(*config_path);
    }

    config_ = config::load_config(path);
    return config_;
}

std::string App::rom_info(std::string_view rom_path)
{
    auto cart = core::cartridge::CartridgeLoader::load(rom_path);
    return cart.get_header().pretty_string();
}

std::string App::version()
{
    std::ostringstream oss;
    oss << "BoyBoy version " << version::VersionStr << " (commit " << version::GitCommit << ")\n"
        << version::LicenseShort << "\n"
        << "Build type: " << version::BuildType << "\n"
        << "Compiler: " << version::CompilerID << " " << version::CompilerVersion;

    return oss.str();
}

std::string App::build_info()
{
    std::ostringstream oss;
    oss << "BoyBoy version " << version::VersionStr << " (commit " << version::GitCommit
        << ", branch " << version::GitBranch << ", tag " << version::GitTag << ")\n"
        << version::LicenseShort << "\n"
        << "Build type:     " << version::BuildType << "\n"
        << "Compiler:       " << version::CompilerID << " " << version::CompilerVersion << "\n"
        << "System:         " << version::SystemName << " (" << version::SystemProcessor << ")\n"
        << "CMake version:  " << version::CmakeVersion << "\n"
        << "Build timestamp:" << version::BuildTimestamp;

    return oss.str();
}

} // namespace boyboy::app