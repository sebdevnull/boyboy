/**
 * @file utils.cpp
 * @brief File utilities and validations.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/common/files/utils.h"

#include <unistd.h>

#include <expected>
#include <filesystem>

#include "boyboy/common/files/errors.h"

namespace boyboy::common::files {

auto validate_file(const std::filesystem::path& path) -> std::expected<void, FileError>
{
    if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
        return std::unexpected(FileError{FileError::Type::IsDirectory, path});
    }

    return {};
}

auto validate_path(const std::filesystem::path& path) -> std::expected<void, FileError>
{
    if (auto err = validate_file(path); !err) {
        return err;
    }

    if (!std::filesystem::exists(path)) {
        return std::unexpected(FileError{FileError::Type::NotFound, path});
    }

    return {};
}

auto validate_mode(std::ios::openmode mode, std::ios::openmode expected)
    -> std::expected<void, FileError>
{
    if ((mode & expected) != expected) {
        return std::unexpected(FileError{FileError::Type::BadMode});
    }

    return {};
}

auto validate_permissions(const std::filesystem::path& path, std::ios::openmode mode)
    -> std::expected<void, FileError>
{
    if (!std::filesystem::exists(path)) {
        return {};
    }

    auto perms = std::filesystem::status(path).permissions();

    if ((mode & std::ios::in) != 0) {
        perms &=
            (std::filesystem::perms::owner_read | std::filesystem::perms::group_read |
             std::filesystem::perms::others_read);
    }
    else if ((mode & std::ios::out) != 0) {
        perms &=
            (std::filesystem::perms::owner_write | std::filesystem::perms::group_write |
             std::filesystem::perms::others_write);
    }
    else {
        return std::unexpected(FileError{FileError::Type::BadMode, path});
    }

    if (perms == std::filesystem::perms::none) {
        return std::unexpected(FileError{FileError::Type::PermissionDenied, path});
    }

    // Check POSIX access for dirs
    if (std::filesystem::is_directory(path)) {
        if (((mode & std::ios::out) != 0) && ::access(path.c_str(), W_OK | X_OK) != 0) {
            return std::unexpected(FileError{FileError::Type::PermissionDenied, path});
        }
        if (((mode & std::ios::in) != 0) && ::access(path.c_str(), R_OK | X_OK) != 0) {
            return std::unexpected(FileError{FileError::Type::PermissionDenied, path});
        }
        return {};
    }

    // Check POSIX access for files
    int amode = ((mode & std::ios::in) != 0 ? R_OK : 0) | ((mode & std::ios::out) != 0 ? W_OK : 0);
    if (::access(path.c_str(), amode) != 0) {
        return std::unexpected(FileError{FileError::Type::PermissionDenied, path});
    }

    return {};
}

auto ensure_parent_dir(const std::filesystem::path& path) -> std::expected<void, FileError>
{
    auto dir = path.parent_path();
    if (!dir.empty() && !std::filesystem::exists(dir)) {
        std::error_code ec;
        if (!std::filesystem::create_directories(dir, ec)) {
            return std::unexpected(FileError{FileError::Type::PermissionDenied, dir, ec.message()});
        }
    }

    return {};
}

auto ensure_readable(const std::filesystem::path& path, std::ios::openmode mode)
    -> std::expected<void, FileError>
{
    return run_checks(
        [&]() { return validate_path(path); },
        [&]() { return validate_mode(mode, std::ios::in); },
        [&]() { return validate_permissions(path, mode); }
    );
}

auto ensure_writable(const std::filesystem::path& path, std::ios::openmode mode)
    -> std::expected<void, FileError>
{
    return run_checks(
        [&]() { return validate_file(path); },
        [&]() { return validate_mode(mode, std::ios::out); },
        [&]() { return ensure_parent_dir(path); },
        [&]() { return validate_permissions(path, mode); }
    );
}

std::uintmax_t stream_size(std::ifstream& stream)
{
    stream.seekg(0, std::ios::end);
    std::uintmax_t size = stream.tellg();
    stream.seekg(0, std::ios::beg);

    return size;
}

} // namespace boyboy::common::files