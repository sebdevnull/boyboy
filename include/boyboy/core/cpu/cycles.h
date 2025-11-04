/**
 * @file cycles.h
 * @brief CPU cycles definitions for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstdint>
#include <utility>

namespace boyboy::core::cpu {

// T-cycle type-alias
using TCycle = uint32_t;
// M-cycle type-alias
using MCycle = uint32_t;

// Cycle duration constants
constexpr TCycle FetchCycles = 4;             // Fetch duration in T-cycles
constexpr TCycle InterruptServiceCycles = 20; // Interrupt service duration in T-cycles

/**
 * @brief CPU ticking modes.
 *
 */
enum class TickMode : uint8_t {
    TCycle,      // Executes in T-cycle steps
    MCycle,      // Executes in M-cycle steps
    Instruction, // Executes a full instruction at a time
};

/**
 * @brief CPU cycles expressed in number of clocks or T-cycles.
 *
 */
enum class Cycles : uint8_t {
    TCycle = 1,
    MCycle = 4,
};

/**
 * @brief Converts a Cycles enum to T-cycles.
 *
 * @param c Cycles to be converted.
 * @return TCycle Number of T-cycles.
 */
constexpr TCycle to_tcycles(Cycles c)
{
    return std::to_underlying(c);
}

/**
 * @brief Converts M-cycles to T-cycles.
 *
 * @param cycles M-cycles to be converted.
 * @return TCycle Number of T-cycles.
 */
constexpr TCycle to_tcycles(MCycle cycles)
{
    return cycles * 4;
}

/**
 * @brief Convert T-cycles to M-cycles.
 *
 * @param cycles T-cycles to be converted.
 * @return MCycle Number of M-cycles.
 */
constexpr MCycle to_mcycles(TCycle cycles)
{
    return cycles / 4;
}

/**
 * @brief Converts a TickMode to its Cycles analog.
 *
 * For TickMode::Instruction returns Cycles::TCycle.
 *
 * @param mode TickMode to convert.
 * @return Cycles Converted Cycles.
 */
constexpr Cycles tickmode_to_cycles(TickMode mode)
{
    return (mode == TickMode::MCycle) ? Cycles::MCycle : Cycles::TCycle;
}

// String conversions

inline static const char* to_string(TickMode mode)
{
    switch (mode) {
        case TickMode::TCycle:
            return "T-Cycle";
        case TickMode::MCycle:
            return "M-Cycle";
        case TickMode::Instruction:
            return "Instruction";
        default:
            return "Unknown";
    }
}

} // namespace boyboy::core::cpu