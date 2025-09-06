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

#include "UnixDomainServer.hpp"

#include <stdexcept>
#include <thread>

namespace hfc::daemon {
// TODO: Query temps every that time and make it configurable, is 100ms ok?
constexpr auto k_sync_server_loop_sleep_ms = std::chrono::milliseconds(100);

UnixDomainSession::UnixDomainSession(boost::asio::local::stream_protocol::socket socket,
                                     RequestHandler request_handler) :
    m_socket(std::move(socket)),
    m_request_handler(std::move(request_handler)) {
}

UnixDomainServerResult<void> UnixDomainSession::start() {
    try {
        while (true) {
            const auto read_header_result = readHeader();
            if (!read_header_result.has_value()) {
                return std::unexpected(read_header_result.error());
            }

            const auto read_body_result = readBody(read_header_result.value());
            if (!read_body_result.has_value()) {
                return std::unexpected(read_body_result.error());
            }

            const auto write_response_result = writeResponse(read_body_result.value());
            if (!write_response_result.has_value()) {
                return std::unexpected(write_response_result.error());
            }

            std::this_thread::sleep_for(k_sync_server_loop_sleep_ms);
        }
    } catch (const boost::system::system_error& e) {
        if (e.code() == boost::asio::error::eof || e.code() == boost::asio::error::bad_descriptor) {
            // Client disconnected normally
        } else {
            // TODO: Maybe not general server session error, be more specific
            return std::unexpected(Error(UnixDomainServerError::GeneralServerSessionError, e.what()));
        }
    }

    return {};
}

UnixDomainServerResult<std::uint32_t> UnixDomainSession::readHeader() {
    std::uint32_t msg_length = 0;
    const auto bytes_read = boost::asio::read(m_socket, boost::asio::buffer(&msg_length, k_payload_length_header_size));
    if (bytes_read != k_payload_length_header_size) {
        std::unexpected(Error(UnixDomainServerError::NotEnoughBytesReceived));
    }

    return msg_length;
}

UnixDomainServerResult<ProtocolMessageBuffer> UnixDomainSession::readBody(const std::uint32_t msg_length) {
    ProtocolMessageBuffer message_buffer;
    message_buffer.resize(msg_length);
    const auto bytes_read = boost::asio::read(m_socket, boost::asio::buffer(message_buffer, msg_length));
    if (bytes_read != msg_length) {
        return std::unexpected(Error(UnixDomainServerError::NotEnoughBytesReceived));
    }

    const auto request = ProtocolMessage(message_buffer.begin(), message_buffer.begin() + bytes_read);
    const auto response = m_request_handler(request);

    const auto response_length = static_cast<std::uint32_t>(response.length());

    ProtocolMessageBuffer response_message_buffer;
    response_message_buffer.resize(k_payload_length_header_size + response.length());

    std::memcpy(response_message_buffer.data(), &response_length, k_payload_length_header_size);
    std::memcpy(response_message_buffer.data() + k_payload_length_header_size, response.data(), response.length());

    return std::move(response_message_buffer);
}

UnixDomainServerResult<void> UnixDomainSession::writeResponse(const ProtocolMessageBuffer& response) {
    try {
        boost::asio::write(m_socket, boost::asio::buffer(response));
    } catch (std::exception& e) {
        return std::unexpected(Error(UnixDomainServerError::UnableToSendMessage, e.what()));
    }

    return {};
}

// TODO: This constructor throw when address of socket is in use. Unlink it if its in use
UnixDomainServer::UnixDomainServer(boost::asio::io_context& io_context,
                                   const std::string& socket_path,
                                   RequestHandler request_handler) :
    m_io_context(io_context),
    m_acceptor(io_context, boost::asio::local::stream_protocol::endpoint(socket_path)),
    m_request_handler(std::move(request_handler)),
    m_socket_path(socket_path) {
}

UnixDomainServerResult<void> UnixDomainServer::run() {
    try {
        m_acceptor.listen();
        while (true) {
            boost::asio::local::stream_protocol::socket socket(m_io_context);
            m_acceptor.accept(socket);

            UnixDomainSession session(std::move(socket), m_request_handler);
            const auto session_result = session.start();
            if (!session_result.has_value()) {
                unlink(m_socket_path.c_str());
                return std::unexpected(session_result.error());
            }

            std::this_thread::sleep_for(k_sync_server_loop_sleep_ms);
        }
    } catch (const boost::system::system_error& e) {
        // TODO: Maybe not general server error, be more specific
        unlink(m_socket_path.c_str());
        return std::unexpected(Error(UnixDomainServerError::GeneralServerError, e.what()));
    }
}
}  // namespace hfc::daemon