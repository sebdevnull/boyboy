/**
 * @file logging.h
 * @brief Logging utilities for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <iostream>
#include <memory>
#include <string>

namespace boyboy::log {

inline void init(const std::string& log_file = "logs/boyboy.log", bool async = true)
{
    try {
        std::shared_ptr<spdlog::logger> logger;
        bool truncate = true;
        auto log_level = spdlog::level::info;

#ifdef LOG_LEVEL
        if (std::string(LOG_LEVEL) == "TRACE") {
            log_level = spdlog::level::trace;
        }
        else if (std::string(LOG_LEVEL) == "DEBUG") {
            log_level = spdlog::level::debug;
        }
        else if (std::string(LOG_LEVEL) == "INFO") {
            log_level = spdlog::level::info;
        }
        else if (std::string(LOG_LEVEL) == "WARN") {
            log_level = spdlog::level::warn;
        }
        else if (std::string(LOG_LEVEL) == "ERROR") {
            log_level = spdlog::level::err;
        }
        else if (std::string(LOG_LEVEL) == "CRITICAL") {
            log_level = spdlog::level::critical;
        }
        else if (std::string(LOG_LEVEL) == "OFF") {
            log_level = spdlog::level::off;
        }
#endif

        // Create sinks: console + file
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_file, truncate);

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
        spdlog::set_level(log_level);
        spdlog::flush_on(spdlog::level::info);

        // --- CPU logger (file only) ---
        static auto cpu_logger = std::make_shared<spdlog::logger>("cpu_file_only", file_sink);
        cpu_logger->set_level(log_level);
        spdlog::register_logger(cpu_logger); // optional: register by name
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

// Get CPU logger (file only)
inline std::shared_ptr<spdlog::logger> cpu_logger()
{
    return spdlog::get("cpu_file_only");
}

// --- CPU-only trace (file only) ---
template <typename... Args>
inline void cpu_trace(const char* fmt, Args&&... args)
{
#ifdef BOYBOY_DEBUG
    cpu_logger()->trace(fmt::runtime(fmt), std::forward<Args>(args)...);
#else
    (void)fmt;
    (void)sizeof...(args);
#endif
}

} // namespace boyboy::log