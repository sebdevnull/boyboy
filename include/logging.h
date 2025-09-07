/**
 * @file logging.h
 * @brief Logging utilities for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace boyboy::log {

inline void init(const std::string& log_file = "logs/boyboy.log", bool async = true)
{
    try {
        std::shared_ptr<spdlog::logger> logger;

        // Create sinks: console + file
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_file, false);

        if (async) {
            // Thread pool: queue size 8192, 1 background thread
            spdlog::init_thread_pool(8192, 1);
            logger = std::make_shared<spdlog::async_logger>(
                "boyboy",
                spdlog::sinks_init_list{console_sink, file_sink},
                spdlog::thread_pool(),
                spdlog::async_overflow_policy::block);
        }
        else {
            logger = std::make_shared<spdlog::logger>(
                "boyboy", spdlog::sinks_init_list{console_sink, file_sink});
        }

        spdlog::set_default_logger(logger);
        spdlog::set_level(spdlog::level::trace);
        spdlog::flush_on(spdlog::level::info);
    }
    catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "Logging initialization failed: " << ex.what() << "\n";
    }
}

// --- Logging functions ---
template <typename... Args>
inline void trace(const char* fmt, Args&&... args)
{
#ifdef BOYBOY_DEBUG
    spdlog::log(spdlog::level::trace, fmt::runtime(fmt), std::forward<Args>(args)...);
#else
    (void)fmt;             // avoid unused parameter warning
    (void)sizeof...(args); // avoids unused parameter pack warning
#endif
}

template <typename... Args>
inline void debug(const char* fmt, Args&&... args)
{
#ifdef BOYBOY_DEBUG
    spdlog::log(spdlog::level::debug, fmt::runtime(fmt), std::forward<Args>(args)...);
#else
    (void)fmt;             // avoid unused parameter warning
    (void)sizeof...(args); // avoids unused parameter pack warning
#endif
}

template <typename... Args>
inline void info(const char* fmt, Args&&... args)
{
    spdlog::log(spdlog::level::info, fmt::runtime(fmt), std::forward<Args>(args)...);
}

template <typename... Args>
inline void warn(const char* fmt, Args&&... args)
{
    spdlog::log(spdlog::level::warn, fmt::runtime(fmt), std::forward<Args>(args)...);
}

template <typename... Args>
inline void error(const char* fmt, Args&&... args)
{
    spdlog::log(spdlog::level::err, fmt::runtime(fmt), std::forward<Args>(args)...);
}

// Shutdown logging (flushes and cleans up)
inline void shutdown()
{
    spdlog::shutdown();
}

} // namespace boyboy::log