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

#include "MockDeviceInterface.hpp"

namespace hfc::core::device {
MockDeviceInterface::MockDeviceInterface() :
    m_logger(utils::createLogger("MockDeviceInterface")) {
}

std::string MockDeviceInterface::getDeviceName() const {
    return "Mock Device";
}

std::uint64_t MockDeviceInterface::getMinimumFanSpeed() {
    return 0;
}

std::uint64_t MockDeviceInterface::getMaximumFanSpeed() {
    return 7'000;
}

std::uint64_t MockDeviceInterface::getCurrentTemperature() {
    return rand() % 101;
}

std::uint64_t MockDeviceInterface::getCurrentFanSpeed() {
    return m_current_fan_speed;
}

void MockDeviceInterface::setFanSpeed(const std::uint64_t speed_rpm) {
    m_current_fan_speed = speed_rpm;
}
}  // namespace hfc::core::device
