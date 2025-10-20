/**
 * @file cartridge_loader.h
 * @brief Cartridge loader for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <memory>
#include <string_view>
#include <vector>

namespace boyboy::core::cartridge {

class Cartridge;
using RomData = std::vector<std::byte>;

class CartridgeLoader {
public:
    static std::unique_ptr<Cartridge> load(std::string_view path);
    static std::unique_ptr<Cartridge> load(RomData&& rom_data);
    static std::unique_ptr<Cartridge> load(const RomData& rom_data);

private:
    static RomData load_rom_data(std::string_view path);
};

} // namespace boyboy::core::cartridge