/**
 * @file cli_app.h
 * @brief Command line interface application for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <memory>
#include <span>

#include "boyboy/app/commands/command.h"
#include "boyboy/frontend/cli/adapters/cli_adapter.h"

namespace boyboy::app {
class App;
}

namespace boyboy::frontend::cli {

class CLIApp {
public:
    CLIApp();
    int run(std::span<std::string_view> args);

private:
    std::shared_ptr<app::App> app_;
    app::commands::CommandContext context_;
    std::unique_ptr<ICLIAdapter> cli_adapter_;

    void setup_commands();
};

} // namespace boyboy::frontend::cli