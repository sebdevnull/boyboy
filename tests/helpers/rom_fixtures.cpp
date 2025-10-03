/**
 * @file rom_fixtures.cpp
 * @brief Test fixtures for Cartridge ROM loading and parsing.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "helpers/rom_fixtures.h"

#include <cstddef>
#include <memory>
#include <stdexcept>

#include "boyboy/cart/cartridge.h"
#include "boyboy/cart/cartridge_loader.h"
#include "boyboy/cart/mbc.h"
#include "common/paths.h"

// boyboy
#include "boyboy/common/errors.h"
#include "boyboy/common/utils.h"
#include "boyboy/log/logging.h"

namespace boyboy::test::rom {

std::string SerialTestCapturer::get_output()
{
    has_new_data_ = false;
    return buffer_.str();
}

void SerialTestCapturer::clear()
{
    buffer_.str("");
    buffer_.clear();
    has_new_data_ = false;
}

void SerialTestCapturer::set_new_line_callback(NewLineCB cb) { new_line_cb_ = std::move(cb); }

void SerialTestCapturer::write_hook(uint16_t addr, uint8_t value)
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

void ROMTest::SetUp()
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

void ROMTest::TearDown()
{
    cart.unload_rom();
    cpu::CpuTest::TearDown();
}

void ROMTest::load(const std::string& path)
{
    try {
        // TODO: use project path
        // Temporary hack to use parent directory file path, until we figure out why project's
        // path behaves so weirdly
        cart = boyboy::cart::CartridgeLoader::load(common::local_file("../" + path, __FILE__));
        // cart = boyboy::cart::CartridgeLoader::load(boyboy::test::common::project_file(path));
    }
    catch (const errors::ChecksumError& e) {
        log::warn("[ROM] Error loading ROM {}: {}", path, e.what());
    }

    if (!cart.is_loaded()) {
        throw std::runtime_error("Failed to load ROM: " + path);
    }

    mmu->map_rom(cart);
}

void ROMTest::unload() { cart.unload_rom(); }

void ROMTest::run()
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

void ROMTest::serial_new_line(const std::string& line)
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

cart::RomData FakeROMTest::make_fake_rom(cart::CartridgeType type,
                                         uint16_t rom_banks,
                                         uint8_t ram_banks,
                                         std::string title)
{
    // Minimum 2 rom_banks (no banking)
    rom_banks = std::max(rom_banks, uint16_t{2});

    cart::RomData rom_data(static_cast<long>(rom_banks) * cart::mbc::RomBankSize);

    // Fill each bank with its index
    for (long i = 0; i < rom_banks; i++) {
        std::fill(rom_data.begin() + i * cart::mbc::RomBankSize,
                  rom_data.begin() + (i + 1) * cart::mbc::RomBankSize,
                  std::byte(i));
    }

    // Header setup
    rom_data.at(cart::Cartridge::Header::CartridgeTypePos) = std::byte(type);
    rom_data.at(cart::Cartridge::Header::ROMSizePos) =
        std::byte(cart::rom_size_from_banks(rom_banks));
    rom_data.at(cart::Cartridge::Header::RAMSizePos) =
        std::byte(cart::ram_size_from_banks(ram_banks));

    // Title (max 16 chars)
    if (title.empty()) {
        title = "FAKE_ROM";
    }
    title = title.substr(0, 16);
    for (size_t i = 0; i < title.size(); i++) {
        rom_data.at(cart::Cartridge::Header::TitlePos + i) = std::byte(title[i]);
    }
    for (size_t i = title.size(); i < 16; i++) {
        rom_data.at(cart::Cartridge::Header::TitlePos + i) = std::byte(0);
    }

    // Header checksum
    uint8_t cks = cart::Cartridge::header_checksum(rom_data);
    rom_data.at(cart::Cartridge::Header::HeaderChecksumPos) = std::byte(cks);

    return rom_data;
}

void MBCParamTest::SetUp()
{
    auto p   = GetParam();
    rom_data = make_fake_rom(p.type, p.rom_banks, p.ram_banks, p.name);
    cart     = std::make_unique<cart::Cartridge>(cart::CartridgeLoader::load(rom_data));
}

} // namespace boyboy::test::rom