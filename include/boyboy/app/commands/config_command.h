/**
 * @file config_command.h
 * @brief Config command for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

#include "boyboy/app/commands/command.h"

namespace boyboy::app::commands {

class ConfigCommand : public ICommand {
public:
    enum class SubCommand : uint8_t { Get, Set, List, Reset, None };

    [[nodiscard]] std::string_view name() const override { return Name; }
    [[nodiscard]] std::string_view description() const override { return Description; }
    int execute(App& app, const CommandContext& context) override;

    // Accessors
    [[nodiscard]] SubCommand get_subcommand() const { return subcommand_; }
    void set_subcommand(SubCommand cmd) { subcommand_ = cmd; }
    [[nodiscard]] std::optional<std::string> get_key() const { return key_; }
    void set_key(const std::optional<std::string>& key) { key_ = key; }
    [[nodiscard]] std::optional<std::string> get_value() const { return value_; }
    void set_value(const std::optional<std::string>& value) { value_ = value; }

private:
    static constexpr std::string_view Name = "config";
    static constexpr std::string_view Description = "Manage emulator configuration";

    SubCommand subcommand_{SubCommand::None};
    std::optional<std::string> key_;
    std::optional<std::string> value_;

    int execute_get(App& app, const CommandContext& context);
    int execute_set(App& app, const CommandContext& context);
    int execute_list(App& app, const CommandContext& context);
    int execute_reset(App& app, const CommandContext& context);
};

} // namespace boyboy::app::commands