/**
 * @file main.cpp
 * @brief Main entry point for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <iostream>
#include <span>
#include <stdexcept>

#include "boyboy/common/config/config.h"
#include "boyboy/common/config/config_utils.h"
#include "boyboy/common/log/logging.h"
#include "boyboy/core/emulator/emulator.h"

using namespace boyboy::common;
using namespace boyboy::core;

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

    // Load and apply configuration
    config::Config config = config::load_config();
    emulator.apply_config(config);

    // Load ROM
    try {
        emulator.load(args[1]);
    }
    catch (const std::runtime_error& e) {
        log::error("Failed to load ROM: {}", e.what());
        return 1;
    }

    // Run emulator
    emulator.run();

    log::info("Exiting BoyBoy emulator");
    log::shutdown();

    return 0;
}