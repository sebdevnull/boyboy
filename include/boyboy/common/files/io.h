/**
 * @file io.h
 * @brief Input/Output file operations.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <concepts>
#include <cstddef>
#include <cstdlib>
#include <expected>
#include <filesystem>
#include <format>
#include <fstream>
#include <span>
#include <string>
#include <string_view>
#include <system_error>
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

// ---------- Atomic operations ----------

template <typename T>
concept AtomicWriteTextType = std::convertible_to<T, std::string_view>;

template <typename T>
concept AtomicWriteBinType = requires(T t) {
    { std::span<const std::byte>(t) };
};

template <typename T>
concept AtomicWriteType = AtomicWriteTextType<T> || AtomicWriteBinType<T>;

/**
 * @brief Atomically writes data to a file by first writing to a temporary file and then renaming
 * it.
 *
 * @tparam T Type of data to write (std::string_view for text, std::span<const std::byte> for
 * binary).
 * @param path Path to the target file.
 * @param data Data to write.
 * @param trunc Whether to truncate the file.
 * @return std::expected<void, FileError> Nothing or FileError on error.
 */
template <AtomicWriteType T>
auto atomic_write(const std::filesystem::path& path, T&& data, bool trunc = true)
    -> std::expected<void, FileError>
{
    auto tmp_path = path;
    tmp_path += std::format(".tmp.{}", std::rand());

    // This error should never be thrown
    std::expected<void, FileError> res = std::unexpected(FileError{
        FileError::Type::Unknown,
        path,
        std::format("Bad type T for atomic write: {}", typeid(T).name())
    });

    if constexpr (AtomicWriteTextType<T>) {
        res = write_text(tmp_path, std::forward<T>(data), trunc);
    }
    else if constexpr (AtomicWriteBinType<T>) {
        res = write_binary(tmp_path, std::forward<T>(data), trunc);
    }

    if (res) {
        std::error_code ec;
        std::filesystem::rename(tmp_path, path, ec);
        if (ec) {
            return std::unexpected(FileError{
                FileError::Type::IOError, path, std::format("Rename failed: {}", ec.message())
            });
        }
    }

    return res;
}

} // namespace boyboy::common::files