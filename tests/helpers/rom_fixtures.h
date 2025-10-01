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
#include <vector>

#include "helpers/cpu_fixtures.h"

// boyboy
#include "boyboy/cart/cartridge.h"
#include "boyboy/cart/cartridge_loader.h"
#include "boyboy/io/io.h"

namespace boyboy::test::rom {

class SerialTestCapturer {
public:
    using NewLineCB = std::function<void(std::string)>;

    [[nodiscard]] bool has_new_data() const { return has_new_data_; }
    std::string get_output();
    void clear();
    void set_new_line_callback(NewLineCB cb);
    void write_hook(uint16_t addr, uint8_t value);

private:
    std::ostringstream buffer_;
    bool has_new_data_     = false;
    NewLineCB new_line_cb_ = nullptr;
};

struct ROMTest : public cpu::CpuTest {
protected:
    static constexpr int LoopThreshold = 10000; // Arbitrary large number to prevent infinite loops

    boyboy::cart::Cartridge cart;
    boyboy::io::Io* io;

    SerialTestCapturer serial_capturer;
    std::string serial_output;
    bool finished = false;
    bool passed   = false;

    const std::vector<std::string> TestStates = {"Passed", "Done", "Failed"};

    void SetUp() override;
    void TearDown() override;

    void load(const std::string& path);
    void unload();
    void run();
    void serial_new_line(const std::string& line);
};

} // namespace boyboy::test::rom