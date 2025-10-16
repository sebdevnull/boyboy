/**
 * @file run_command.h
 * @brief Run command for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <optional>
#include <string_view>

#include "boyboy/app/app.h"
#include "boyboy/app/commands/command.h"

namespace boyboy::app::commands {

class RunCommand : public ICommand {
public:
    [[nodiscard]] std::string_view name() const override { return Name; }
    [[nodiscard]] std::string_view description() const override { return Description; }
    int execute(App& app, const CommandContext& context) override;

    // Accessors
    [[nodiscard]] std::optional<int> get_speed() const { return speed_; }
    void set_speed(std::optional<int> speed) { speed_ = speed; }
    [[nodiscard]] std::optional<int> get_scale() const { return scale_; }
    void set_scale(std::optional<int> scale) { scale_ = scale; }
    [[nodiscard]] std::optional<bool> get_vsync() const { return vsync_; }
    void set_vsync(std::optional<bool> vsync) { vsync_ = vsync; }

private:
    static constexpr std::string_view Name = "run";
    static constexpr std::string_view Description = "Run the emulator with the specified ROM file";

    std::optional<int> scale_;
    std::optional<int> speed_;
    std::optional<bool> vsync_;
};

} // namespace boyboy::app::commands