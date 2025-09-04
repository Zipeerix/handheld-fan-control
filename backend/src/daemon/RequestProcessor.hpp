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
#include "proto/FanControlService.pb.h"
#include "utils/Logging.hpp"

using namespace hfc::fan_control_service_proto;  // TODO: remove?

namespace hfc::daemon {
class RequestProcessor {
public:
    explicit RequestProcessor(core::ApplicationCoreContext app_ctx);

    static GetPluginVersionResponse getPluginVersion(const GetPluginVersionRequest& request);

private:
    utils::SharedLogger m_logger;
    core::ApplicationCoreContext m_app_ctx;
};
}  // namespace hfc::daemon

#endif  // REQUESTPROCESSOR_HPP
