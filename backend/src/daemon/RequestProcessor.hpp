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

#ifndef REQUESTPROCESSOR_HPP
#define REQUESTPROCESSOR_HPP

#include "core/ApplicationCoreContext.hpp"
#include "daemon/RequestIdGenerator.hpp"
#include "proto/FanControlService.pb.h"
#include "utils/Logging.hpp"
#include "utils/ProtobufTools.hpp"

namespace hfc::daemon {
using namespace hfc::fan_control_service_proto;  // TODO: remove?

class RequestProcessor {
public:
    explicit RequestProcessor(core::ApplicationCoreContext app_ctx);

    GetPluginVersionResponse getPluginVersion(const GetPluginVersionRequest& request);
    GetDeviceMetadataResponse getDeviceMetadata(const GetDeviceMetadataRequest& request);

    GetCurrentFanSpeedResponse getCurrentFanSpeed(const GetCurrentFanSpeedRequest& request);
    void setTargetFanSpeed(const SetTargetFanSpeedRequest& request);

private:
    utils::SharedLogger m_logger;
    core::ApplicationCoreContext m_app_ctx;
    RequestIDGenerator m_request_id_generator;

    template <utils::IsDerivedFromProtoMessage RequestType>
    RequestID logRequest(const RequestType& request) {
        const auto tracking_id = m_request_id_generator.next();
        const auto message_info = utils::getProtoMessageInfo(request);
        m_logger->info(
            "[tracking_id: {}] Received request: {} => [{}]", tracking_id, message_info.name, message_info.data);

        return tracking_id;
    }

    template <utils::IsDerivedFromProtoMessage RequestType>
    void logResponse(const RequestType& request, RequestID request_id) const {
        const auto message_info = utils::getProtoMessageInfo(request);
        m_logger->info(
            "[tracking_id: {}] Sending response: {} => [{}]", request_id, message_info.name, message_info.data);
    }
};
}  // namespace hfc::daemon

#endif  // REQUESTPROCESSOR_HPP
