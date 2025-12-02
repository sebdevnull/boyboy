/**
 * @file global_tick_mode.h
 * @brief Global tick mode for tests.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include "boyboy/core/cpu/cycles.h"

namespace boyboy::tests {

// NOLINTNEXTLINE
inline boyboy::core::cpu::TickMode g_tick_mode = boyboy::core::cpu::TickMode::MCycle;

} // namespace boyboy::tests