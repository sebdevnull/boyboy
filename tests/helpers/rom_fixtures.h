/**
 * @file rom_fixtures.h
 * @brief Test fixtures for Cartridge ROM loading and parsing.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <gtest/gtest.h>

#include <cstdint>

#include "common/paths.h"
#include "helpers/cpu_fixtures.h"

// boyboy
#include "boyboy/cartridge.h"
#include "boyboy/cpu/opcodes.h"

namespace boyboy::test::rom {

struct ROMTest : public cpu::CpuTest {
    boyboy::cartridge::Cartridge cart;

    void SetUp() override
    {
        cpu::CpuTest::SetUp();
        cart.unload_rom(); // start with ROM unloaded
    }

    void load(const std::string& path)
    {
        // TODO: use project path
        // Temporary hack to use parent directory file path, until we figure out why project's path
        // behaves so weirdly
        cart.load_rom(common::local_file("../" + path, __FILE__));
        // cart.load_rom(boyboy::test::common::project_file(path));

        mmu->map_rom(cart);
    }

    void unload() { cart.unload_rom(); }

    void run()
    {
        if (!cart.is_loaded()) {
            throw std::runtime_error("ROM not loaded");
        }

        while (cpu.peek() != static_cast<uint8_t>(boyboy::cpu::Opcode::HALT)) {
            cpu.step();
        }
    }
};

} // namespace boyboy::test::rom