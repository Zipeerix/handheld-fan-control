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

#ifndef FANCONTROLSERVICESERVER_HPP
#define FANCONTROLSERVICESERVER_HPP

#include "DaemonServer.hpp"
#include "RequestProcessor.hpp"

#include "core/ApplicationCoreContext.hpp"
#include "utils/Logging.hpp"

namespace hfc::daemon {
class FanControlServiceServer final {
public:
    explicit FanControlServiceServer(core::ApplicationCoreContext app_ctx);

    void run() const;

private:
    utils::SharedLogger m_logger;
    core::ApplicationCoreContext m_app_ctx;

    RequestProcessor m_request_processor;
    DaemonServer m_daemon_server;
};
}  // namespace hfc::daemon

#endif  // FANCONTROLSERVICESERVER_HPP
