/**
 * @file cli11_adapter.h
 * @brief CLI11 adapter for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <CLI/CLI.hpp>
#include <memory>
#include <optional>

#include "boyboy/app/commands/command.h"
#include "boyboy/frontend/cli/adapters/cli_adapter.h"

namespace boyboy::app::commands {
class RunCommand;
class InfoCommand;
class ConfigCommand;
} // namespace boyboy::app::commands

namespace boyboy::frontend::cli {
class CLI11Adapter : public ICLIAdapter {
public:
    explicit CLI11Adapter(std::shared_ptr<app::App> app, app::commands::CommandContext& context);

    int run(std::span<std::string_view> args) override;
    void register_command(app::commands::ICommand& command) override;

private:
    struct CLIOptions {
        // Run
        std::optional<int> scale;
        std::optional<int> speed;
        std::optional<bool> vsync;
        std::optional<std::string> save_path;
        std::optional<bool> autosave;
        std::optional<int> save_interval_ms;
        std::optional<std::string> tick_mode;
        // Config
        std::optional<std::string> cfg_key;
        std::optional<std::string> cfg_value;
    } options_;

    CLI::App app_parser_;
    std::shared_ptr<app::App> app_;
    app::commands::CommandContext& context_;

    int parse(std::span<std::string_view> args);

    void register_run(app::commands::RunCommand& command);
    void register_info(app::commands::InfoCommand& command);
    void register_config(app::commands::ConfigCommand& command);
};

} // namespace boyboy::frontend::cli