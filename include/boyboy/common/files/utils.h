/**
 * @file utils.h
 * @brief File utilities and validations.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstdint>
#include <expected>
#include <filesystem>
#include <fstream>

#include "boyboy/common/files/errors.h"

namespace boyboy::common::files {

// Run multiple validations at once
template <typename... CheckFns>
std::expected<void, FileError> run_checks(CheckFns... checks)
{
    for (auto& check : {checks()...}) {
        if (!check) {
            return check;
        }
    }
    return {};
}

// ---------- Validation methods ----------

/**
 * @brief Validates a file path.
 *
 * Path is considered valid if it contains or could contain a regular file. I.e. if it's not an
 * existent directory.
 *
 * @param path Path to validate.
 * @return std::expected<void, FileError> Nothing or FileError on error.
 */
auto validate_file(const std::filesystem::path& path) -> std::expected<void, FileError>;

/**
 * @brief Validates a path.
 *
 * Path is considered valid if it passes validate_file and also is an existent path. I.e. file
 * exists and is not a directory.
 *
 * @param path Path to validate.
 * @return std::expected<void, FileError> Nothing or FileError on error.
 */
auto validate_path(const std::filesystem::path& path) -> std::expected<void, FileError>;

/**
 * @brief Validates a file open mode.
 *
 * Mode is considered valid if it contains all expected modes.
 *
 * @param mode Mode(s) to validate.
 * @param expected Expected mode(s) in mode.
 * @return std::expected<void, FileError> Nothing or FileError on error.
 */
auto validate_mode(std::ios::openmode mode, std::ios::openmode expected)
    -> std::expected<void, FileError>;

/**
 * @brief Validates file permissions with the corresponding open mode.
 *
 * Checks if an input open mode has read permissions or if an output open mode has write
 * permissions.
 * Additionally, it checks that the user running the app has permissions to that path.
 *
 * Notes:
 * - Other mode other than std::ios::in or std::ios::out are considered FileError:Type::BadMode
 * - A non existent path will return as valid. It's up to the user to check the permissions of the
 * parent path where the is to be created.
 *
 * @param path Path to validate.
 * @param mode Mode to open the file (std::ios::in, std::ios::out).
 * @return std::expected<void, FileError> Nothing or FileError on error.
 */
auto validate_permissions(const std::filesystem::path& path, std::ios::openmode mode)
    -> std::expected<void, FileError>;

/**
 * @brief Ensures that the parent directory exists.
 *
 * Checks if the parent dir exists. If it does not, it creates recursively all parent directories.
 * If parent dir already exists it does nothing.
 *
 * @param path Path which parent directory needs to be ensured.
 * @return std::expected<void, FileError> Nothing or FileError on error.
 */
auto ensure_parent_dir(const std::filesystem::path& path) -> std::expected<void, FileError>;

/**
 * @brief Ensures a path is readable in the given mode.
 *
 * The path must correspond to an existent file, with read permissions for the running user.
 * Mode must include std::ios::in.
 *
 * It performs the following checks:
 * - validate_path
 * - validate_mode
 * - validate_permissions
 *
 * @param path Path to ensure is readable.
 * @param mode Mode to open the path.
 * @return std::expected<void, FileError> Nothing or FileError on error.
 */
auto ensure_readable(const std::filesystem::path& path, std::ios::openmode mode)
    -> std::expected<void, FileError>;

/**
 * @brief Ensures a path is writable in the given mode.
 *
 * The path must be a writable or creatable file. If the parent directory of the file does not
 * exist, it is created recursively.
 * Mode must be std::ios::out.
 *
 * It performs the following checks:
 * - validate_file
 * - validate_mode
 * - ensure_parent_dir
 * - validate_permissions
 *
 * @param path Path to ensure is writable.
 * @param mode Mode to open the path.
 * @return std::expected<void, FileError> Nothing or FileError on error.
 */
auto ensure_writable(const std::filesystem::path& path, std::ios::openmode mode)
    -> std::expected<void, FileError>;

// ---------- File utility functions ----------

/**
 * @brief Gets the size of a file stream.
 *
 * @param stream Input file stream.
 * @return std::uintmax_t Size of the stream.
 */
std::uintmax_t stream_size(std::ifstream& stream);

} // namespace boyboy::common::files