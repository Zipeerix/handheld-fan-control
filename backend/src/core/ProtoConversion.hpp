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

#ifndef PROTOCONVERSION_HPP
#define PROTOCONVERSION_HPP

#include "FanController.hpp"

#include "proto/FanControlService.pb.h"

namespace hfc::core {
using namespace hfc::fan_control_service_proto;

GetCurrentFanSpeedResponse convertToProtoEquivalent(const FanSpeedData& fan_speed_data);
GetDeviceMetadataResponse convertToProtoEquivalent(device::BaseDeviceInterface& device_interface);
}  // namespace hfc::core

#endif  // PROTOCONVERSION_HPP
