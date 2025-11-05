/**
 * @file cli11_adapter.cpp
 * @brief CLI11 adapter for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/frontend/cli/adapters/cli11_adapter.h"

#include <CLI/CLI.hpp>
#include <optional>
#include <string>
#include <string_view>

#include "boyboy/app/app.h"
#include "boyboy/app/commands/config_command.h"
#include "boyboy/app/commands/info_command.h"
#include "boyboy/app/commands/run_command.h"
#include "boyboy/common/config/config_limits.h"
#include "boyboy/common/log/logging.h"
#include "boyboy/version.h"

namespace boyboy::frontend::cli {

inline static constexpr std::string_view RunCommandName = "run";
inline static constexpr std::string_view InfoCommandName = "info";
inline static constexpr std::string_view ConfigCommandName = "config";

inline static const std::string GlobalFooter = std::format(
    "For more information and bug reports, visit <https://github.com/sebdevnull/boyboy>\n\n{}",
    version::LicenseLong
);

namespace {
std::string make_footer(std::string&& footer)
{
    return std::move(footer) + "\n" + GlobalFooter;
}
} // namespace

CLI11Adapter::CLI11Adapter(std::shared_ptr<app::App> app, app::commands::CommandContext& context)
    : app_(std::move(app)), context_(context)
{
    app_parser_.name("boyboy");
    app_parser_.description("BoyBoy - A Game Boy emulator");
    app_parser_.set_help_flag("-h,--help", "Print help message and exit");
    app_parser_.set_version_flag("-v,--version", app::App::version());
    app_parser_.add_flag_callback(
        "--build-info",
        [] {
            std::cout << app::App::build_info() << "\n";
            std::exit(0);
        },
        "Display detailed build information and exit"
    );

    app_parser_.footer(GlobalFooter);
}

int CLI11Adapter::run(std::span<std::string_view> args)
{
    return parse(args);
}

void CLI11Adapter::register_command(app::commands::ICommand& command)
{
    if (command.name() == RunCommandName) {
        register_run(dynamic_cast<app::commands::RunCommand&>(command));
    }
    else if (command.name() == InfoCommandName) {
        register_info(dynamic_cast<app::commands::InfoCommand&>(command));
    }
    else if (command.name() == ConfigCommandName) {
        register_config(dynamic_cast<app::commands::ConfigCommand&>(command));
    }
    else {
        throw std::runtime_error(
            std::string("Unsupported command: ") + std::string(command.name())
        );
    }
}

int CLI11Adapter::parse(std::span<std::string_view> args)
{
    if (args.size() <= 1) {
        std::cout << app_parser_.help();
        return 0;
    }

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

void CLI11Adapter::register_run(app::commands::RunCommand& command)
{
    auto* cmd = app_parser_.add_subcommand(
        std::string(command.name()), std::string(command.description())
    );
    cmd->footer(make_footer(R"(
        Examples:
          boyboy run path/to/rom.gb
          boyboy run path/to/rom.gb --config path/to/config.toml
          boyboy run path/to/rom.gb --scale 2 --speed 2 --vsync off
        
        Notes:
          Any options provided here will override those in the configuration file.
    )"));

    cmd->add_option("<rom>", context_.rom_path, "Path to the ROM file")->type_name("")->required();
    cmd->add_option("-c,--config", context_.config_path, "Path to the configuration file")
        ->type_name("<file>");
    cmd->add_option("--scale", options_.scale, "Display scaling factor")->type_name("<factor>");
    cmd->add_option("--speed", options_.speed, "Emulation speed multiplier (0 = uncapped)")
        ->type_name("<mult>");
    cmd->add_option("--vsync", options_.vsync, "Enable or disable vertical synchronization")
        ->type_name("<bool>");
    cmd->add_option(
           "--log-level",
           context_.log_level,
           std::format(
               "Logging level: {}",
               common::config::ConfigLimits::Debug::LogLevelOptions.option_list()
           )
    )
        ->option_text("<level>")
        ->check(CLI::IsMember(common::config::ConfigLimits::Debug::LogLevels));

    auto tickmode_validator = CLI::Validator(
        [](const std::string& input) {
            for (auto opt : common::config::ConfigLimits::Emulator::TickModes) {
                if (opt == input) {
                    return std::string();
                }
            }
            return std::format("Invalid tick mode option: {}", input);
        },
        "TICKMODE_VALIDATOR",
        "Tickmode Validator"
    );
    cmd->add_option(
           "--tick-mode",
           options_.tick_mode,
           std::format(
               "CPU ticking mode: {}",
               common::config::ConfigLimits::Emulator::TickModeOptions.option_list()
           )
    )
        ->option_text("<mode>")
        ->check(tickmode_validator);

    // Battery save options
    cmd->add_option("--save-file", options_.save_path, "Battery save file for this ROM")
        ->type_name("<file>");
    cmd->add_option("--autosave", options_.autosave, "Enable or disable battery autosave")
        ->type_name("<bool>");
    cmd->add_option(
           "--save-interval", options_.save_interval_ms, "Battery autosave interval in milliseconds"
    )
        ->type_name("<t>");

    cmd->callback([this, &command]() {
        command.set_scale(options_.scale);
        command.set_speed(options_.speed);
        command.set_vsync(options_.vsync);
        command.set_save_path(options_.save_path);
        command.set_autosave(options_.autosave);
        command.set_save_interval_ms(options_.save_interval_ms);
        command.set_tick_mode(options_.tick_mode);
        command.execute(*app_, context_);
    });
}

void CLI11Adapter::register_info(app::commands::InfoCommand& command)
{
    auto* cmd = app_parser_.add_subcommand(
        std::string(command.name()), std::string(command.description())
    );
    cmd->footer(make_footer(R"(
        Examples:
          boyboy info path/to/rom.gb
    )"));

    cmd->add_option("<rom>", context_.rom_path, "Path to the ROM file")->type_name("")->required();

    cmd->callback([this, &command]() { command.execute(*app_, context_); });
}

void CLI11Adapter::register_config(app::commands::ConfigCommand& command)
{
    auto* cli_config = app_parser_.add_subcommand(
        std::string(command.name()), std::string(command.description())
    );
    cli_config->add_option("-c,--config", context_.config_path, "Path to the configuration file")
        ->type_name("<file>");
    cli_config->require_subcommand(1);

    // Get
    auto* get_sub = cli_config->add_subcommand("get", "Get a configuration value");
    get_sub->add_option("<key>", options_.cfg_key, "Configuration key")->type_name("")->required();
    get_sub->callback([this, &command]() {
        command.set_subcommand(app::commands::ConfigCommand::SubCommand::Get);
        command.set_key(options_.cfg_key);
        command.execute(*app_, context_);
    });
    get_sub->footer(make_footer(R"(
        Examples:
          boyboy config get emulator.speed
          boyboy config -c path/to/config.toml get video.vsync
    )"));

    // Set
    auto* set_sub = cli_config->add_subcommand("set", "Set a configuration value");
    set_sub->add_option("<key>", options_.cfg_key, "Configuration key")->type_name("")->required();
    set_sub->add_option("<value>", options_.cfg_value, "Configuration value")
        ->type_name("")
        ->required();
    set_sub->callback([this, &command]() {
        command.set_subcommand(app::commands::ConfigCommand::SubCommand::Set);
        command.set_key(options_.cfg_key);
        command.set_value(options_.cfg_value);
        command.execute(*app_, context_);
    });
    set_sub->footer(make_footer(R"(
        Examples:
          boyboy config set emulator.speed 2
          boyboy config -c path/to/config.toml set video.vsync true
    )"));

    // List
    auto* list_sub = cli_config->add_subcommand("list", "List configuration values");
    list_sub->callback([this, &command]() {
        command.set_subcommand(app::commands::ConfigCommand::SubCommand::List);
        command.execute(*app_, context_);
    });
    list_sub->footer(make_footer(R"(
        Examples:
          boyboy config list
          boyboy config -c path/to/config.toml list
    )"));

    // Reset
    auto* reset_sub = cli_config->add_subcommand("reset", "Reset configuration to default");
    reset_sub->callback([this, &command]() {
        command.set_subcommand(app::commands::ConfigCommand::SubCommand::Reset);
        command.execute(*app_, context_);
    });
    reset_sub->footer(make_footer(R"(
        Examples:
          boyboy config reset
          boyboy config -c path/to/config.toml reset
    )"));
}

} // namespace boyboy::frontend::cli