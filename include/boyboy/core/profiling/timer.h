/**
 * @file timer.h
 * @brief Timer utility for profiling code execution time.
 *
 * Provides interfaces and implementations for timing named code sections,
 * supporting both manual and RAII-based (scoped) timing.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "boyboy/common/log/logging.h"

namespace boyboy::core::profiling {

/**
 * @brief Abstract interface for a profiling timer.
 */
class ITimer {
public:
    ITimer() = default;
    ITimer(const ITimer&) = delete;
    ITimer& operator=(const ITimer&) = delete;
    ITimer(ITimer&&) = delete;
    ITimer& operator=(ITimer&&) = delete;
    virtual ~ITimer() = default;

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
     * @brief Output a profiling report for all timers.
     */
    virtual void report() = 0;

    /**
     * @brief Get the total accumulated time for a named section (in microseconds).
     * @param name Timer name.
     * @return Total time in microseconds.
     */
    [[nodiscard]] virtual uint64_t get_time_us(const std::string& name) const = 0;

    /**
     * @brief Get the number of times a named section was timed.
     * @param name Timer name.
     * @return Call count.
     */
    [[nodiscard]] virtual uint64_t get_call_count(const std::string& name) const = 0;
};

/**
 * @brief Concrete timer implementation for profiling named code sections.
 *
 * Supports manual start/stop as well as RAII-based (scoped) timing.
 * Reports all timers in alphabetical order.
 */
class Timer : public ITimer {
public:
    void start(const std::string& name) override
    {
        start_times_[name] = std::chrono::high_resolution_clock::now();
    }

    void stop(const std::string& name) override
    {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto it = start_times_.find(name);
        if (it != start_times_.end()) {
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
                end_time - it->second
            );
            accumulated_times_[name] += duration.count();
            call_counts_[name]++;
            start_times_.erase(it);
        }
    }

    /**
     * @brief Output a profiling report for all timers in alphabetical order.
     *
     * After reporting, all accumulated data is cleared.
     */
    void report() override
    {
        // Collect and sort keys in alphabetical order
        std::vector<std::string> names;
        names.reserve(accumulated_times_.size());
        for (const auto& [name, _] : accumulated_times_) {
            names.push_back(name);
        }
        std::ranges::sort(names);

        common::log::info("----- Profiler Report -----");
        for (const auto& name : names) {
            uint64_t total_time = accumulated_times_[name];
            uint64_t count = call_counts_[name];
            double avg_time = static_cast<double>(total_time) / static_cast<double>(count);
            common::log::info(
                "[{}]: total={}us, calls={}, avg={:.2f}us", name, total_time, count, avg_time
            );
        }
        common::log::info("---------------------------");

        start_times_.clear();
        accumulated_times_.clear();
        call_counts_.clear();
    }

    [[nodiscard]] uint64_t get_time_us(const std::string& name) const override
    {
        auto it = accumulated_times_.find(name);
        if (it != accumulated_times_.end()) {
            return it->second;
        }
        return 0;
    }

    [[nodiscard]] uint64_t get_call_count(const std::string& name) const override
    {
        auto it = call_counts_.find(name);
        if (it != call_counts_.end()) {
            return it->second;
        }
        return 0;
    }

private:
    std::unordered_map<std::string, std::chrono::high_resolution_clock::time_point> start_times_;
    std::unordered_map<std::string, uint64_t> accumulated_times_;
    std::unordered_map<std::string, uint64_t> call_counts_;
};

/**
 * @brief Null timer implementation (no-op).
 *
 * Used when profiling is disabled.
 */
class NullTimer : public ITimer {
public:
    void start(const std::string& /*name*/) override {}
    void stop(const std::string& /*name*/) override {}
    void report() override {}
    [[nodiscard]] uint64_t get_time_us(const std::string& /*name*/) const override { return 0; }
    [[nodiscard]] uint64_t get_call_count(const std::string& /*name*/) const override { return 0; }
};

/**
 * @brief Abstract interface for a scoped timer (RAII).
 */
class IScopedTimer {
public:
    IScopedTimer() = default;
    IScopedTimer(const IScopedTimer&) = delete;
    IScopedTimer& operator=(const IScopedTimer&) = delete;
    IScopedTimer(IScopedTimer&&) = delete;
    IScopedTimer& operator=(IScopedTimer&&) = delete;
    virtual ~IScopedTimer() = default;
};

/**
 * @brief RAII-based scoped timer.
 *
 * Starts timing on construction and stops on destruction.
 */
class ScopedTimer : public IScopedTimer {
public:
    ScopedTimer(ITimer& timer, std::string name) : timer_(timer), name_(std::move(name))
    {
        timer_.start(name_);
    }
    ~ScopedTimer() override { timer_.stop(name_); }

    ScopedTimer(const ScopedTimer&) = delete;
    ScopedTimer& operator=(const ScopedTimer&) = delete;
    ScopedTimer(ScopedTimer&&) = delete;
    ScopedTimer& operator=(ScopedTimer&&) = delete;

private:
    ITimer& timer_;
    std::string name_;
};

/**
 * @brief Null scoped timer (no-op).
 *
 * Used when profiling is disabled.
 */
class NullScopedTimer : public IScopedTimer {
public:
    NullScopedTimer(ITimer& /*timer*/, const std::string& /*name*/) {}
};

} // namespace boyboy::core::profiling