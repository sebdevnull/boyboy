/**
 * @file run_command.h
 * @brief Run command for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <string_view>

#include "boyboy/app/app.h"
#include "boyboy/app/commands/command.h"

namespace boyboy::app::commands {

class RunCommand : public ICommand {
public:
    [[nodiscard]] std::string_view name() const override { return Name; }
    [[nodiscard]] std::string_view description() const override { return Description; }
    int execute(App& app, const CommandContext& context) override;

private:
    static constexpr std::string_view Name = "run";
    static constexpr std::string_view Description = "Run the emulator with the specified ROM file";
};

} // namespace boyboy::app::commands