/**
 * @file info_command.cpp
 * @brief Info command for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/app/commands/info_command.h"

#include <iostream>
#include <string>

#include "boyboy/app/app.h"
#include "boyboy/app/commands/command_registry.h"
#include "boyboy/common/log/logging.h"

namespace boyboy::app::commands {

namespace {
REGISTER_COMMAND(InfoCommand);
}

int InfoCommand::execute([[maybe_unused]] App& app, const CommandContext& context)
{
    try {
        common::log::set_level(common::log::LogLevel::Off); // disable logging for info command
        std::string info = App::rom_info(context.rom_path);
        std::cout << info << "\n";
        return 0;
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Failed to load ROM: " << e.what() << "\n";
        return 1;
    }
}

} // namespace boyboy::app::commands