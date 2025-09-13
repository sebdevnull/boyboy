/**
 * @file test_paths.h
 * @brief Utility functions for handling file paths in tests.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <filesystem>
#include <string>

namespace boyboy::test::common {

// Absolute path to a file relative to the project root.
inline std::string project_file(const std::string& relative)
{
    static const std::filesystem::path ProjRoot = std::filesystem::current_path();
    return (ProjRoot / relative).string();
}

// Absolute path to a file relative to the test source file.
inline std::string local_file(const std::string& relative, const std::string& this_file)
{
    return (std::filesystem::path(this_file).parent_path() / relative).string();
}

} // namespace boyboy::test::common