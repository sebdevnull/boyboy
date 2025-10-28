/**
 * @file state.h
 * @brief CPU execution state for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <utility>

#include "boyboy/core/cpu/cycles.h"
#include "boyboy/core/cpu/instructions.h"

namespace boyboy::core::cpu {

enum class Stage : uint8_t {
    None = 0,
    Fetch = 1 << 0,
    CBInstruction = 1 << 1,
    Execute = 1 << 2,
    InterruptService = 1 << 3,
};

constexpr Stage operator|(Stage lhs, Stage rhs) noexcept
{
    return static_cast<Stage>(std::to_underlying(lhs) | std::to_underlying(rhs));
}

constexpr Stage operator&(Stage lhs, Stage rhs) noexcept
{
    return static_cast<Stage>(std::to_underlying(lhs) & std::to_underlying(rhs));
}

constexpr Stage operator|=(Stage& lhs, Stage rhs) noexcept
{
    lhs = lhs | rhs;
    return lhs;
}

constexpr Stage operator&=(Stage& lhs, Stage rhs) noexcept
{
    lhs = lhs & rhs;
    return lhs;
}

constexpr bool has_flag(Stage value, Stage flag) noexcept
{
    return (value & flag) != Stage::None;
}

constexpr void clear_flag(Stage& value, Stage flag) noexcept
{
    value &= static_cast<Stage>(~std::to_underlying(flag));
}

struct ExecutionState {
    Stage stage{Stage::Fetch};
    uint8_t fetched{0};
    TCycle cycles_left{FetchCycles};
    const Instruction* instr = nullptr;

    void init()
    {
        stage = Stage::Fetch;
        cycles_left = FetchCycles;
        fetched = 0;
        instr = nullptr;
    }

    void reset() { init(); }

    [[nodiscard]] bool has_stage(Stage flag) const { return has_flag(stage, flag); }
};

} // namespace boyboy::core::cpu
