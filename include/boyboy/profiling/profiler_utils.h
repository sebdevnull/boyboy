/**
 * @file profiler_utils.h
 * @brief Profiler utilities API for the BoyBoy emulator.
 *
 * Provides macros and inline functions for easy, conditional profiling and frame statistics.
 * When ENABLE_PROFILING is not defined, all macros/functions become no-ops except for frame stats.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once
#include "boyboy/profiling/frame_profiler.h"
#include "boyboy/profiling/profiler.h"

namespace boyboy::profiling {

// NOLINTBEGIN(cppcoreguidelines-macro-usage)

/**
 * @defgroup ProfilingMacros Profiling Macros
 * @brief Macros for ergonomic profiling instrumentation.
 * @{
 */

#ifdef ENABLE_PROFILING
using ActiveProfiler = Profiler;

/// Concatenate two tokens (internal macro).
#define BB_CONCAT_IMPL(x, y) x##y
/// Concatenate two tokens (user macro).
#define BB_CONCAT(x, y) BB_CONCAT_IMPL(x, y)

/**
 * @brief Start a named profiling timer.
 * @param name Timer name (string or FrameTimer enum).
 */
#define BB_PROFILE_START(name) boyboy::profiling::profile_start(name)

/**
 * @brief Stop a named profiling timer.
 * @param name Timer name (string or FrameTimer enum).
 */
#define BB_PROFILE_STOP(name) boyboy::profiling::profile_stop(name)

/**
 * @brief Profile a scope using RAII. Automatically stops when leaving scope.
 * @param name Timer name (string or FrameTimer enum).
 */
#define BB_PROFILE_SCOPE(name)                                                                     \
    [[maybe_unused]] auto BB_CONCAT(_bb_profile_scope_, __LINE__) =                                \
        boyboy::profiling::profile_scope(name)

/**
 * @brief Output a profiling report for all timers.
 */
#define BB_PROFILE_REPORT() boyboy::profiling::profile_report()
#else
using ActiveProfiler = NullProfiler;

#define BB_PROFILE_START(name)
#define BB_PROFILE_STOP(name)
#define BB_PROFILE_REPORT()
#define BB_PROFILE_SCOPE(name)
#endif

/**
 * @brief Record per-frame statistics (instructions, cycles, and optionally timing data).
 * @param instr Instruction count for the frame.
 * @param cycles Cycle count for the frame.
 */
#define BB_PROFILE_FRAME(instr, cycles) boyboy::profiling::profile_frame(instr, cycles)

/**
 * @brief Output a frame profiler report (FPS, IPS, CPS, etc).
 */
#define BB_FRAME_PROFILE_REPORT() boyboy::profiling::frame_profile_report()

/** @} */
// NOLINTEND(cppcoreguidelines-macro-usage)

/**
 * @brief Get the global profiler instance.
 * @return Reference to the active profiler (real or null).
 */
inline IProfiler& get_profiler()
{
    static ActiveProfiler profiler;
    return profiler;
}

/**
 * @brief Get the global frame profiler instance.
 * @return Reference to the frame profiler.
 */
inline FrameProfiler& get_frame_profiler()
{
    static FrameProfiler frame_profiler;
    return frame_profiler;
}

/**
 * @brief Start a named profiling timer (string).
 * @param name Timer name.
 */
inline void profile_start(const std::string& name)
{
    get_profiler().start(name);
}

/**
 * @brief Start a named profiling timer (enum).
 * @param timer FrameTimer enum value.
 */
inline void profile_start(FrameTimer timer)
{
    get_profiler().start(to_string(timer));
}

/**
 * @brief Stop a named profiling timer (string).
 * @param name Timer name.
 */
inline void profile_stop(const std::string& name)
{
    get_profiler().stop(name);
}

/**
 * @brief Stop a named profiling timer (enum).
 * @param timer FrameTimer enum value.
 */
inline void profile_stop(FrameTimer timer)
{
    get_profiler().stop(to_string(timer));
}

/**
 * @brief Profile a scope using RAII (string).
 * @param name Timer name.
 * @return RAII object that stops the timer on destruction.
 */
inline auto profile_scope(const std::string& name)
{
    return get_profiler().scoped(name);
}

/**
 * @brief Profile a scope using RAII (enum).
 * @param timer FrameTimer enum value.
 * @return RAII object that stops the timer on destruction.
 */
inline auto profile_scope(FrameTimer timer)
{
    return get_profiler().scoped(to_string(timer));
}

/**
 * @brief Record per-frame statistics.
 *
 * Always records instruction and cycle counts. If profiling is enabled,
 * also records per-frame timing deltas for each FrameTimer.
 *
 * @param instructions Instruction count for the frame.
 * @param cycles Cycle count for the frame.
 */
inline void profile_frame(uint64_t instructions, uint64_t cycles)
{
    FrameData frame_data{};
    frame_data.instruction_count = instructions;
    frame_data.cycle_count = cycles;

#ifdef ENABLE_PROFILING
    // Store last times to calculate deltas to pass to FrameProfiler
    static FrameTimes last_times{};

    FrameTimes current_times;
    for (size_t i = 0; i < static_cast<size_t>(FrameTimer::Count); ++i) {
        current_times.at(i) =
            get_profiler().timer().get_time_us(to_string(static_cast<FrameTimer>(i)));
    }

    FrameTimes deltas;
    for (size_t i = 0; i < static_cast<size_t>(FrameTimer::Count); ++i) {
        deltas.at(i) = current_times.at(i) - last_times.at(i);
        last_times.at(i) = current_times.at(i);
    }
    frame_data.times_us = deltas;
#endif

    get_frame_profiler().record_frame(frame_data);
}

/**
 * @brief Output a profiling report for all timers.
 */
inline void profile_report()
{
    get_profiler().report();
}

/**
 * @brief Output a frame profiler report (FPS, IPS, CPS, etc).
 */
inline void frame_profile_report()
{
    get_frame_profiler().report();
}

} // namespace boyboy::profiling