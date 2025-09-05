/***************************************************************************
 *  HandheldFanControl Copyright (C) 2025  Ziperix                         *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRAN-=TY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>. *
 ***************************************************************************/

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <cstdint>
#include <expected>
#include <format>
#include <map>
#include <optional>
#include <string>

#include "utils/Logging.hpp"

namespace hfc {
struct LoggingSettings {
    std::string file_output_path;
    utils::LogLevel minimum_log_level;
    std::string pattern;
};

struct FanSettings {
    // TODO: Add setting (in general) to use interpolation or just change RPM when temperature reaches next point
    std::map<std::uint64_t, std::uint64_t> temperatures_to_speeds;

    // TODO: Move this to diff class, this should just be simple struct
    // TODO: When its a class, add adding/removing points methods
    std::uint64_t getSpeedForTemperature(std::uint64_t temperature);
};

struct GeneralSettings {
    std::uint64_t temp_update_interval_ms;
};

struct PluginSettings {
    LoggingSettings logging_settings;
    GeneralSettings general_settings;
    FanSettings fan_settings;
};

// TODO: ConfigParserResult like in UnixServer
std::expected<PluginSettings, std::string> parseConfigFile(std::string_view file_path);
}  // namespace hfc

template <>
struct std::formatter<hfc::LoggingSettings> {
    static constexpr auto parse(const std::format_parse_context& ctx) {
        return ctx.begin();
    }

    static auto format(const hfc::LoggingSettings& obj, std::format_context& ctx) {
        return std::format_to(ctx.out(),
                              "[file_output_path=\"{}\", log_level=\"{}\", pattern=\"{}\"]",
                              obj.file_output_path,
                              hfc::utils::convertLogLevelToString(obj.minimum_log_level),
                              obj.pattern);
    }
};

template <>
struct std::formatter<hfc::GeneralSettings> {
    static constexpr auto parse(const std::format_parse_context& ctx) {
        return ctx.begin();
    }

    static auto format(const hfc::GeneralSettings& obj, std::format_context& ctx) {
        return std::format_to(ctx.out(), "[temp_update_interval_ms={}]", obj.temp_update_interval_ms);
    }
};

template <>
struct std::formatter<hfc::FanSettings> {
    static constexpr auto parse(const std::format_parse_context& ctx) {
        return ctx.begin();
    }

    static auto format(const hfc::FanSettings& obj, std::format_context& ctx) {
        return std::format_to(ctx.out(), "[temperatures_to_speeds={}]", obj.temperatures_to_speeds);
    }
};

template <>
struct std::formatter<hfc::PluginSettings> {
    static constexpr auto parse(const std::format_parse_context& ctx) {
        return ctx.begin();
    }

    static auto format(const hfc::PluginSettings& obj, std::format_context& ctx) {
        return std::format_to(ctx.out(),
                              "[logging_settings={}, general_settings={}, fan_settings={}]",
                              obj.logging_settings,
                              obj.general_settings,
                              obj.fan_settings);
    }
};

#endif  // CONFIGPARSER_HPP
