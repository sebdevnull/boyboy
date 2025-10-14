/**
 * @file cli_adapter.h
 * @brief CLI adapter interface for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <span>
#include <string_view>

#include "boyboy/app/commands/command.h"

namespace boyboy::frontend::cli {

class ICLIAdapter {
public:
    ICLIAdapter() = default;
    virtual ~ICLIAdapter() = default;
    ICLIAdapter(ICLIAdapter&) = delete;
    ICLIAdapter(ICLIAdapter&&) = delete;
    ICLIAdapter& operator=(ICLIAdapter&) = delete;
    ICLIAdapter& operator=(ICLIAdapter&&) = delete;

    virtual int run(std::span<std::string_view> args) = 0;
    virtual void register_command(app::commands::ICommand& command) = 0;
};

} // namespace boyboy::frontend::cli
