/**
 * @file app.cpp
 * @brief Main application class for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/app/app.h"

#include <filesystem>
#include <optional>
#include <sstream>

#include "boyboy/common/config/config.h"
#include "boyboy/common/config/config_utils.h"
#include "boyboy/common/log/logging.h"
#include "boyboy/common/save/save_manager.h"
#include "boyboy/core/cartridge/cartridge.h"
#include "boyboy/core/cartridge/cartridge_loader.h"
#include "boyboy/version.h"

namespace boyboy::app {

using namespace boyboy::common;

namespace {
std::optional<std::filesystem::path> get_fs_path(std::optional<std::string_view> path)
{
    if (path && !(*path).empty()) {
        return std::filesystem::path(*path);
    }
    return std::nullopt;
}
} // namespace

int App::run(std::string_view rom_path)
{
    log::info("Running BoyBoy emulator...");

    // Load ROM
    try {
        emulator_.load(std::string(rom_path));
    }
    catch (const std::runtime_error& e) {
        log::error("Failed to load ROM: {}", e.what());
        return 1;
    }

    // Apply configuration
    emulator_.apply_config(config_);

    // Run emulator
    emulator_.run();

    log::info("Exiting BoyBoy emulator");

    return 0;
}

common::config::Config& App::load_config(std::optional<std::string_view> config_path)
{
    std::optional<std::filesystem::path> path = get_fs_path(config_path);
    config_ = config::load_config(path);
    return config_;
}

void App::save_config( // NOLINT
    const common::config::Config& config,
    std::optional<std::string_view> config_path
)
{
    std::optional<std::filesystem::path> path = get_fs_path(config_path);
    config::save_config(config, path);
}

void App::set_battery_save_path(std::string_view save_path) // NOLINT
{
    save::SaveManager::instance().set_sram_save_path(save_path);
}

std::string App::rom_info(std::string_view rom_path)
{
    auto cart = core::cartridge::CartridgeLoader::load(rom_path);
    return cart->get_header().pretty_string();
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