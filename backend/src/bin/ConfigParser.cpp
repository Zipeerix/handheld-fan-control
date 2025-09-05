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

#include "ConfigParser.hpp"

#include <format>

#include <toml++/toml.hpp>

#include "utils/Environment.hpp"
#include "utils/Logging.hpp"

namespace hfc {
namespace {
std::expected<LoggingSettings, std::string> parseLoggingConfig(const toml::table* config_ptr) {
    if (!config_ptr) {
        return std::unexpected("Missing logging configuration");
    }

    const auto config = *config_ptr;

    const auto minimum_log_info = config["minimum_log_level"].value<std::string>();
    if (!minimum_log_info.has_value()) {
        return std::unexpected("Missing minimum log level configuration in log configuration");
    }

    const auto minimum_log_level = utils::convertStringToLogLevel(minimum_log_info.value());
    if (!minimum_log_level.has_value()) {
        return std::unexpected(std::format("Invalid log format: {}", minimum_log_info.value()));
    }

    const auto pattern = config["log_pattern"].value<std::string>();
    if (!pattern.has_value()) {
        return std::unexpected("Missing pattern configuration in log configuration");
    }

    return LoggingSettings{.file_output_path = utils::getLogFilePath().string(),
                           .minimum_log_level = minimum_log_level.value(),
                           .pattern = pattern.value()};
}

std::expected<GeneralSettings, std::string> parseGeneralConfig(const toml::table* config_ptr) {
    if (!config_ptr) {
        return std::unexpected("Missing general configuration");
    }

    const auto config = *config_ptr;

    const auto temp_update_interval_ms = config["temp_update_interval_ms"].value<std::uint16_t>();
    if (!temp_update_interval_ms.has_value()) {
        return std::unexpected("Missing temp_update_interval_ms configuration in general configuration");
    }

    return GeneralSettings{.temp_update_interval_ms = temp_update_interval_ms.value()};
}

std::expected<FanSettings, std::string> parseFanConfig(const toml::table* config_ptr) {
    if (!config_ptr) {
        return std::unexpected("Missing fan configuration");
    }

    const auto config = *config_ptr;
    FanSettings settings;

    for (const auto& [key, value] : config) {
        try {
            const auto temperature = std::stoull(std::string(key));

            auto speed_opt = value.value<std::uint64_t>();
            if (!speed_opt.has_value()) {
                return std::unexpected("Invalid speed value for temperature " + std::string(key));
            }

            settings.temperatures_to_speeds[temperature] = speed_opt.value();
        } catch (const std::exception&) {
            return std::unexpected("Invalid temperature value: " + std::string(key));
        }
    }

    if (settings.temperatures_to_speeds.empty()) {
        return std::unexpected("No temperature-to-speed mappings found in fan configuration");
    }

    return settings;
}
}  // namespace

std::expected<PluginSettings, std::string> parseConfigFile(std::string_view file_path) {
    const auto config_file = toml::parse_file(file_path);
    if (!config_file.succeeded()) {
        return std::unexpected(std::format("Unable to open or parse configuration file at {}", file_path));
    }

    const auto logging_config_parse_res = parseLoggingConfig(config_file["logging"].as_table());
    if (!logging_config_parse_res.has_value()) {
        return std::unexpected(logging_config_parse_res.error());
    }

    const auto fan_config_parse_res = parseFanConfig(config_file["fan"].as_table());
    if (!fan_config_parse_res.has_value()) {
        return std::unexpected(fan_config_parse_res.error());
    }

    const auto general_config_parse_res = parseGeneralConfig(config_file["general"].as_table());
    if (!general_config_parse_res.has_value()) {
        return std::unexpected(general_config_parse_res.error());
    }

    return PluginSettings{.logging_settings = logging_config_parse_res.value(),
                          .general_settings = general_config_parse_res.value(),
                          .fan_settings = fan_config_parse_res.value()};
}

std::uint64_t FanSettings::getSpeedForTemperature(std::uint64_t temperature) {
    auto it = temperatures_to_speeds.find(temperature);
    if (it != temperatures_to_speeds.end()) {
        return it->second;
    }

    std::uint64_t lower_temp = 0;
    std::uint64_t upper_temp = std::numeric_limits<std::uint64_t>::max();
    std::uint64_t lower_speed = 0;
    std::uint64_t upper_speed = 0;

    auto found_lower = false;
    auto found_upper = false;

    for (const auto& [temp, speed] : temperatures_to_speeds) {
        if (temp <= temperature && temp > lower_temp) {
            lower_temp = temp;
            lower_speed = speed;
            found_lower = true;
        }
        if (temp > temperature && temp < upper_temp) {
            upper_temp = temp;
            upper_speed = speed;
            found_upper = true;
        }
    }

    if (!found_lower && found_upper) {
        return upper_speed;
    }
    if (found_lower && !found_upper) {
        return lower_speed;
    }
    if (!found_lower && !found_upper) {
        return 0;
    }

    const auto actual_speed =
        lower_speed + (((upper_speed - lower_speed) * (temperature - lower_temp)) / (upper_temp - lower_temp));

    return actual_speed;
}
}  // namespace hfc