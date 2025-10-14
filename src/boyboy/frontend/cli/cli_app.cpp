/**
 * @file cli_app.cpp
 * @brief Command line interface application for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/frontend/cli/cli_app.h"

#include "boyboy/app/commands/command_registry.h"
#include "boyboy/frontend/cli/adapters/cli11_adapter.h"

namespace boyboy::frontend::cli {

using CLIAdapter = CLI11Adapter;

CLIApp::CLIApp() : context_(), cli_adapter_(std::make_unique<CLIAdapter>(context_)) {}

int CLIApp::run(std::span<std::string_view> args)
{
    setup_commands();
    return cli_adapter_->run(args);
}

void CLIApp::setup_commands()
{
    auto& registry = app::commands::CommandRegistry::instance();
    registry.register_all(*cli_adapter_);
}

} // namespace boyboy::frontend::cli