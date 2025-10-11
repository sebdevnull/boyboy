/**
 * @file config_loader.h
 * @brief Configuration loader interface and TOML implementation.
 *
 * @license GPLv3 (see LICENSE file)
 */

#pragma once

#include <istream>
#include <ostream>

#include "boyboy/common/config/config.h"

namespace boyboy::common::config {

class IConfigLoader {
public:
    IConfigLoader() = default;
    virtual ~IConfigLoader() = default;
    IConfigLoader(const IConfigLoader&) = delete;
    IConfigLoader(const IConfigLoader&&) = delete;
    IConfigLoader& operator=(const IConfigLoader&) = delete;
    IConfigLoader& operator=(const IConfigLoader&&) = delete;

    [[nodiscard]] virtual Config load(std::istream& input, bool normalize = true) const = 0;
    void virtual save(const Config& config, std::ostream& output) const = 0;
};

class TomlConfigLoader : public IConfigLoader {
public:
    [[nodiscard]] Config load(std::istream& input, bool normalize = true) const override;
    void save(const Config& config, std::ostream& output) const override;
};

} // namespace boyboy::common::config