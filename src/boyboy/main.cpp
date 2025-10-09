/**
 * @file main.cpp
 * @brief Main entry point for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <iostream>
#include <span>
#include <stdexcept>

#include "boyboy/emulator.h"
#include "boyboy/log/logging.h"

using namespace boyboy;

int main(int argc, const char** argv)
{
    std::span<const char*> args(argv, static_cast<std::size_t>(argc));

    if (args.size() != 2) {
        std::cout << "Usage: " << args[0] << " <path_to_rom>\n";
        return 1;
    }

    log::init("logs/boyboy.log", true);
    log::info("Starting BoyBoy emulator");

    emulator::Emulator emulator;

    try {
        emulator.load(args[1]);
    }
    catch (const std::runtime_error& e) {
        log::error("Failed to load ROM: {}", e.what());
        return 1;
    }

    emulator.run();

    log::info("Exiting BoyBoy emulator");
    log::shutdown();

    return 0;
}