/**
 * @file logging.h
 * @brief Logging utilities for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

// TODO: add module-specific loggers (e.g., CPU, PPU, MMU)
// TODO: improve logging format, add more context

#pragma once

#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <iostream>
#include <memory>
#include <string>

namespace boyboy::common::log {

enum class LogLevel : uint8_t { Trace, Debug, Info, Warn, Error, Critical, Off };

inline LogLevel log_level_from_string(const std::string& level_str)
{
    if (level_str == "trace") {
        return LogLevel::Trace;
    }
    if (level_str == "debug") {
        return LogLevel::Debug;
    }
    if (level_str == "info") {
        return LogLevel::Info;
    }
    if (level_str == "warn") {
        return LogLevel::Warn;
    }
    if (level_str == "error") {
        return LogLevel::Error;
    }
    if (level_str == "critical") {
        return LogLevel::Critical;
    }
    if (level_str == "off") {
        return LogLevel::Off;
    }
    return LogLevel::Info; // default
}

inline std::string log_level_to_string(LogLevel level)
{
    switch (level) {
        case LogLevel::Trace:
            return "trace";
        case LogLevel::Debug:
            return "debug";
        case LogLevel::Info:
            return "info";
        case LogLevel::Warn:
            return "warn";
        case LogLevel::Error:
            return "error";
        case LogLevel::Critical:
            return "critical";
        case LogLevel::Off:
            return "off";
        default:
            return "info";
    }
}

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
        auto cpu_file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
            "logs/cpu.log", truncate
        );

        if (async) {
            // Thread pool: queue size 8192, 1 background thread
            spdlog::init_thread_pool(8192, 1);
            logger = std::make_shared<spdlog::async_logger>(
                "boyboy",
                spdlog::sinks_init_list{console_sink, file_sink},
                spdlog::thread_pool(),
                spdlog::async_overflow_policy::block
            );
        }
        else {
            logger = std::make_shared<spdlog::logger>(
                "boyboy", spdlog::sinks_init_list{console_sink, file_sink}
            );
        }

        spdlog::set_default_logger(logger);
        spdlog::set_level(log_level);
        spdlog::flush_on(spdlog::level::info);

        // --- CPU logger (file only) ---
        static auto cpu_logger = std::make_shared<spdlog::logger>("cpu", cpu_file_sink);
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

inline void set_level(LogLevel level)
{
    switch (level) {
        case LogLevel::Trace:
            spdlog::set_level(spdlog::level::trace);
            break;
        case LogLevel::Debug:
            spdlog::set_level(spdlog::level::debug);
            break;
        case LogLevel::Info:
            spdlog::set_level(spdlog::level::info);
            break;
        case LogLevel::Warn:
            spdlog::set_level(spdlog::level::warn);
            break;
        case LogLevel::Error:
            spdlog::set_level(spdlog::level::err);
            break;
        case LogLevel::Critical:
            spdlog::set_level(spdlog::level::critical);
            break;
        case LogLevel::Off:
            spdlog::set_level(spdlog::level::off);
            break;
        default:
            spdlog::set_level(spdlog::level::info);
            break;
    }
}

inline void set_level(const std::string& level_str)
{
    set_level(log_level_from_string(level_str));
}

// Shutdown logging (flushes and cleans up)
inline void shutdown()
{
    spdlog::shutdown();
}

// Get CPU logger (file only)
inline std::shared_ptr<spdlog::logger> cpu_logger()
{
    return spdlog::get("cpu");
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

} // namespace boyboy::common::log