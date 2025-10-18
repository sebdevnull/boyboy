/**
 * @file test_utils.cpp
 * @brief Unit tests for file utils.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>
#include <unistd.h>

#include <filesystem>

#include "helpers/file_fixtures.h"

// boyboy
#include "boyboy/common/files/errors.h"
#include "boyboy/common/files/utils.h"

using boyboy::common::files::ensure_parent_dir;
using boyboy::common::files::ensure_readable;
using boyboy::common::files::ensure_writable;
using boyboy::common::files::FileError;
using boyboy::common::files::stream_size;
using boyboy::common::files::validate_file;
using boyboy::common::files::validate_mode;
using boyboy::common::files::validate_path;
using boyboy::common::files::validate_permissions;

class FileUtilsTest : public FileTest {};

TEST_F(FileUtilsTest, ValidateFile)
{
    {
        // Existent dir should return error
        auto res = validate_file(ExistentDir);
        EXPECT_FALSE(res.has_value());
        EXPECT_EQ(res.error().type, FileError::Type::IsDirectory);
    }
    {
        // Existent file should NOT return error
        auto res = validate_file(ExistentFile);
        EXPECT_TRUE(res.has_value());
    }
    {
        // Non existent dir should NOT return error
        auto res = validate_file(NonExistentDir);
        EXPECT_TRUE(res.has_value());
    }
    {
        // Non existent file should NOT return error
        auto res = validate_file(NonExistentFile);
        EXPECT_TRUE(res.has_value());
    }
}

TEST_F(FileUtilsTest, ValidatePath)
{
    {
        // Existent dir should return error
        auto res = validate_path(ExistentDir);
        EXPECT_FALSE(res.has_value());
        EXPECT_EQ(res.error().type, FileError::Type::IsDirectory);
    }
    {
        // Existent file should NOT return error
        auto res = validate_path(ExistentFile);
        EXPECT_TRUE(res.has_value());
    }
    {
        // Non existent dir should return error
        auto res = validate_path(NonExistentDir);
        EXPECT_EQ(res.error().type, FileError::Type::NotFound);
    }
    {
        // Non existent file should return error
        auto res = validate_path(NonExistentFile);
        EXPECT_EQ(res.error().type, FileError::Type::NotFound);
    }
}

TEST_F(FileUtilsTest, ValidateMode)
{
    // Arbitrary mode
    auto mode = std::ios::out | std::ios::binary;
    {
        // Same mode should NOT return error
        auto same_mode = mode;

        auto res = validate_mode(mode, same_mode);
        EXPECT_TRUE(res.has_value());
    }
    {
        // Opposite mode should return error
        auto opposite_mode = ~mode;

        auto res = validate_mode(mode, opposite_mode);
        EXPECT_FALSE(res.has_value());
        EXPECT_EQ(res.error().type, FileError::Type::BadMode);
    }
    {
        // Matching mode should NOT return error
        auto expected_mode = std::ios::out;

        auto res = validate_mode(mode, expected_mode);
        EXPECT_TRUE(res.has_value());
    }
    {
        // Only one matching mode should return error
        auto expected_mode = std::ios::out | std::ios::trunc;

        auto res = validate_mode(mode, expected_mode);
        EXPECT_FALSE(res.has_value());
        EXPECT_EQ(res.error().type, FileError::Type::BadMode);
    }
}

TEST_F(FileUtilsTest, ValidatePermissions)
{
    {
        // Owned dir should NOT return read permission error
        auto res = validate_permissions(ExistentDir, InMode);
        EXPECT_TRUE(res.has_value());
    }
    {
        // Owned dir should NOT return write permission error
        auto res = validate_permissions(ExistentDir, OutMode);
        EXPECT_TRUE(res.has_value());
    }
    {
        // Owned file should NOT return read permission error
        auto res = validate_permissions(ExistentFile, InMode);
        EXPECT_TRUE(res.has_value());
    }
    {
        // Owned file should NOT return write permission error
        auto res = validate_permissions(ExistentFile, OutMode);
        EXPECT_TRUE(res.has_value());
    }
    {
        // Non existent dir should NOT return error
        auto res = validate_permissions(ExistentDir, InMode);
        EXPECT_TRUE(res.has_value());
    }
    {
        // Non existent file should NOT return error
        auto res = validate_permissions(ExistentDir, InMode);
        EXPECT_TRUE(res.has_value());
    }
    {
        // Not owned dir should return error
        std::filesystem::path dir("/usr/bin");
        auto res = validate_permissions(dir, OutMode);
        EXPECT_FALSE(res.has_value());
        EXPECT_EQ(res.error().type, FileError::Type::PermissionDenied);
    }
    {
        // Not owned file should return read error
        std::filesystem::path dir("/etc/shadow");
        auto res = validate_permissions(dir, InMode);
        EXPECT_FALSE(res.has_value());
        EXPECT_EQ(res.error().type, FileError::Type::PermissionDenied);
    }
    {
        // Not owned file should return write error
        std::filesystem::path dir("/bin/bash");
        auto res = validate_permissions(dir, OutMode);
        EXPECT_FALSE(res.has_value());
        EXPECT_EQ(res.error().type, FileError::Type::PermissionDenied);
    }
}

TEST_F(FileUtilsTest, EnsureParentDir)
{
    {
        // Non existent parent dir should be created
        auto parent_dir = NonExistentFile.parent_path();
        EXPECT_FALSE(std::filesystem::exists(parent_dir));

        auto res = ensure_parent_dir(NonExistentFile);
        EXPECT_TRUE(res.has_value());
        EXPECT_TRUE(std::filesystem::exists(parent_dir));

        // Cleanup
        rmdir(parent_dir);
    }
    {
        // Existent parent dir should do nothing
        auto parent_dir = ExistentFile.parent_path();
        EXPECT_TRUE(std::filesystem::exists(parent_dir));

        auto res = ensure_parent_dir(ExistentFile);
        EXPECT_TRUE(res.has_value());
        EXPECT_TRUE(std::filesystem::exists(parent_dir));
    }
    {
        // Non existent parent dirs should be created recursively
        auto long_path  = NonExistentDir / "a" / "b" / "file.txt";
        auto parent_dir = long_path.parent_path();
        EXPECT_FALSE(std::filesystem::exists(parent_dir));

        auto res = ensure_parent_dir(long_path);
        EXPECT_TRUE(res.has_value());
        EXPECT_TRUE(std::filesystem::exists(parent_dir));

        // Cleanup
        rm(NonExistentDir, true);
    }
}

TEST_F(FileUtilsTest, EnsureReadable)
{
    {
        // Existent file should be readable
        auto res = ensure_readable(ExistentFile, InMode);
        EXPECT_TRUE(res.has_value());
    }
    {
        // Non existent file should NOT be readable
        auto res = ensure_readable(NonExistentFile, InMode);
        EXPECT_FALSE(res.has_value());
        EXPECT_EQ(res.error().type, FileError::Type::NotFound);
    }
    {
        // Existent file with bad mode should NOT be readable
        auto res = ensure_readable(ExistentFile, OutMode);
        EXPECT_FALSE(res.has_value());
        EXPECT_EQ(res.error().type, FileError::Type::BadMode);
    }
    {
        // Existent dir should NOT be readable
        auto res = ensure_readable(ExistentDir, InMode);
        EXPECT_FALSE(res.has_value());
        EXPECT_EQ(res.error().type, FileError::Type::IsDirectory);
    }
}

TEST_F(FileUtilsTest, EnsureWritable)
{
    {
        // Existent file should be writable
        auto res = ensure_writable(ExistentFile, OutMode);
        EXPECT_TRUE(res.has_value());
    }
    {
        // Non existent file should be readable and parent dirs created
        auto parent_dir = NonExistentFile.parent_path();
        EXPECT_FALSE(std::filesystem::exists(parent_dir));

        auto res = ensure_writable(NonExistentFile, OutMode);
        EXPECT_TRUE(res.has_value());
        EXPECT_TRUE(std::filesystem::exists(parent_dir));

        // Cleanup
        rm(parent_dir, true);
    }
    {
        // Existent file with bad mode should NOT be writable
        auto res = ensure_writable(ExistentFile, InMode);
        EXPECT_FALSE(res.has_value());
        EXPECT_EQ(res.error().type, FileError::Type::BadMode);
    }
    {
        // Existent dir should NOT be writable
        auto res = ensure_writable(ExistentDir, OutMode);
        EXPECT_FALSE(res.has_value());
        EXPECT_EQ(res.error().type, FileError::Type::IsDirectory);
    }
}

TEST_F(FileUtilsTest, StreamSize)
{
    {
        std::ifstream stream(ExistentFile);
        auto size = stream_size(stream);
        EXPECT_EQ(size, 0);
    }
    {
        auto file_path = NonExistentFile;
        auto res       = ensure_parent_dir(file_path);
        EXPECT_TRUE(res.has_value());

        std::uintmax_t expected_size = 256;
        touch(file_path, expected_size);
        EXPECT_TRUE(std::filesystem::exists(file_path));
        EXPECT_EQ(std::filesystem::file_size(file_path), expected_size);

        std::ifstream stream(file_path);
        auto size = stream_size(stream);
        EXPECT_EQ(size, expected_size);
    }
}