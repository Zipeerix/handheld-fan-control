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

#ifndef DAEMONSERVER_HPP
#define DAEMONSERVER_HPP

#include "UnixDomainServer.hpp"

#include <expected>
#include <format>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include <boost/asio.hpp>

#include "meta/Error.hpp"
#include "utils/Logging.hpp"
#include "utils/ProtobufTools.hpp"

namespace hfc::daemon {
enum class DaemonServerError {
    UnknownError,
    HandlerNotRegistered,
    RuntimeError,
    UnableToDeserializeMessage,
    UnableToSerializeMessage,
};

// TODO: Move somewhere else, in DaemonServer ctor for example before Unix gets ctored, so no function like this
void prepareSocketFile();

template <typename SuccessType>
using DaemonServerResult = std::expected<SuccessType, Error<DaemonServerError> >;

class DaemonServer final {
    template <utils::IsDerivedFromProtoMessage RequestType, utils::IsDerivedFromProtoMessage ResponseType>
    using HandlerForSpecificType = std::function<ResponseType(const RequestType&)>;

public:
    DaemonServer();

    template <utils::IsDerivedFromProtoMessage RequestType, utils::IsDerivedFromProtoMessage ResponseType>
    bool registerHandler(HandlerForSpecificType<RequestType, ResponseType> handler) {
        const auto request_name = RequestType::descriptor()->full_name();
        const auto response_name = ResponseType::descriptor()->full_name();
        if (m_handlers.contains(request_name)) {
            throw std::logic_error("Handler already registered for request type " + request_name);
        }

        registerRequestResponsePair<RequestType, ResponseType>(request_name, response_name, handler);
        return true;
    }

    DaemonServerResult<void> start() const;

private:
    using GenericHandler = std::function<std::string(const utils::BaseProtoType&)>;

    utils::SharedLogger m_logger;

    boost::asio::io_context m_io_context;
    std::unordered_map<std::string, GenericHandler> m_handlers;
    std::unordered_map<std::string, std::string> m_request_response_pairs;
    std::unique_ptr<UnixDomainServer> m_server;

    template <utils::IsDerivedFromProtoMessage RequestType, utils::IsDerivedFromProtoMessage ResponseType>
    void registerRequestResponsePair(const std::string& request_name,
                                     const std::string& response_name,
                                     HandlerForSpecificType<RequestType, ResponseType> handler) {
        m_handlers[request_name] = [handler = std::move(handler)](const utils::BaseProtoType& msg) {
            const auto response = handler(static_cast<const RequestType&>(msg));
            return utils::makePayloadFromProto(response);
        };
        m_request_response_pairs[request_name] = response_name;
    }

    DaemonServerResult<utils::SerializedProtoPayload> acceptMessage(const utils::SerializedProtoPayload& serialized);
};
}  // namespace hfc::daemon

template <>
struct std::formatter<hfc::daemon::DaemonServerError> {
public:
    static constexpr auto parse(const std::format_parse_context& ctx) {
        return ctx.begin();
    }

    static auto format(const hfc::daemon::DaemonServerError error, std::format_context& ctx) {
        return std::format_to(ctx.out(), "{}", convertDaemonServerErrorToString(error));
    }

private:
    static constexpr std::string_view convertDaemonServerErrorToString(
        const hfc::daemon::DaemonServerError error_type) {
        switch (error_type) {
            case hfc::daemon::DaemonServerError::UnknownError:
                return "Unknown error";
            case hfc::daemon::DaemonServerError::HandlerNotRegistered:
                return "Handler not registered";
            case hfc::daemon::DaemonServerError::RuntimeError:
                return "Runtime error";
            case hfc::daemon::DaemonServerError::UnableToDeserializeMessage:
                return "Unable to deserialize message";
            case hfc::daemon::DaemonServerError::UnableToSerializeMessage:
                return "Unable to serialize message";

            default:
                return "<Unknown>";
        }
    }
};

#endif  // DAEMONSERVER_HPP
