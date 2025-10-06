/**
 * @file profiler.h
 * @brief Profiler utilities for the BoyBoy emulator.
 *
 * Provides interfaces and implementations for high-level profiling,
 * including manual and scoped timing, and integration with the Timer API.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <memory>
#include <string>

#include "boyboy/profiling/timer.h"

namespace boyboy::profiling {

/**
 * @brief Abstract interface for a high-level profiler.
 *
 * Provides methods for starting/stopping timers, scoped (RAII) timing,
 * reporting, and access to the underlying timer.
 */
class IProfiler {
public:
    IProfiler() = default;
    IProfiler(const IProfiler&) = delete;
    IProfiler& operator=(const IProfiler&) = delete;
    IProfiler(IProfiler&&) = delete;
    IProfiler& operator=(IProfiler&&) = delete;

    virtual ~IProfiler() = default;

    /**
     * @brief Start timing a named code section.
     * @param name Timer name.
     */
    virtual void start(const std::string& name) = 0;

    /**
     * @brief Stop timing a named code section.
     * @param name Timer name.
     */
    virtual void stop(const std::string& name) = 0;

    /**
     * @brief Create a scoped timer for a named code section.
     * @param name Timer name.
     * @return Unique pointer to a scoped timer (RAII).
     */
    virtual std::unique_ptr<IScopedTimer> scoped(const std::string& name) = 0;

    /**
     * @brief Output a profiling report for all timers.
     */
    virtual void report() = 0;

    /**
     * @brief Access the underlying timer implementation.
     * @return Reference to the timer.
     */
    virtual ITimer& timer() = 0;
};

/**
 * @brief Concrete profiler implementation.
 *
 * Wraps a Timer and provides high-level profiling API.
 */
class Profiler : public IProfiler {
public:
    void start(const std::string& name) override { timer_.start(name); }
    void stop(const std::string& name) override { timer_.stop(name); }

    std::unique_ptr<IScopedTimer> scoped(const std::string& name) override
    {
        return std::make_unique<ScopedTimer>(timer_, name);
    }

    void report() override { timer_.report(); }

    ITimer& timer() override { return timer_; }

private:
    Timer timer_;
};

/**
 * @brief Null profiler implementation (no-op).
 *
 * Used when profiling is disabled.
 */
class NullProfiler : public IProfiler {
public:
    void start(const std::string& /*name*/) override {}
    void stop(const std::string& /*name*/) override {}
    std::unique_ptr<IScopedTimer> scoped(const std::string& /*name*/) override
    {
        return std::make_unique<NullScopedTimer>(null_timer_, "");
    }
    void report() override {}
    ITimer& timer() override { return null_timer_; }

private:
    NullTimer null_timer_;
};

} // namespace boyboy::profiling