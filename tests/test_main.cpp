/**
 * @file test_main.cpp
 * @brief Main entry point for the BoyBoy emulator unit tests.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include <filesystem>
#include <iostream>

// helpers
#include "helpers/global_tick_mode.h"

// boyboy
#include "boyboy/common/log/logging.h"
#include "boyboy/core/cpu/cycles.h"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    // Initialize logging
    boyboy::common::log::init("logs/test_boyboy.log", true);

    using boyboy::core::cpu::TickMode;
    using boyboy::tests::g_tick_mode;

    // Optional argument: --tick-mode=[instruction|mcycle|tcycle]
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i]; // NOLINT
        if (arg == "--tick-mode=instruction") {
            g_tick_mode = TickMode::Instruction;
        }
        else if (arg == "--tick-mode=mcycle") {
            g_tick_mode = TickMode::MCycle;
        }
        else if (arg == "--tick-mode=tcycle") {
            g_tick_mode = TickMode::TCycle;
        }
    }

    std::cout << "\n=== Running tests with TickMode = " << to_string(g_tick_mode) << " ===\n";

    boyboy::common::log::debug(
        "Tests working directory: {}", std::filesystem::current_path().string()
    );

    int result = RUN_ALL_TESTS();

    boyboy::common::log::shutdown();
    return result;
}