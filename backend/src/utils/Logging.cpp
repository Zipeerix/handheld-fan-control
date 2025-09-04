/***************************************************************************
 *  HandheldFanControl Copyright (C) 2025  Ziperix                         *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "Logging.hpp"

#include "bin/ConfigParser.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace hfc::utils {
namespace {
std::string convertStringToLowercase(std::string target) {
    std::ranges::transform(target, target.begin(), [](const char c) { return std::tolower(c); });

    return target;
}
}  // namespace

static std::optional<LoggingSettings> g_shared_logging_settings = std::nullopt;

void setLoggerSettings(const LoggingSettings& config) {
    g_shared_logging_settings = config;
}

std::optional<LogLevel> convertStringToLogLevel(const std::string& str) {
    const std::string lc_str = convertStringToLowercase(str);
    if (lc_str == "trace") {
        return LogLevel::Trace;
    } else if (lc_str == "debug") {
        return LogLevel::Debug;
    } else if (lc_str == "info") {
        return LogLevel::Info;
    } else if (lc_str == "warn" || lc_str == "warning") {
        return LogLevel::Warn;
    } else if (lc_str == "error" || lc_str == "err") {
        return LogLevel::Error;
    } else if (lc_str == "critical" || lc_str == "fatal" || lc_str == "crit") {
        return LogLevel::Critical;
    } else if (lc_str == "off" || lc_str == "disable" || lc_str == "disabled" || lc_str == "none") {
        return LogLevel::Off;
    }

    return std::nullopt;
}

std::string convertLogLevelToString(const LogLevel level) {
    switch (level) {
        case LogLevel::Trace:
            return "trace";
        case LogLevel::Debug:
            return "debug";
        case LogLevel::Info:
            return "info";
        case LogLevel::Warn:
            return "warn";
        case LogLevel::Error:
            return "error";
        case LogLevel::Critical:
            return "critical";
        case LogLevel::Off:
            return "off";
    }
}

spdlog::level::level_enum convertLogLevelToSpdlogLevel(const LogLevel level) {
    switch (level) {
        case LogLevel::Trace:
            return spdlog::level::trace;
        case LogLevel::Debug:
            return spdlog::level::debug;
        case LogLevel::Info:
            return spdlog::level::info;
        case LogLevel::Warn:
            return spdlog::level::warn;
        case LogLevel::Error:
            return spdlog::level::err;
        case LogLevel::Critical:
            return spdlog::level::critical;
        case LogLevel::Off:
            return spdlog::level::off;
    }
}

SharedLogger createLogger(const std::string& name) {
    if (!g_shared_logging_settings) {
        throw std::logic_error("Attempting to create logger before settings are set");
    }

    auto logger = spdlog::get(name);
    if (logger) {
        return logger;
    }

    std::vector<spdlog::sink_ptr> sinks;

    const auto min_log_level = convertLogLevelToSpdlogLevel(g_shared_logging_settings->minimum_log_level);

    // TODO: Implement logging to file, important for plugging debugging on machine
    const auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(min_log_level);
    console_sink->set_pattern(g_shared_logging_settings->pattern);
    sinks.push_back(console_sink);

    logger = std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());
    logger->set_level(min_log_level);

    spdlog::register_logger(logger);

    return logger;
}
}  // namespace hfc::utils
