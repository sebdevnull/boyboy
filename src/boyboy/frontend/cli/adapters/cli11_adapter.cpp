/**
 * @file cli11_adapter.cpp
 * @brief CLI11 adapter for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/frontend/cli/adapters/cli11_adapter.h"

#include <CLI/CLI.hpp>

#include "boyboy/common/log/logging.h"

namespace boyboy::frontend::cli {

inline static constexpr std::string_view RunCommandName = "run";

CLI11Adapter::CLI11Adapter(app::App& app, app::commands::CommandContext& context)
    : app_(app), context_(context)
{
    app_parser_.name("boyboy");
    app_parser_.description("BoyBoy - A Game Boy emulator");
    app_parser_.set_help_flag("-h,--help", "Print help message and exit");
    app_parser_.set_help_all_flag("--help-all", "Print all help messages and exit");
    app_parser_.require_subcommand(1);
}

int CLI11Adapter::run(std::span<std::string_view> args)
{
    return parse(args);
}

void CLI11Adapter::register_command(app::commands::ICommand& command)
{
    if (command.name() == RunCommandName) {
        register_run(command);
    }
    else {
        throw std::runtime_error(
            std::string("Unsupported command: ") + std::string(command.name())
        );
    }
}

int CLI11Adapter::parse(std::span<std::string_view> args)
{
    std::vector<const char*> argv;
    for (const auto& arg : args) {
        argv.push_back(arg.data());
    }

    try {
        app_parser_.parse(static_cast<int>(argv.size()), argv.data());
    }
    catch (const CLI::ParseError& e) {
        return app_parser_.exit(e);
    }
    catch (const std::exception& e) {
        common::log::error("Error running command: {}", e.what());
        return 1;
    }

    return 0;
}

void CLI11Adapter::register_run(app::commands::ICommand& command)
{
    auto* cmd = app_parser_.add_subcommand(
        std::string(command.name()), std::string(command.description())
    );
    cmd->add_option("rom", context_.rom_path, "Path to the ROM file")
        ->option_text("ROM_PATH")
        ->required();
    cmd->add_option("-c,--config", context_.config_path, "Path to the configuration file")
        ->option_text("CONFIG_PATH")
        ->default_val("");
    cmd->callback([this, &command]() { command.execute(app_, context_); });
}

} // namespace boyboy::frontend::cli