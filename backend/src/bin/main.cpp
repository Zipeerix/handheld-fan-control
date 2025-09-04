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

static std::expected<hfc::PluginSettings, int> parseCommandLineArguments(const int argc, const char** argv) {
    const auto arguments = std::vector<std::string>(argv + 1, argv + argc);
    if (arguments.size() != 1) {
        std::cout << "usage: HandheldFanControlBackend <config-path>" << std::endl;
        return std::unexpected(EXIT_FAILURE);
    }

    const std::string& config_path = arguments.at(0);
    const auto parse_res = hfc::parseConfigFile(config_path);
    if (!parse_res.has_value()) {
        std::cout << parse_res.error() << std::endl;
        return std::unexpected(EXIT_FAILURE);
    }

    return parse_res.value();
}

int main(const int argc, const char** argv) {
    const auto parse_result = parseCommandLineArguments(argc, argv);
    if (!parse_result.has_value()) {
        return parse_result.error();
    }

    const auto& plugin_settings = parse_result.value();

    hfc::daemon::prepareSocketFile();
    hfc::utils::setLoggerSettings(plugin_settings.logging_settings);

    hfc::utils::SharedLogger logger = hfc::utils::createLogger("main");
    logger->info("Starting HandheldFanControl backend with settings: {}", plugin_settings);

    auto fan_controller = hfc::core::FanController(plugin_settings.fan_settings);
    const auto application_ctx = hfc::core::ApplicationCoreContext{.shared_fan_controller = fan_controller};

    const auto daemon = hfc::daemon::FanControlServiceServer(application_ctx);
    daemon.run();

    logger->info("Shutting down HandheldFanControl backend");

    return 0;
}