/**
 * @file test_io.cpp
 * @brief Unit test for file I/O operations
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <filesystem>
#include <ios>
#include <string>

#include "helpers/file_fixtures.h"

// boyboy
#include "boyboy/common/files/errors.h"
#include "boyboy/common/files/io.h"

using boyboy::common::files::FileError;
using boyboy::common::files::input_stream;
using boyboy::common::files::output_stream;
using boyboy::common::files::read_binary;
using boyboy::common::files::read_text;
using boyboy::common::files::write_binary;
using boyboy::common::files::write_text;

class FileIoTest : public FileTest {
protected:
    inline static const std::string SampleText = "This is a test";
    static constexpr auto SampleBinary         = std::to_array<std::byte>({
        std::byte{0x00},
        std::byte{0x01},
        std::byte{0x02},
        std::byte{0x03},
        std::byte{0x04},
        std::byte{0x05},
        std::byte{0x06},
        std::byte{0x07},
        std::byte{0x08},
        std::byte{0x09},
    });
};

TEST_F(FileIoTest, InputStream)
{
    {
        // Existent file should open correctly
        auto stream = input_stream(ExistentFile);
        EXPECT_TRUE(stream.has_value());
        EXPECT_TRUE(stream->is_open());
    }
    {
        // Non existent file should give error
        auto stream = input_stream(NonExistentFile);
        EXPECT_FALSE(stream.has_value());
        EXPECT_EQ(stream.error().type, FileError::Type::NotFound);
    }
    {
        // Dir should give error
        auto stream = input_stream(ExistentDir);
        EXPECT_FALSE(stream.has_value());
        EXPECT_EQ(stream.error().type, FileError::Type::IsDirectory);
    }
}

TEST_F(FileIoTest, OutputStream)
{
    {
        // Existent file should open correctly
        auto stream = output_stream(ExistentFile);
        EXPECT_TRUE(stream.has_value());
        EXPECT_TRUE(stream->is_open());
    }
    {
        // Non existent file should also open correctly and create parent dirs
        auto parent_dir = NonExistentFile.parent_path();
        EXPECT_FALSE(std::filesystem::exists(parent_dir));

        auto stream = output_stream(NonExistentFile);
        EXPECT_TRUE(stream.has_value());
        EXPECT_TRUE(stream->is_open());
        EXPECT_TRUE(std::filesystem::exists(parent_dir));

        stream->close();
        EXPECT_TRUE(std::filesystem::exists(NonExistentFile));

        // Cleanup
        rm(parent_dir, true);
    }
    {
        // Dir should give error
        auto stream = output_stream(ExistentDir);
        EXPECT_FALSE(stream.has_value());
        EXPECT_EQ(stream.error().type, FileError::Type::IsDirectory);
    }
}

TEST_F(FileIoTest, ReadText)
{
    {
        // Read should work with existing text file
        auto file_path = ExistentFile;
        {
            // Create sample file for testing
            auto file = output_stream(file_path, std::ios::trunc);
            EXPECT_TRUE(file.has_value());

            file->write(SampleText.data(), static_cast<std::streamsize>(SampleText.size()));
        }

        auto read_content = read_text(file_path);
        EXPECT_TRUE(read_content.has_value());
        EXPECT_EQ(*read_content, SampleText);
    }
}

TEST_F(FileIoTest, WriteText)
{
    {
        // Write should work with existing text file
        auto file_path = ExistentFile;
        {
            auto res = write_text(file_path, SampleText);
            EXPECT_TRUE(res.has_value());
        }

        // Read back the file content
        auto read_content = read_text(file_path);
        EXPECT_TRUE(read_content.has_value());
        EXPECT_EQ(*read_content, SampleText);
    }
    {
        // Write should also work with non existing file
        auto file_path = NonExistentFile;
        {
            auto res = write_text(file_path, SampleText);
            EXPECT_TRUE(res.has_value());
        }

        // Read back the file content
        auto read_content = read_text(file_path);
        EXPECT_TRUE(read_content.has_value());
        EXPECT_EQ(*read_content, SampleText);
    }
}

TEST_F(FileIoTest, ReadBinary)
{
    {
        // Read should work with existing text file
        auto file_path = ExistentFile;
        {
            // Create sample file for testing
            auto file = output_stream(file_path, std::ios::trunc | std::ios::binary);
            EXPECT_TRUE(file.has_value());

            file->write(
                reinterpret_cast<const char*>(SampleBinary.data()), // NOLINT
                static_cast<std::streamsize>(SampleBinary.size())
            );
        }

        auto read_bin = read_binary(file_path);
        EXPECT_TRUE(read_bin.has_value());
        EXPECT_EQ(read_bin->size(), SampleBinary.size());
        EXPECT_TRUE(std::ranges::equal(*read_bin, SampleBinary));
    }
}

TEST_F(FileIoTest, WriteBinary)
{
    {
        // Write should work with existing binary file
        auto file_path = ExistentFile;
        {
            auto res = write_binary(file_path, SampleBinary);
            EXPECT_TRUE(res.has_value());
        }

        // Read back the file content
        auto read_bin = read_binary(file_path);
        EXPECT_TRUE(read_bin.has_value());
        EXPECT_TRUE(std::ranges::equal(*read_bin, SampleBinary));
    }
    {
        // Write should also work with non existing file
        auto file_path = NonExistentFile;
        {
            auto res = write_binary(file_path, SampleBinary);
            EXPECT_TRUE(res.has_value());
        }

        // Read back the file content
        auto read_bin = read_binary(file_path);
        EXPECT_TRUE(read_bin.has_value());
        EXPECT_TRUE(std::ranges::equal(*read_bin, SampleBinary));
    }
}