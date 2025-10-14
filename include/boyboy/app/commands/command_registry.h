/**
 * @file command_registry.h
 * @brief Command registry for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include "boyboy/app/commands/command.h"

namespace boyboy::app {
class App;
}

namespace boyboy::app::commands {

// Macro to register a command class with the registry
#define REGISTER_COMMAND(TYPE)                                                                     \
    static const bool TYPE##_registered = [] {                                                     \
        boyboy::app::commands::CommandRegistry::instance().register_factory(#TYPE, [] {            \
            return std::make_unique<TYPE>();                                                       \
        });                                                                                        \
        return true;                                                                               \
    }()

// Concept to ensure the registrar has the required register_command method
template <typename T>
concept CommandRegistrar = requires(T& registrar, ICommand& cmd) {
    registrar.register_command(cmd);
};

class CommandRegistry {
public:
    using Factory = std::function<std::unique_ptr<ICommand>()>;

    static CommandRegistry& instance()
    {
        static CommandRegistry reg;
        return reg;
    }

    void register_factory(std::string_view name, Factory factory)
    {
        factories_[std::string(name)] = std::move(factory);
    }

    template <typename CommandRegister>
        requires CommandRegistrar<CommandRegister>
    void register_all(CommandRegister& reg)
    {
        for (auto& [name, factory] : factories_) {
            auto cmd = factory();
            reg.register_command(*cmd);
            commands_.push_back(std::move(cmd));
        }
    }

private:
    std::unordered_map<std::string, Factory> factories_;
    std::vector<std::unique_ptr<ICommand>> commands_;
};

} // namespace boyboy::app::commands