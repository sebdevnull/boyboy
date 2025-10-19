/**
 * @file save_manager.h
 * @brief Save Manager for BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstdint>
#include <expected>
#include <filesystem>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace boyboy::core::cartridge {
class Cartridge;
}

namespace boyboy::common::save {

class SaveManager {
public:
    /** @brief Save ERAM data to disk.
     *
     * @param rom_title Title of the ROM (used to determine save file path)
     * @param data Span of ERAM data to save
     * @param save_path Optional custom save file path
     * @return std::expected<void, std::string> Empty on success, error message on failure
     */
    static auto save_eram(
        std::string_view rom_title,
        std::span<const uint8_t> data,
        const std::optional<std::filesystem::path>& save_path = {}
    ) -> std::expected<void, std::string>;

    /** @brief Load ERAM data from disk.
     *
     * @param rom_title Title of the ROM (used to determine save file path)
     * @param save_path Optional custom save file path
     * @return std::expected<std::vector<uint8_t>, std::string> Loaded ERAM data on success, error
     * message on failure
     */
    static auto load_eram(
        std::string_view rom_title, const std::optional<std::filesystem::path>& save_path = {}
    ) -> std::expected<std::vector<uint8_t>, std::string>;

private:
    static std::filesystem::path eram_path(std::string_view rom_title);
    static uint16_t checksum(std::span<const uint8_t> data);
    [[nodiscard]] static uint16_t read_checksum(std::span<const std::byte> buf);
    static void write_checksum(std::span<std::byte> buf, uint16_t cks);
};

} // namespace boyboy::common::save