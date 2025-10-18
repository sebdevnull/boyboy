/**
 * @file cartridge_loader.cpp
 * @brief Cartridge loader for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/core/cartridge/cartridge_loader.h"

#include <filesystem>
#include <format>
#include <memory>

#include "boyboy/common/files/errors.h"
#include "boyboy/common/files/io.h"
#include "boyboy/core/cartridge/cartridge.h"

namespace boyboy::core::cartridge {

using namespace boyboy::common;

std::unique_ptr<Cartridge> CartridgeLoader::load(std::string_view path)
{
    return load(load_rom_data(path));
}

std::unique_ptr<Cartridge> CartridgeLoader::load(RomData&& rom_data)
{
    return std::make_unique<Cartridge>(std::move(rom_data));
}

std::unique_ptr<Cartridge> CartridgeLoader::load(const RomData& rom_data)
{
    return std::make_unique<Cartridge>(rom_data);
}

RomData CartridgeLoader::load_rom_data(std::string_view path)
{
    std::filesystem::path file_path(path);

    auto rom_data = files::read_binary(file_path);
    if (!rom_data) {
        throw std::runtime_error(
            std::format("Error loading ROM data: {}", rom_data.error().error_message())
        );
    }

    return *rom_data;
}

} // namespace boyboy::core::cartridge