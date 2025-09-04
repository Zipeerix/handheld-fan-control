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

#include "meta/Metadata.hpp"

namespace hfc::daemon {
RequestProcessor::RequestProcessor(const core::ApplicationCoreContext app_ctx) :
    m_logger(utils::createLogger("RequestProcessor")),
    m_app_ctx(app_ctx) {
}

GetPluginVersionResponse RequestProcessor::getPluginVersion([[maybe_unused]] const GetPluginVersionRequest& request) {
    GetPluginVersionResponse response;
    response.set_version(meta::getPluginVersion());

    return response;
}
}  // namespace hfc::daemon