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

#ifndef BASEDEVICEINTERFACE_HPP
#define BASEDEVICEINTERFACE_HPP

#include <memory>
#include <string>

namespace hfc::core::device {
/*
 * TODO: either read /sys files for fan speed, cpu temps etc etc maybe using Sysfspwm or std::ifstream file(hwmon_path +
 * "/fan1_input"); or libudev or find a way to interact with lm_sensors
 */
class BaseDeviceInterface {
public:
    virtual ~BaseDeviceInterface() = default;

    virtual std::string getDeviceName() const = 0;
    virtual std::uint64_t getMinimumFanSpeed() = 0;
    virtual std::uint64_t getMaximumFanSpeed() = 0;
    virtual std::uint64_t getCurrentTemperature() = 0;
    virtual std::uint64_t getCurrentFanSpeed() = 0;

    virtual void setFanSpeed(std::uint64_t speed_rpm) = 0;
};

std::unique_ptr<BaseDeviceInterface> getDeviceInterface();
}  // namespace hfc::core::device

#endif  // BASEDEVICEINTERFACE_HPP
