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

#include "Environment.hpp"

#include "Time.hpp"

#include <filesystem>
#include <fstream>

#include <libenvpp/env.hpp>

namespace hfc::utils {
namespace {
constexpr auto k_decky_prefix = "DECKY_PLUGIN";
constexpr auto k_decky_log_dir_env_var = "LOG_DIR";
constexpr auto k_decky_settings_env_var = "SETTINGS_DIR";

constexpr auto k_default_preset_name = "default";

std::filesystem::path g_log_dir_path;
std::filesystem::path g_settings_dir_path;

std::expected<void, std::string> createDefaultConfigFileIfNotExists() {
    const auto default_config_path = g_settings_dir_path / std::format("{}.toml", k_default_preset_name);
    if (!std::filesystem::exists(default_config_path)) {
        std::filesystem::create_directories(default_config_path.parent_path());

        auto file = std::ofstream(default_config_path);
        if (file.is_open()) {
            file << "[logging]\n";
            file << "minimum_log_level = \"info\"\n";
            file << "log_pattern = \"[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] %v\"\n";
            file << "[general]\n";
            file << "temp_update_interval_ms = 1000\n";
            file << "[fan]\n";

            file.close();
        } else {
            return std::unexpected(
                std::format("Unable to create default config file at {}", default_config_path.string()));
        }
    }

    return {};
}
}  // namespace

std::expected<void, std::string> prepareEnvironment() {
    auto env = env::prefix(k_decky_prefix);
    const auto log_dir_env_var_handle = env.register_required_variable<std::filesystem::path>(k_decky_log_dir_env_var);
    const auto settings_dir_env_var_handle =
        env.register_required_variable<std::filesystem::path>(k_decky_settings_env_var);

    const auto parsed_and_validated_pre = env.parse_and_validate();
    if (!parsed_and_validated_pre.ok()) {
        return std::unexpected(parsed_and_validated_pre.error_message());
    }

    const auto log_dir = parsed_and_validated_pre.get(log_dir_env_var_handle);
    g_log_dir_path = log_dir;
    const auto settings_dir = parsed_and_validated_pre.get(settings_dir_env_var_handle);
    g_settings_dir_path = settings_dir;

    const auto default_config_creation_result = createDefaultConfigFileIfNotExists();
    if (!default_config_creation_result.has_value()) {
        return std::unexpected(default_config_creation_result.error());
    }

    return {};
}

std::filesystem::path getLogFilePath() {
    const auto current_time = getCurrentUtcTime();
    const auto file_name = std::format("log-{}.txt", current_time);

    return g_log_dir_path / file_name;
}

std::filesystem::path getSettingsFile(const std::string_view preset) {
    if (preset != "default") {
        throw std::runtime_error("Non default presets not implemented");
    }

    return g_settings_dir_path / "default.toml";
}
}  // namespace hfc::utils