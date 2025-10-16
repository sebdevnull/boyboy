/**
 * @file main.cpp
 * @brief Main entry point for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include <span>
#include <string_view>

#include "boyboy/common/log/logging.h"
#include "boyboy/frontend/cli/cli_app.h"

using namespace boyboy::common;
using namespace boyboy::frontend::cli;

int main(int argc, const char** argv)
{
    // Convert argv to vector of string_view for better handling
    std::vector<std::string_view> args;
    for (const auto* arg : std::span(argv, static_cast<std::size_t>(argc))) {
        args.emplace_back(arg);
    }

    log::init("logs/boyboy.log", true);

    CLIApp app{};
    int result = app.run(args);

    log::shutdown();

    return result;
}