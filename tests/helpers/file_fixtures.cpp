/**
 * @file file_fixtures.cpp
 * @brief File fixtures for unit tests.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "helpers/file_fixtures.h"

#include <filesystem>
#include <fstream>
#include <ios>

void FileTest::mkdir(const std::filesystem::path& dir)
{
    if (!std::filesystem::exists(dir)) {
        std::filesystem::create_directories(dir);
    }
}

void FileTest::rmdir(const std::filesystem::path& dir)
{
    if (std::filesystem::exists(dir) && std::filesystem::is_directory(dir) &&
        std::filesystem::is_empty(dir)) {
        std::filesystem::remove(dir);
    }
}

void FileTest::rm(const std::filesystem::path& path, bool recursive)
{
    if (std::filesystem::exists(path)) {
        if (recursive) {
            std::filesystem::remove_all(path);
        }
        else if (std::filesystem::is_regular_file(path)) {
            std::filesystem::remove(path);
        }
    }
}

void FileTest::touch(const std::filesystem::path& path, size_t size)
{
    if (!std::filesystem::exists(path)) {
        std::ofstream file(path, std::ios::out | std::ios::binary);
        if (size > 0) {
            file.seekp(static_cast<std::streamoff>(size - 1));
            file.write("", 1);
        }
    }
}