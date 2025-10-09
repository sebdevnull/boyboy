/**
 * @file frame_profiler.h
 * @brief Frame profiler for measuring FPS and performance metrics.
 *
 * Provides types and a class for collecting and reporting frame-based statistics,
 * such as FPS, IPS, CPS, and per-component timing.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <array>
#include <chrono>
#include <cstddef>
#include <format>
#include <functional>
#include <optional>
#include <string>

#include "boyboy/common/log/logging.h"

namespace boyboy::core::profiling {

/**
 * @brief Enum for core frame timing components.
 */
enum class FrameTimer : uint8_t { Cpu, Ppu, Render, Count };

/**
 * @brief Convert FrameTimer enum to string.
 * @param timer FrameTimer value.
 * @return String representation.
 */
inline std::string to_string(FrameTimer timer)
{
    switch (timer) {
        case FrameTimer::Cpu:
            return "Cpu";
        case FrameTimer::Ppu:
            return "Ppu";
        case FrameTimer::Render:
            return "Render";
        default:
            return "Unknown";
    }
}

/**
 * @brief Array of per-component timing values (in microseconds).
 */
using FrameTimes = std::array<uint64_t, static_cast<size_t>(FrameTimer::Count)>;

/**
 * @brief Aggregate two FrameTimes arrays element-wise.
 * @param lhs Left-hand side (accumulated).
 * @param rhs Right-hand side (to add).
 * @return Reference to lhs.
 */
inline FrameTimes& operator+=(FrameTimes& lhs, const FrameTimes& rhs)
{
    std::ranges::transform(lhs, rhs, lhs.begin(), std::plus<>{});
    return lhs;
}

/**
 * @brief Per-frame data: instruction/cycle counts and optional timing.
 */
struct FrameData {
    uint64_t instruction_count{0};
    uint64_t cycle_count{0};
    std::optional<FrameTimes> times_us;

    /**
     * @brief Aggregate another FrameData into this one.
     * @param other FrameData to add.
     * @return Reference to this.
     */
    FrameData& operator+=(const FrameData& other)
    {
        instruction_count += other.instruction_count;
        cycle_count += other.cycle_count;
        if (other.times_us) {
            if (!times_us) {
                times_us.emplace();
            }
            *times_us += *other.times_us;
        }
        return *this;
    }

    /**
     * @brief Reset all counts to zero (and timing if present).
     */
    void reset()
    {
        instruction_count = 0;
        cycle_count = 0;
        if (times_us) {
            times_us.emplace();
        }
    }
};

/**
 * @brief Accumulated stats for a set of frames (interval or total).
 */
struct RunningStats {
    uint64_t frame_count{0};
    FrameData frame_data;

    /**
     * @brief Aggregate another RunningStats into this one.
     * @param other Stats to add.
     * @return Reference to this.
     */
    RunningStats& operator+=(const RunningStats& other)
    {
        frame_count += other.frame_count;
        frame_data += other.frame_data;
        return *this;
    }

    /**
     * @brief Reset all stats to zero.
     */
    void reset()
    {
        frame_count = 0;
        frame_data.reset();
    }
};

/**
 * @brief FrameProfiler collects and reports frame-based statistics.
 *
 * Tracks FPS, IPS, CPS, and per-component timing, with periodic and total reporting.
 */
class FrameProfiler {
public:
    /**
     * @brief Construct a FrameProfiler.
     * @param log_interval Interval (in seconds) for periodic logging.
     */
    FrameProfiler(double log_interval = 1.0)
        : log_interval_(log_interval),
          last_log_time_(std::chrono::high_resolution_clock::now()),
          start_time_(last_log_time_)
    {
    }

    /**
     * @brief Get total accumulated stats.
     * @return Reference to total RunningStats.
     */
    [[nodiscard]] const RunningStats& total_stats() const { return total_stats_; }

    /**
     * @brief Set the periodic log interval (in seconds).
     * @param interval Log interval in seconds.
     */
    void set_log_interval(double interval) { log_interval_ = interval; }

    /**
     * @brief Record a new frame's data.
     * @param frame_data Data for the frame.
     */
    void record_frame(const FrameData& frame_data)
    {
        frame_stats_.frame_count++;
        frame_stats_.frame_data += frame_data;

        auto now = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(now - last_log_time_).count();

        if (elapsed > log_interval_) {
            log_frame(elapsed);
            last_log_time_ = now;
            flush();
        }
    }

    /**
     * @brief Output a final report of all accumulated stats.
     *
     * Flushes any pending interval data before reporting.
     */
    void report()
    {
        flush(); // Ensure all interval data is included

        auto now = std::chrono::high_resolution_clock::now();
        double total_elapsed = std::chrono::duration<double>(now - start_time_).count();

        const auto& frame_data = total_stats_.frame_data;

        double avg_fps = static_cast<double>(total_stats_.frame_count) / total_elapsed;
        double avg_ips = static_cast<double>(frame_data.instruction_count) / total_elapsed;
        double avg_cps = static_cast<double>(frame_data.cycle_count) / total_elapsed;

        std::string log_msg = std::format(
            "Frames: {} | Avg FPS: {:.1f} | Avg IPS: {:.1f}k | Avg CPS: {:.1f}k",
            total_stats_.frame_count,
            avg_fps,
            avg_ips / 1e3,
            avg_cps / 1e3
        );

        // Component timing averaged per frame
        if (frame_data.times_us) {
            for (size_t i = 0; i < static_cast<size_t>(FrameTimer::Count); ++i) {
                double avg_time = static_cast<double>((*frame_data.times_us).at(i)) /
                                  static_cast<double>(total_stats_.frame_count);
                log_msg += std::format(
                    " | Avg {}: {:.1f}us", to_string(static_cast<FrameTimer>(i)), avg_time
                );
            }
        }

        common::log::info("----- Frame Profiler Report -----");
        common::log::info("{}", log_msg);
        common::log::info("---------------------------------");
    }

    /**
     * @brief Flush current interval stats into totals and reset the interval.
     */
    void flush()
    {
        total_stats_ += frame_stats_;
        frame_stats_.reset();
    }

private:
    double log_interval_;
    std::chrono::high_resolution_clock::time_point last_log_time_;
    std::chrono::high_resolution_clock::time_point start_time_;

    RunningStats frame_stats_{};
    RunningStats total_stats_{};

    /**
     * @brief Log the current interval's stats.
     * @param elapsed Elapsed time (in seconds) since last log.
     */
    void log_frame(double elapsed) const
    {
        const auto& frame_data = frame_stats_.frame_data;
        double fps = static_cast<double>(frame_stats_.frame_count) / elapsed;
        double ips = static_cast<double>(frame_data.instruction_count) / elapsed;
        double cps = static_cast<double>(frame_data.cycle_count) / elapsed;

        // Compose log message
        std::string log_msg = std::format(
            "FPS: {:.1f} | IPS: {:.1f}k | CPS: {:.1f}k", fps, ips / 1e3, cps / 1e3
        );

        // Compose timing breakdown if available
        if (frame_data.times_us) {
            for (size_t i = 0; i < static_cast<size_t>(FrameTimer::Count); ++i) {
                double avg_time = static_cast<double>((*frame_data.times_us).at(i)) /
                                  static_cast<double>(frame_stats_.frame_count);
                log_msg += std::format(
                    " | {}: {:.1f}us", to_string(static_cast<FrameTimer>(i)), avg_time
                );
            }
        }

        common::log::info("{}", log_msg);
    }
};

} // namespace boyboy::core::profiling