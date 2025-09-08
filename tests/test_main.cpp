/**
 * @file test_main.cpp
 * @brief Main entry point for the BoyBoy emulator unit tests.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <gtest/gtest.h>

#include "logging.h"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    // Initialize logging
    boyboy::log::init("logs/test_boyboy.log", true);

    int result = RUN_ALL_TESTS();

    boyboy::log::shutdown();
    return result;
}