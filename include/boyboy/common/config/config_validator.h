/**
 * @file config_validator.h
 * @brief Configuration validator for BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <string>
#include <vector>

namespace boyboy::common::config {

// Forward declaration
struct Config;

struct ValidationResult {
    bool valid = true;
    std::vector<std::string> warnings;
    std::vector<std::string> errors;
};

class ConfigValidator {
public:
    static ValidationResult validate(Config& config, bool normalize = false);
    static void check_result(const ValidationResult& result);

private:
    template <typename Limit, typename Value>
    static void validate_field(
        ValidationResult& result,
        Value& field,
        const Limit& limit,
        std::string_view name,
        bool normalize = false
    );
};

} // namespace boyboy::common::config