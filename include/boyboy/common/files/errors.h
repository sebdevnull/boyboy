/**
 * @file errors.h
 * @brief File errors definitions.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>

namespace boyboy::common::files {

struct FileError {
    enum class Type : uint8_t {
        NotFound,
        PermissionDenied,
        IsDirectory,
        BadMode,
        ReadError,
        WriteError,
        Unknown,
    } type;

    std::filesystem::path path;
    std::optional<std::string> message;

    FileError(Type t, std::filesystem::path p = {}, std::optional<std::string> m = {})
        : type(t), path(std::move(p)), message(std::move(m)) {};

    // Error type to string
    static const char* to_string(Type e)
    {
        switch (e) {
            case Type::NotFound:
                return "File not found";
            case Type::PermissionDenied:
                return "Permission denied";
            case Type::IsDirectory:
                return "Path is a directory";
            case Type::BadMode:
                return "Bad open mode";
            case Type::ReadError:
                return "Read error";
            case Type::WriteError:
                return "Write error";
            default:
                return "Unknown file error";
        }
    }

    // Compose full human-readable error message
    [[nodiscard]] std::string error_message() const
    {
        std::string err_msg = "[" + std::string(to_string(type)) + "]";
        if (!path.empty()) {
            err_msg += " " + path.string();
        }
        if (message) {
            err_msg += ": " + *message;
        }
        return err_msg;
    }
};

} // namespace boyboy::common::files