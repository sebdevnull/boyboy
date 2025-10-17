/**
 * @file paths.h
 * @brief Filesystem paths for configuration, data, and cache.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstdlib>
#include <filesystem>

namespace boyboy::common::files {

static constexpr std::filesystem::path get_xdg_path(const char* env, const char* fallback);

// Base directories
constexpr auto BoyBoyDir = "boyboy";
const auto ConfigDir = get_xdg_path("XDG_CONFIG_HOME", ".config") / BoyBoyDir;
const auto DataDir = get_xdg_path("XDG_DATA_HOME", ".local/share") / BoyBoyDir;
const auto CacheDir = get_xdg_path("XDG_CACHE_HOME", ".cache") / BoyBoyDir;

static constexpr std::filesystem::path get_xdg_path(const char* env, const char* fallback)
{
    if (const auto* xdg = std::getenv(env)) {
        return xdg;
    }
    if (const auto* home = std::getenv("HOME")) {
        return std::filesystem::path(home) / fallback;
    }

    // if neither is set, point fallback to current dir
    return fallback;
}

} // namespace boyboy::common::files