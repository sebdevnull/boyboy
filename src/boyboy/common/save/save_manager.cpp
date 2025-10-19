/**
 * @file save_manager.cpp
 * @brief Save Manager for BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/common/save/save_manager.h"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <filesystem>
#include <numeric>
#include <span>
#include <string_view>
#include <vector>

#include "boyboy/common/files/io.h"
#include "boyboy/common/files/paths.h"
#include "boyboy/common/log/logging.h"
#include "boyboy/common/utils.h"

namespace boyboy::common::save {

constexpr auto BatteryRamFile = "battery.sav";

auto SaveManager::save_eram(
    std::string_view rom_title,
    std::span<const uint8_t> data,
    const std::optional<std::filesystem::path>& save_path
) -> std::expected<void, std::string>
{
    auto file_path = save_path.value_or(eram_path(rom_title));
    log::debug("[SaveManager] Saving ERAM to: {}", file_path.string());

    if (!std::filesystem::exists(file_path)) {
        log::debug(
            "[SaveManager] ERAM save file does not exist, creating new one: {}", file_path.string()
        );
    }

    // Create a vector of bytes to append the checksum
    std::vector<std::byte> eram_bytes{data.size() + 2}; // fit all data + checksum
    std::ranges::transform(data.begin(), data.end(), eram_bytes.begin(), [](uint8_t b) {
        return std::byte{b};
    });

    // Calculate checksum and write to buffer
    uint16_t cks = checksum(data);
    write_checksum(eram_bytes, cks);

    auto res = files::atomic_write(file_path, eram_bytes);
    if (!res) {
        log::error(
            "[SaveManager] Error writing ERAM to save file: {}", res.error().error_message()
        );
        return std::unexpected(res.error().error_message());
    }

    log::info("[SaveManager] ERAM saved to file: {}", file_path.string());

    return {};
}

auto SaveManager::load_eram(
    std::string_view rom_title, const std::optional<std::filesystem::path>& save_path
) -> std::expected<std::vector<uint8_t>, std::string>
{
    auto file_path = save_path.value_or(eram_path(rom_title));
    log::debug("[SaveManager] Loading ERAM from: {}", file_path.string());

    auto eram_bytes = files::read_binary(file_path);
    if (!eram_bytes) {
        log::error("[SaveManager] Error loading ERAM file: {}", eram_bytes.error().error_message());
        return std::unexpected(eram_bytes.error().error_message());
    }

    if (eram_bytes->size() < 2) {
        log::error("[SaveManager] ERAM save file too small");
        return std::unexpected(eram_bytes.error().error_message());
    }

    // Create a span view to calculate the checksum and avoid allocation if it fails
    auto eram_span = std::span(
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        reinterpret_cast<const uint8_t*>(eram_bytes->data()),
        eram_bytes->size() - 2
    );

    // Compare checksums
    uint16_t read_cks = read_checksum(*eram_bytes);
    uint16_t calc_cks = checksum(eram_span);
    if (read_cks != calc_cks) {
        log::error(
            "[SaveManager] Checksum mismatch for ERAM save file: read {}, calc {}",
            utils::PrettyHex{read_cks}.to_string(),
            utils::PrettyHex{calc_cks}.to_string()
        );
        return {};
    }

    log::info("[SaveManager] ERAM save file loaded from: {}", file_path.string());

    return std::vector<uint8_t>{eram_span.begin(), eram_span.end()};
}

std::filesystem::path SaveManager::eram_path(std::string_view rom_title)
{
    return files::DataDir / utils::normalize_rom_title(rom_title) / BatteryRamFile;
}

uint16_t SaveManager::checksum(std::span<const uint8_t> data)
{
    uint16_t cks = 0;
    cks = std::accumulate(data.begin(), data.end(), cks);

    return cks;
}

[[nodiscard]] uint16_t SaveManager::read_checksum(std::span<const std::byte> buf)
{
    std::span<const std::byte> cks_span(buf.end() - 2, buf.end());
    return utils::to_u16(cks_span[0], cks_span[1]);
}

void SaveManager::write_checksum(std::span<std::byte> buf, uint16_t cks)
{
    std::span<std::byte> cks_span(buf.end() - 2, buf.end());
    cks_span[0] = std::byte{utils::msb(cks)};
    cks_span[1] = std::byte{utils::lsb(cks)};
}

} // namespace boyboy::common::save