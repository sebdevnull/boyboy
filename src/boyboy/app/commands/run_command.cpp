/**
 * @file run_command.cpp
 * @brief Run command for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/app/commands/run_command.h"

#include "boyboy/app/commands/command_registry.h"

namespace boyboy::app::commands {

namespace {
REGISTER_COMMAND(RunCommand);
}

} // namespace boyboy::app::commands