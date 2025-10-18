/**
 * @file file_fixtures.h
 * @brief File fixtures for unit tests.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <gtest/gtest.h>

#include <cstddef>
#include <filesystem>

class FileTest : public ::testing::Test {
protected:
    static constexpr auto InMode                              = std::ios::in;
    static constexpr auto OutMode                             = std::ios::out;
    inline static const std::filesystem::path BaseDir         = "/tmp";
    inline static const std::filesystem::path ExistentDir     = BaseDir / "existentdir";
    inline static const std::filesystem::path ExistentFile    = ExistentDir / "existentfile.txt";
    inline static const std::filesystem::path NonExistentDir  = BaseDir / "nonexistentdir";
    inline static const std::filesystem::path NonExistentFile = NonExistentDir / "nonexistent.txt";

    void SetUp() override
    {
        mkdir(ExistentDir);
        touch(ExistentFile);
    }

    void TearDown() override
    {
        rm(NonExistentDir, true);
        rm(ExistentDir, true);
    }

    // File helpers
    static void mkdir(const std::filesystem::path& dir);
    static void rmdir(const std::filesystem::path& dir);
    static void rm(const std::filesystem::path& path, bool recursive = false);
    static void touch(const std::filesystem::path& path, size_t size = 0);
};