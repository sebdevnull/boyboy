/**
 * @file rom_fixtures.h
 * @brief Test fixtures for Cartridge ROM loading and parsing.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <gtest/gtest.h>

#include <cstdint>
#include <functional>
#include <stdexcept>
#include <vector>

#include "common/paths.h"
#include "helpers/cpu_fixtures.h"

// boyboy
#include "boyboy/cartridge.h"
#include "boyboy/common/errors.h"
#include "boyboy/common/utils.h"
#include "boyboy/io/io.h"
#include "boyboy/log/logging.h"

namespace boyboy::test::rom {

class SerialTestCapturer {
public:
    [[nodiscard]] bool has_new_data() const { return has_new_data_; }

    std::string get_output()
    {
        has_new_data_ = false;
        return buffer_.str();
    }

    void clear()
    {
        buffer_.str("");
        buffer_.clear();
        has_new_data_ = false;
    }

    void set_new_line_callback(std::function<void(std::string)> cb)
    {
        new_line_cb_ = std::move(cb);
    }

    void write_hook(uint16_t addr, uint8_t value)
    {
        if (addr == io::IoReg::Serial::SB) {
            buffer_ << static_cast<char>(value);
            has_new_data_ = true;
            if (value == '\n' && new_line_cb_) {
                new_line_cb_(buffer_.str());
                clear();
            }
        }
    }

private:
    std::ostringstream buffer_;
    bool has_new_data_                            = false;
    std::function<void(std::string)> new_line_cb_ = nullptr;
};

struct ROMTest : public cpu::CpuTest {
protected:
    const int LoopThreshold = 10000; // Arbitrary large number to prevent infinite loops

    boyboy::cartridge::Cartridge cart;
    boyboy::io::Io* io;

    SerialTestCapturer serial_capturer;
    std::string serial_output;
    bool finished = false;
    bool passed   = false;

    const std::vector<std::string> TestStates = {"Passed", "Done", "Failed"};

    void SetUp() override
    {
        // We don't call CpuTest::SetUp() because we want "real" CPU initial state
        // (e.g. PC=0x0100, SP=0xFFFE, etc)

        cpu.reset();
        mmu->reset();

        io = &mmu->io();
        cart.unload_rom();
        serial_capturer.clear();
        serial_output.clear();
        finished = false;
        passed   = false;

        serial_capturer.set_new_line_callback(
            [this](const std::string& line) { serial_new_line(line); });

        mmu->set_io_write_callback(
            [this](uint16_t addr, uint8_t value) { serial_capturer.write_hook(addr, value); });
    }

    void TearDown() override
    {
        cart.unload_rom();
        cpu::CpuTest::TearDown();
    }

    void load(const std::string& path)
    {
        try {
            // TODO: use project path
            // Temporary hack to use parent directory file path, until we figure out why project's
            // path behaves so weirdly
            cart.load_rom(common::local_file("../" + path, __FILE__));
            // cart.load_rom(boyboy::test::common::project_file(path));
        }
        catch (const errors::ChecksumError& e) {
            log::warn("[ROM] Error loading ROM {}: {}", path, e.what());
        }

        if (!cart.is_loaded()) {
            throw std::runtime_error("Failed to load ROM: " + path);
        }

        mmu->map_rom(cart);
    }

    void unload() { cart.unload_rom(); }

    void run()
    {
        if (!cart.is_loaded()) {
            throw std::runtime_error("ROM not loaded");
        }

        uint16_t last_pc = 0;
        int repeat_count = 0;
        uint8_t cycles   = 0;

        // Run until finished or potential infinite loop detected
        while (!finished) {
            uint16_t current_pc = cpu.get_pc();
            if (current_pc == last_pc) {
                repeat_count++;
                if (repeat_count >= LoopThreshold) {
                    log::warn("[ROM] Detected potential infinite loop at PC={}: {}",
                              utils::PrettyHex(current_pc).to_string(),
                              cpu.disassemble(current_pc));
                    break;
                }
            }
            else {
                repeat_count = 0;
            }

            last_pc = current_pc;

            cycles = cpu.step();
            io->serial().tick(cycles); // no need to tick all io components
        }

        if (serial_output.empty() && serial_capturer.has_new_data()) {
            serial_output = serial_capturer.get_output();
            log::warn("[ROM] Empty serial output, dumping captured data:\n{}", serial_output);
        }

        EXPECT_TRUE(passed) << "ROM test did not pass:\n"
                            << "Last PC=" << utils::PrettyHex(last_pc) << "\n"
                            << "Last Instruction=" << cpu.disassemble(last_pc) << "\n"
                            << "Serial Output:\n"
                            << serial_output;
    }

    void serial_new_line(const std::string& line)
    {
        if (line.empty() || line == "\n") {
            return;
        }

        serial_output += line;
        log::info("[ROM] Serial Output new line received: {}",
                  line.substr(0, line.size() - 1)); // exclude newline character

        for (const auto& state : TestStates) {
            if (line.find(state) != std::string::npos) {
                finished = true;
                passed   = (state == "Passed");
                log::info("[ROM] Detected ROM test state: {}", state);
                break;
            }
        }
    }
};

} // namespace boyboy::test::rom