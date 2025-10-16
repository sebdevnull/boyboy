/**
 * @file config_command.cpp
 * @brief Config command for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/app/commands/config_command.h"

#include <iostream>
#include <stdexcept>

#include "boyboy/app/app.h"
#include "boyboy/app/commands/command_registry.h"
#include "boyboy/common/config/config.h"

namespace boyboy::app::commands {

namespace {
REGISTER_COMMAND(ConfigCommand);
}

int ConfigCommand::execute(App& app, const CommandContext& context)
{
    switch (subcommand_) {
        case SubCommand::Get:
            return execute_get(app, context);
        case SubCommand::Set:
            return execute_set(app, context);
        case SubCommand::List:
            return execute_list(app, context);
        case SubCommand::Reset:
            return execute_reset(app, context);
        default:
            break;
    }

    return 1;
}

int ConfigCommand::execute_get(App& app, const CommandContext& context)
{
    if (!key_) {
        return 1;
    }

    auto& config = app.load_config(context.config_path);
    try {
        std::cout << config.key_value_str(*key_) << "\n";
        return 0;
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Error retrieving key " << *key_ << ": " << e.what() << "\n";
    }

    return 1;
}
int ConfigCommand::execute_set(App& app, const CommandContext& context)
{
    if (!key_ || !value_) {
        return 1;
    }

    auto& config = app.load_config(context.config_path);
    try {
        config.set_string(*key_, *value_);
        app.save_config(config, context.config_path);
        std::cout << config.key_value_str(*key_) << "\n";
        return 0;
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Error setting key " << *key_ << ": " << e.what() << "\n";
    }

    return 1;
}

int ConfigCommand::execute_list(App& app, const CommandContext& context) // NOLINT
{
    auto& config = app.load_config(context.config_path);
    for (const auto& key : common::config::ConfigKeys::KeyList) {
        std::cout << config.key_value_str(key) << "\n";
    }

    return 0;
}

int ConfigCommand::execute_reset(App& app, const CommandContext& context) // NOLINT
{
    auto config = common::config::Config::default_config();
    app.save_config(config, context.config_path);
    std::cout << "Resetted configuration to default" << "\n";
    return 0;
}

} // namespace boyboy::app::commands