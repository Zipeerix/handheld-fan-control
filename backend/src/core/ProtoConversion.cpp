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

#include "ProtoConversion.hpp"

namespace hfc::core {
GetCurrentFanSpeedResponse convertToProtoEquivalent(const FanSpeedData& fan_speed_data) {
    GetCurrentFanSpeedResponse proto;
    proto.set_current_fan_speed(fan_speed_data.current_speed);
    proto.set_user_target_fan_speed(fan_speed_data.user_target_speed);

    return proto;
}

GetDeviceMetadataResponse convertToProtoEquivalent(device::BaseDeviceInterface& device_interface) {
    GetDeviceMetadataResponse proto;
    proto.set_device_name(device_interface.getDeviceName());
    proto.set_min_fan_speed(device_interface.getMinimumFanSpeed());
    proto.set_max_fan_speed(device_interface.getMaximumFanSpeed());

    return proto;
}
}  // namespace hfc::core