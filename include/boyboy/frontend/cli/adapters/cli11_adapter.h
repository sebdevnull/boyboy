/**
 * @file cli11_adapter.h
 * @brief CLI11 adapter for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <CLI/CLI.hpp>

#include "boyboy/app/commands/command.h"
#include "boyboy/frontend/cli/adapters/cli_adapter.h"

namespace boyboy::frontend::cli {
class CLI11Adapter : public ICLIAdapter {
public:
    explicit CLI11Adapter(app::App& app, app::commands::CommandContext& context);

    int run(std::span<std::string_view> args) override;
    void register_command(app::commands::ICommand& command) override;

private:
    CLI::App app_parser_;
    app::App& app_;
    app::commands::CommandContext& context_;

    int parse(std::span<std::string_view> args);

    void register_run(app::commands::ICommand& command);
};

} // namespace boyboy::frontend::cli