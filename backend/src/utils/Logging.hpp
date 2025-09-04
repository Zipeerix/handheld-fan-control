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

#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <memory>
#include <string>

#include <spdlog/logger.h>

namespace hfc {
struct LoggingSettings;
}

namespace hfc::utils {
using SharedLogger = std::shared_ptr<spdlog::logger>;

enum class LogLevel {
    Trace,
    Debug,
    Info,
    Warn,
    Error,
    Critical,
    Off
};

std::optional<LogLevel> convertStringToLogLevel(const std::string& str);

std::string convertLogLevelToString(LogLevel level);

spdlog::level::level_enum convertLogLevelToSpdlogLevel(LogLevel level);

void setLoggerSettings(const LoggingSettings& config);

SharedLogger createLogger(const std::string& name);
}  // namespace hfc::utils

#endif  // LOGGING_HPP
