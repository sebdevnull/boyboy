/**
 * @file config_validator.cpp
 * @brief Configuration validator for BoyBoy emulator.
 *
 * @license GPLv3 (see LICENSE file)
 */

#include "boyboy/common/config/config_validator.h"

#include <format>

#include "boyboy/common/config/config.h"
#include "boyboy/common/config/config_limits.h"

namespace boyboy::common::config {

ValidationResult ConfigValidator::validate(Config& config, bool normalize)
{
    ValidationResult result{};

    validate_field(
        result,
        config.emulator.speed,
        ConfigLimits::Emulator::SpeedRange,
        "emulator.speed",
        normalize
    );

    validate_field(
        result, config.video.scale, ConfigLimits::Video::ScaleRange, "video.scale", normalize
    );

    validate_field(
        result,
        config.debug.log_level,
        ConfigLimits::Debug::LogLevelOptions,
        "debug.log_level",
        normalize
    );

    return result;
}

template <typename Limit, typename Value>
void ConfigValidator::validate_field(
    ValidationResult& result,
    Value& field,
    const Limit& limit,
    std::string_view name,
    bool normalize
)
{
    if constexpr (requires { limit.in_range(field); }) {
        if (!limit.in_range(field)) {
            if (normalize) {
                result.warnings.emplace_back(std::format(
                    "Normalizing {} from {} to default value {}", name, field, limit.default_value
                ));
                field = limit.default_value;
            }
            else {
                result.errors.emplace_back(std::format(
                    "{} value {} is out of range ({}-{})", name, field, limit.min, limit.max
                ));
                result.valid = false;
            }
        }
    }
    else if constexpr (requires { limit.valid(field); }) {
        if (!limit.valid(field)) {
            if (normalize) {
                result.warnings.emplace_back(std::format(
                    "Normalizing {} from {} to default value {}", name, field, limit.default_value
                ));
                field = limit.default_value;
            }
            else {
                result.errors.emplace_back(std::format(
                    "{} value '{}' is invalid (valid options are: {})",
                    name,
                    field,
                    limit.option_list()
                ));
            }
        }
    }
}

} // namespace boyboy::common::config