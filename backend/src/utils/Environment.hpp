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

#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <expected>
#include <filesystem>
#include <string>
#include <string_view>

namespace hfc::utils {
std::expected<void, std::string> prepareEnvironment();

std::filesystem::path getLogFilePath();
std::filesystem::path getSettingsFile(std::string_view preset);
}  // namespace hfc::utils

#endif  // ENVIRONMENT_HPP
