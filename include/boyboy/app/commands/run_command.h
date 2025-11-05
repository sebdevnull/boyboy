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
    [[nodiscard]] std::optional<std::string> get_save_path() const { return save_path_; }
    void set_save_path(std::optional<std::string> save_path) { save_path_ = std::move(save_path); }
    [[nodiscard]] std::optional<bool> get_autosave() const { return autosave_; }
    void set_autosave(std::optional<bool> autosave) { autosave_ = autosave; }
    [[nodiscard]] std::optional<int> get_save_interval_ms() const { return save_interval_ms_; }
    void set_save_interval_ms(std::optional<int> interval_ms) { save_interval_ms_ = interval_ms; }
    [[nodiscard]] std::optional<std::string> get_tick_mode() const { return tick_mode_; }
    void set_tick_mode(std::optional<std::string> tick_mode) { tick_mode_ = std::move(tick_mode); }

private:
    static constexpr std::string_view Name = "run";
    static constexpr std::string_view Description = "Run the emulator with the specified ROM file";

    std::optional<int> scale_;
    std::optional<int> speed_;
    std::optional<bool> vsync_;
    std::optional<std::string> save_path_;
    std::optional<bool> autosave_;
    std::optional<int> save_interval_ms_;
    std::optional<std::string> tick_mode_;
};

} // namespace boyboy::app::commands