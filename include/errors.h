/**
 * @file errors.h
 * @brief Errors and exceptions for the BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <cstdint>
#include <format>
#include <stdexcept>

namespace boyboy::errors {

class CpuException : public std::runtime_error {
public:
    explicit CpuException(const std::string& msg) : std::runtime_error(msg) {}
};

class UnimplementedOpcode : public CpuException {
public:
    UnimplementedOpcode(uint8_t opcode, const std::string& mnemonic)
        : CpuException(make_message(opcode, mnemonic)), opcode(opcode), mnemonic(mnemonic)
    {
    }

    uint8_t opcode;
    std::string mnemonic;

private:
    static std::string make_message(uint8_t opcode, const std::string& mnemonic)
    {
        return std::format("Opcode 0x{:02X} ({}) not implemented", opcode, mnemonic);
    }
};

} // namespace boyboy::errors