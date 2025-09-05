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

#include "core/ApplicationCoreContext.hpp"
#include "core/FanController.hpp"
#include "daemon/FanControlServiceServer.hpp"
#include "utils/Environment.hpp"

int main() {
    // TODO: autodelete logs every 24h, keep last 2 days
    const auto env_prepare_result = hfc::utils::prepareEnvironment();
    if (!env_prepare_result.has_value()) {
        std::println("Error preparing environment: {}", env_prepare_result.error());
        return EXIT_FAILURE;
    }

    const auto& config_path = hfc::utils::getSettingsFile("default").string();
    const auto config_parse_result = hfc::parseConfigFile(config_path);
    if (!config_parse_result.has_value()) {
        std::println("Unable to parse config: {}", config_parse_result.error());
        return EXIT_FAILURE;
    }

    const auto& plugin_settings = config_parse_result.value();

    hfc::daemon::prepareSocketFile();
    hfc::utils::setLoggerSettings(plugin_settings.logging_settings);

    const auto logger = hfc::utils::createLogger("main");
    logger->info("Starting HandheldFanControl backend with settings: {}", plugin_settings);

    auto fan_controller = hfc::core::FanController(plugin_settings.fan_settings);
    const auto application_ctx = hfc::core::ApplicationCoreContext{.shared_fan_controller = fan_controller};

    const auto daemon = hfc::daemon::FanControlServiceServer(application_ctx);
    daemon.run();

    logger->info("Shutting down HandheldFanControl backend");

    return 0;
}