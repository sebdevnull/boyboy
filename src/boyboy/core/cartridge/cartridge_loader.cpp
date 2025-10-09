/**
 * @file cartridge_loader.cpp
 * @brief Cartridge loader for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/core/cartridge/cartridge_loader.h"

#include <filesystem>
#include <format>
#include <fstream>

#include "boyboy/core/cartridge/cartridge.h"
#include "boyboy/common/utils.h"
#include "boyboy/common/log/logging.h"

namespace boyboy::core::cartridge {

using namespace boyboy::common;

Cartridge CartridgeLoader::load(std::string_view path)
{
    return load(load_rom_data(path));
}

Cartridge CartridgeLoader::load(RomData&& rom_data)
{
    return {std::move(rom_data)};
}

Cartridge CartridgeLoader::load(const RomData& rom_data)
{
    return {rom_data};
}

RomData CartridgeLoader::load_rom_data(std::string_view path)
{
    namespace fs = std::filesystem;

    fs::path file_path(path);

    if (!fs::exists(file_path)) {
        log::debug("Current path: {}", fs::current_path().string());
        throw std::runtime_error(std::format("ROM File not found: {}", path));
    }

    std::uintmax_t file_size = 0;
    try {
        file_size = fs::file_size(file_path);
    }
    catch (const fs::filesystem_error& e) {
        throw std::runtime_error(std::format("Failed to get ROM file size: {}", e.what()));
    }

    std::ifstream rom_file(file_path, std::ios::binary);
    if (!rom_file.is_open()) {
        throw std::runtime_error(std::format("Failed to open ROM file: {}", path));
    }

    RomData rom_data(file_size);
    rom_file.read(utils::as_char_ptr(rom_data.data()), static_cast<std::streamsize>(file_size));

    if (rom_file.gcount() != static_cast<std::streamsize>(file_size)) {
        rom_data.clear();
        throw std::runtime_error("Failed to read entire ROM file");
    }

    return rom_data;
}

} // namespace boyboy::core::cartridge