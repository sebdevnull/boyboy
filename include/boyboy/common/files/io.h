/**
 * @file io.h
 * @brief Input/Output file operations.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstddef>
#include <expected>
#include <filesystem>
#include <fstream>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "boyboy/common/files/errors.h"

namespace boyboy::common::files {

// ---------- Text file operations ----------

/**
 * @brief Reads a text file and returns its contents.
 *
 * @param path Path to the text file to read.
 * @return std::expected<std::string, FileError> File content or FileError on error.
 */
[[nodiscard]] auto read_text(const std::filesystem::path& path)
    -> std::expected<std::string, FileError>;

/**
 * @brief Write data to a text file.
 *
 * @param path Path to the text file to write.
 * @param data Data to write to the file.
 * @param trunc Whether it should truncate the file.
 * @return std::expected<void, FileError> Nothing or FileError on error.
 */
auto write_text(const std::filesystem::path& path, std::string_view data, bool trunc = true)
    -> std::expected<void, FileError>;

// ---------- Binary file operations ----------

/**
 * @brief Reads a binary file and returns its contents.
 *
 * @param path Path to the binary file to read.
 * @return std::expected<std::vector<std::byte>, FileError> File content or FileError on error.
 */
[[nodiscard]] auto read_binary(const std::filesystem::path& path)
    -> std::expected<std::vector<std::byte>, FileError>;

/**
 * @brief Write data to a binary file.
 *
 * @param path Path to the binary file to write.
 * @param data Data to write to the file.
 * @param trunc Whether it should truncate the file.
 * @return std::expected<void, FileError> Nothing or FileError on error.
 */
auto write_binary(
    const std::filesystem::path& path, std::span<const std::byte> data, bool trunc = true
) -> std::expected<void, FileError>;

// ---------- Stream operations ----------

/**
 * @brief Opens an input stream for reading.
 *
 * Mode is always appended with std::ios::in, regardless of the given mode.
 *
 * @param path Path to the file to open.
 * @param mode File open mode.
 * @return std::expected<std::ifstream, FileError> Input stream or FileError on error.
 */
auto input_stream(const std::filesystem::path& path, std::ios::openmode mode = {})
    -> std::expected<std::ifstream, FileError>;

/**
 * @brief Opens an output stream for writing.
 *
 * Mode is always appended with std::ios::out, regardless of the given mode.
 *
 * @param path Path to the file to open.
 * @param mode File open mode.
 * @return std::expected<std::ofstream, FileError> Output stream or FileError on error.
 */
auto output_stream(const std::filesystem::path& path, std::ios::openmode mode = {})
    -> std::expected<std::ofstream, FileError>;

} // namespace boyboy::common::files