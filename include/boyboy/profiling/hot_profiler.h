/**
 * @file hot_profiler.h
 * @brief High-performance profiler for hot code sections.
 *
 * Provides a lightweight, allocation-free profiler for measuring execution time and call counts of
 * fixed, high-frequency code sections.
 * Uses enum-based section IDs for fast lookup and reporting.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>

#include "boyboy/log/logging.h"

namespace boyboy::profiling {

/**
 * @brief Enum for identifying hot code sections to be profiled.
 *
 * Each value corresponds to a specific subsystem or operation.
 */
enum class HotSection : uint8_t {
    CpuFetch,
    CpuExecute,
    MmuRead,
    MmuWrite,
    MmuLookup,
    Count,
};

/**
 * @brief Convert a HotSection enum value to a string.
 * @param section HotSection value.
 * @return C-string representation of the section name.
 */
inline const char* to_string(HotSection section)
{
    switch (section) {
        case HotSection::CpuFetch:
            return "Cpu::fetch";
        case HotSection::CpuExecute:
            return "Cpu::execute";
        case HotSection::MmuLookup:
            return "Mmu::lookup";
        case HotSection::MmuRead:
            return "Mmu::read";
        case HotSection::MmuWrite:
            return "Mmu::write";
        default:
            return "Unknown";
    }
}

/**
 * @brief High-performance profiler for hot code sections.
 *
 * Tracks timing and call counts for a fixed set of code sections,
 * using fast array-based storage indexed by HotSection.
 */
class HotProfiler {
public:
    using clock = std::chrono::high_resolution_clock;
    using duration = std::chrono::nanoseconds;
    using time_point = std::chrono::time_point<clock>;

    /**
     * @brief Start timing a hot section.
     * @param section The HotSection to start timing.
     */
    void start(HotSection section) { start_times_[static_cast<size_t>(section)] = clock::now(); }

    /**
     * @brief Stop timing a hot section and accumulate the elapsed time.
     * @param section The HotSection to stop timing.
     */
    void stop(HotSection section)
    {
        auto idx = static_cast<size_t>(section);
        auto now = clock::now();
        auto elapsed = std::chrono::duration_cast<duration>(now - start_times_[idx]);
        accumulated_times_[idx] += elapsed;
        call_counts_[idx]++;
    }

    /**
     * @brief Output a profiling report for all hot sections.
     *
     * Prints total time, call count, and average time per call for each section.
     */
    void report()
    {
        log::info("----- Hot Profiler Report -----");
        for (size_t i = 0; i < static_cast<size_t>(HotSection::Count); ++i) {
            if (call_counts_[i] == 0) {
                continue;
            }
            auto avg_time = static_cast<double>(accumulated_times_[i].count()) /
                            static_cast<double>(call_counts_[i]);
            log::info(
                "[{}]: total={}ns, calls={}, avg={:.2f}ns",
                to_string(static_cast<HotSection>(i)),
                accumulated_times_[i].count(),
                call_counts_[i],
                avg_time
            );
        }
        log::info("-------------------------------");
    }

private:
    std::array<time_point, static_cast<size_t>(HotSection::Count)> start_times_{};
    std::array<duration, static_cast<size_t>(HotSection::Count)> accumulated_times_{};
    std::array<uint64_t, static_cast<size_t>(HotSection::Count)> call_counts_{};
};

} // namespace boyboy::profiling