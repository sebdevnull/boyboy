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

#include "boyboy/common/utils.h"

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
        return std::format(
            "Opcode {} ({}) not implemented", utils::PrettyHex{opcode}.to_string(), mnemonic);
    }
};

class IllegalOpcode : public CpuException {
public:
    IllegalOpcode(uint8_t opcode) : CpuException(make_message(opcode)), opcode(opcode) {}
    uint8_t opcode;

private:
    static std::string make_message(uint8_t opcode)
    {
        return std::format("Illegal Opcode {} encountered", utils::PrettyHex{opcode}.to_string());
    }
};

class ChecksumError : public std::runtime_error {
public:
    explicit ChecksumError(const std::string& where, uint16_t expected, uint16_t actual)
        : std::runtime_error(make_message(where, expected, actual)), where(where),
          expected(expected), actual(actual)
    {
    }
    std::string where;
    uint16_t expected;
    uint16_t actual;

private:
    static std::string make_message(const std::string& where, uint16_t expected, uint16_t actual)
    {
        return std::format("Checksum error in {}: expected {}, got {}",
                           where,
                           utils::PrettyHex{expected}.to_string(),
                           utils::PrettyHex{actual}.to_string());
    }
};

} // namespace boyboy::errors