/**
 * @file io.cpp
 * @brief Input/Output file operations.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/common/files/io.h"

#include <expected>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iterator>
#include <string>

#include "boyboy/common/files/errors.h"
#include "boyboy/common/files/utils.h"

namespace boyboy::common::files {

[[nodiscard]] auto read_text(const std::filesystem::path& path)
    -> std::expected<std::string, FileError>
{
    auto file = input_stream(path);
    if (!file) {
        return std::unexpected(file.error());
    }

    std::string buffer(
        std::istreambuf_iterator<char>(file.value()), std::istreambuf_iterator<char>()
    );

    if (file->fail()) {
        return std::unexpected(FileError{FileError::Type::ReadError, path});
    }

    return buffer;
}

auto write_text(const std::filesystem::path& path, std::string_view data, bool trunc)
    -> std::expected<void, FileError>
{
    std::ios::openmode mode{};
    if (trunc) {
        mode |= std::ios::trunc;
    }

    auto file = output_stream(path, mode);
    if (!file) {
        return std::unexpected(file.error());
    }

    file->write(data.data(), static_cast<std::streamsize>(data.size()));
    if (file->fail()) {
        return std::unexpected(FileError{FileError::Type::WriteError, path});
    }

    file->flush();

    return {};
}

[[nodiscard]] auto read_binary(const std::filesystem::path& path)
    -> std::expected<std::vector<std::byte>, FileError>
{
    auto file = input_stream(path, std::ios::binary);
    if (!file) {
        return std::unexpected(file.error());
    }

    auto size = stream_size(file.value());
    std::vector<std::byte> buffer(size);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    file->read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(size));

    if (file->fail()) {
        return std::unexpected(FileError{FileError::Type::ReadError, path});
    }

    return buffer;
}

auto write_binary(const std::filesystem::path& path, std::span<const std::byte> data, bool trunc)
    -> std::expected<void, FileError>
{
    std::ios::openmode mode{std::ios::binary};
    if (trunc) {
        mode |= std::ios::trunc;
    }

    auto file = output_stream(path, mode);
    if (!file) {
        return std::unexpected(file.error());
    }

    file->write(
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        reinterpret_cast<const char*>(data.data()),
        static_cast<std::streamsize>(data.size())
    );
    if (file->fail()) {
        return std::unexpected(FileError{FileError::Type::WriteError, path});
    }

    file->flush();

    return {};
}

auto input_stream(const std::filesystem::path& path, std::ios::openmode mode)
    -> std::expected<std::ifstream, FileError>
{
    mode |= std::ios::in;

    if (auto err = ensure_readable(path, mode); !err) {
        return std::unexpected(err.error());
    }

    std::ifstream file(path, mode);
    if (!file.is_open()) {
        return std::unexpected(
            FileError{FileError::Type::Unknown, path, "Failed to open file for reading"}
        );
    }

    return file;
}

auto output_stream(const std::filesystem::path& path, std::ios::openmode mode)
    -> std::expected<std::ofstream, FileError>
{
    mode |= std::ios::out;

    if (auto err = ensure_writable(path, mode); !err) {
        return std::unexpected(err.error());
    }

    std::ofstream file(path, mode);
    if (!file.is_open()) {
        return std::unexpected(
            FileError{FileError::Type::Unknown, path, "Failed to open file for writing"}
        );
    }

    return file;
}

} // namespace boyboy::common::files