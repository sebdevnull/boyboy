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
    config.battery.autosave = autosave_.value_or(config.battery.autosave);
    config.battery.interval_ms = save_interval_ms_.value_or(config.battery.interval_ms);

    auto result = common::config::ConfigValidator::validate(config, true);
    common::config::ConfigValidator::check_result(result);

    if (save_path_) {
        app.set_battery_save_path(*save_path_);
    }

    return app.run(context.rom_path);
}

} // namespace boyboy::app::commands