/**
 * @file command.h
 * @brief Command interface for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <string>
#include <string_view>

namespace boyboy::app {
class App;
}

namespace boyboy::app::commands {

struct CommandContext {
    std::string rom_path;
    std::string config_path;
};

class ICommand {
public:
    ICommand() = default;
    virtual ~ICommand() = default;
    ICommand(ICommand&) = delete;
    ICommand(ICommand&&) = delete;
    ICommand& operator=(ICommand&) = delete;
    ICommand& operator=(ICommand&&) = delete;

    [[nodiscard]] virtual std::string_view name() const = 0;
    [[nodiscard]] virtual std::string_view description() const = 0;
    virtual int execute(App& app, const CommandContext& context) = 0;
};

} // namespace boyboy::app::commands