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

#include "DaemonServer.hpp"

#include "utils/Environment.hpp"

namespace hfc::daemon {
namespace {
constexpr auto k_unix_domain_socket_filename = "handheld_fan_control.sock";
}

std::string getDomainSocketPath() {
    return (utils::getRuntimePath() / k_unix_domain_socket_filename).string();
}

void prepareSocketFile() {
    unlink(getDomainSocketPath().c_str());
}

DaemonServer::DaemonServer() :
    m_logger(utils::createLogger("DaemonServer")) {
    const auto domain_socket_path = getDomainSocketPath();
    m_logger->info("Starting daemon server on domain socket: {}", domain_socket_path);
    m_server = std::make_unique<UnixDomainServer>(m_io_context, domain_socket_path, [this](const ProtocolMessage& msg) {
        // TODO: acceptMessage error handling should be exception?
        const auto accept_result = acceptMessage(msg);
        if (!accept_result.has_value()) {
            throw std::runtime_error(std::format("Error while accepting message: {}", accept_result.error()));
        }

        return accept_result.value();
    });
}

DaemonServerResult<void> DaemonServer::start() const {
    const auto result = m_server->run();
    if (!result.has_value()) {
        // TODO: erorr mapping? Remove runtime error altogether maybe
        return std::unexpected(Error(DaemonServerError::RuntimeError, result.error().message));
    }

    return {};
}

DaemonServerResult<utils::SerializedProtoPayload> DaemonServer::acceptMessage(
    const utils::SerializedProtoPayload& serialized) {
    const auto msg_result = utils::makeBaseProtoFromPayload(serialized);
    if (!msg_result.has_value()) {
        return std::unexpected(Error(DaemonServerError::UnableToDeserializeMessage, msg_result.error().message));
    }

    const auto& msg = msg_result.value();

    const auto type_name = msg->GetDescriptor()->full_name();
    const auto it = m_handlers.find(type_name);
    if (it == m_handlers.end()) {
        return std::unexpected(
            Error(DaemonServerError::HandlerNotRegistered, std::format("No handler for {} registered", type_name)));
    }

    return it->second(*msg);
}
}  // namespace hfc::daemon