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
    if (context.scale) {
        config.video.scale = *context.scale;
    }
    if (context.speed) {
        config.emulator.speed = *context.speed;
    }
    if (context.vsync) {
        config.video.vsync = *context.vsync;
    }
    if (context.log_level) {
        config.debug.log_level = *context.log_level;
    }

    auto result = common::config::ConfigValidator::validate(config, true);
    common::config::ConfigValidator::check_result(result);

    return app.run(context.rom_path);
}

} // namespace boyboy::app::commands