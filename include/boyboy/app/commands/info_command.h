/**
 * @file info_command.h
 * @brief Info command for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include "boyboy/app/commands/command.h"

namespace boyboy::app::commands {

class InfoCommand : public ICommand {
public:
    [[nodiscard]] std::string_view name() const override { return Name; }
    [[nodiscard]] std::string_view description() const override { return Description; }
    int execute(App& app, const CommandContext& context) override;

private:
    static constexpr std::string_view Name = "info";
    static constexpr std::string_view Description = "Display ROM metadata information";
};

} // namespace boyboy::app::commands