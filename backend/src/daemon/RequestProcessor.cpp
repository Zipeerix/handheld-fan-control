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

#include "RequestProcessor.hpp"

#include "core/ProtoConversion.hpp"
#include "meta/Metadata.hpp"

namespace hfc::daemon {
RequestProcessor::RequestProcessor(const core::ApplicationCoreContext app_ctx) :
    m_logger(utils::createLogger("RequestProcessor")),
    m_app_ctx(app_ctx) {
}

// TODO: Generic handleRequest with templates, log there, and there route to specific handlers (via template overload)

GetPluginVersionResponse RequestProcessor::getPluginVersion(const GetPluginVersionRequest& request) {
    const auto request_id = logRequest(request);

    GetPluginVersionResponse response;
    response.set_version(meta::getPluginVersion());

    logResponse(response, request_id);

    return response;
}

GetDeviceMetadataResponse RequestProcessor::getDeviceMetadata(const GetDeviceMetadataRequest& request) {
    const auto request_id = logRequest(request);

    // TODO: Refactor?
    const auto& device_interface = m_app_ctx.shared_fan_controller.getDeviceInterface();
    const auto response = core::convertToProtoEquivalent(*device_interface);

    logResponse(response, request_id);

    return response;
}

GetCurrentFanSpeedResponse RequestProcessor::getCurrentFanSpeed(const GetCurrentFanSpeedRequest& request) {
    const auto request_id = logRequest(request);

    const auto result = m_app_ctx.shared_fan_controller.getCurrentFanSpeed();
    const auto response = core::convertToProtoEquivalent(result);

    logResponse(response, request_id);

    return response;
}

void RequestProcessor::setTargetFanSpeed(const SetTargetFanSpeedRequest& request) {
    const auto request_id = logRequest(request);

    const auto temperature = request.temperature_celsius();
    const auto speed = request.target_fan_speed();
    m_app_ctx.shared_fan_controller.setTargetFanSpeed(temperature, speed);

    logResponse(google::protobuf::Empty(), request_id);
}
}  // namespace hfc::daemon