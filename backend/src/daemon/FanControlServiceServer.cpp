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

#include "FanControlServiceServer.hpp"

#include "meta/Metadata.hpp"
#include "proto/FanControlService.pb.h"

namespace hfc::daemon {
FanControlServiceServer::FanControlServiceServer(const core::ApplicationCoreContext app_ctx) :
    m_logger(utils::createLogger("FanControlServiceServer")),
    m_app_ctx(app_ctx),
    m_request_processor(m_app_ctx) {
    using namespace hfc::fan_control_service_proto;

    m_daemon_server.registerHandler<GetPluginVersionRequest, GetPluginVersionResponse>(
        [this](const auto& request) { return m_request_processor.getPluginVersion(request); });

    m_daemon_server.registerHandler<GetDeviceMetadataRequest, GetDeviceMetadataResponse>(
        [this](const auto& request) { return m_request_processor.getDeviceMetadata(request); });

    m_daemon_server.registerHandler<GetCurrentFanSpeedRequest, GetCurrentFanSpeedResponse>(
        [this](const auto& request) { return m_request_processor.getCurrentFanSpeed(request); });

    m_daemon_server.registerHandler<SetTargetFanSpeedRequest, google::protobuf::Empty>([this](const auto& request) {
        m_request_processor.setTargetFanSpeed(request);
        return google::protobuf::Empty();
    });
}

void FanControlServiceServer::run() const {
    m_logger->info("Starting handheld fan control server ({})", meta::getPluginVersion());
    const auto result = m_daemon_server.start();
    if (!result.has_value()) {
        m_logger->error("Error while running daemon server: {}", result.error().message);
    }
}
}  // namespace hfc::daemon