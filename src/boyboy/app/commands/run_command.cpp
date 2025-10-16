/**
 * @file run_command.cpp
 * @brief Run command for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/app/commands/run_command.h"

#include "boyboy/app/commands/command_registry.h"
#include "boyboy/common/config/config_validator.h"

namespace boyboy::app::commands {

namespace {
REGISTER_COMMAND(RunCommand);
}

int RunCommand::execute(App& app, const CommandContext& context)
{
    auto& config = app.load_config(context.config_path);
    config.video.scale = scale_.value_or(config.video.scale);
    config.emulator.speed = speed_.value_or(config.emulator.speed);
    config.video.vsync = vsync_.value_or(config.video.vsync);
    config.debug.log_level = context.log_level.value_or(config.debug.log_level);

    auto result = common::config::ConfigValidator::validate(config, true);
    common::config::ConfigValidator::check_result(result);

    return app.run(context.rom_path);
}

} // namespace boyboy::app::commands