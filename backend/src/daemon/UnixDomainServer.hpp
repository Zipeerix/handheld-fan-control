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

#ifndef UNIXDOMAINSERVER_HPP
#define UNIXDOMAINSERVER_HPP

#include <expected>

#include <boost/asio.hpp>

#include "meta/Error.hpp"

namespace hfc::daemon {
using ProtocolMessage = std::string;
using ProtocolMessageBuffer = std::vector<char>;

constexpr std::size_t k_payload_length_header_size = 4;

enum class UnixDomainServerError {
    UnknownError,
    NotEnoughBytesReceived,
    GeneralServerError,
    GeneralServerSessionError,
    UnableToSendMessage
};

using RequestHandler = std::function<ProtocolMessage(const ProtocolMessage&)>;

template <typename SuccessType>
using UnixDomainServerResult = std::expected<SuccessType, Error<UnixDomainServerError> >;

class UnixDomainSession final {
public:
    UnixDomainSession(boost::asio::local::stream_protocol::socket socket, RequestHandler request_handler);

    UnixDomainServerResult<void> start();

private:
    boost::asio::local::stream_protocol::socket m_socket;
    RequestHandler m_request_handler;

    UnixDomainServerResult<std::uint32_t> readHeader();

    UnixDomainServerResult<ProtocolMessageBuffer> readBody(std::uint32_t msg_length);

    UnixDomainServerResult<void> writeResponse(const ProtocolMessageBuffer& response);
};

class UnixDomainServer final {
public:
    UnixDomainServer(boost::asio::io_context& io_context,
                     const std::string& socket_path,
                     RequestHandler request_handler);

    UnixDomainServerResult<void> run();

private:
    boost::asio::io_context& m_io_context;
    boost::asio::local::stream_protocol::acceptor m_acceptor;
    RequestHandler m_request_handler;
    std::string m_socket_path;
};
}  // namespace hfc::daemon

#endif  // UNIXDOMAINSERVER_HPP
