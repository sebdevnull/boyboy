/**
 * @file main.cpp
 * @brief Main entry point for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <iostream>
#include <span>

#include "cartridge.h"
#include "display.h"

using namespace boyboy;

int main(int argc, const char** argv) {
    std::span<const char*> args(argv, static_cast<std::size_t>(argc));

    cartridge::Cartridge cart{};

    if (args.size() == 2) {
        cart.load(args[1]);
    } else {
        std::cout << "Usage: " << args[0] << " <path_to_rom>\n";
        return 1;
    }

    cart.parse_header();

    auto header = cart.get_header();
    header.print();

    display::Display display;
    display.init();

    return 0;
}