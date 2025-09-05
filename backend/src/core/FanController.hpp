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

#ifndef FANCONTROLLER_HPP
#define FANCONTROLLER_HPP

#include <thread>

#include "bin/ConfigParser.hpp"
#include "device/BaseDeviceInterface.hpp"

namespace hfc::core {
struct FanSpeedData {
    std::uint64_t current_speed;
    std::uint64_t user_target_speed;
};

class FanController {
public:
    FanController(GeneralSettings general_settings, FanSettings fan_settings);
    ~FanController();

    // TODO: Separate temperature monitor class? With callbacks to this clas or no?
    void startMonitor();

    FanSpeedData getCurrentFanSpeed();
    void setTargetFanSpeed(std::uint64_t temperature, std::uint64_t target_speed);

    // TODO: Get rid of this if refactor in RequestProcessor
    const std::unique_ptr<device::BaseDeviceInterface>& getDeviceInterface() const;

private:
    utils::SharedLogger m_logger;
    GeneralSettings m_general_settings;

    FanSettings m_fan_settings;
    std::mutex m_fan_settings_mutex;

    std::unique_ptr<device::BaseDeviceInterface> m_device_interface;

    bool m_continue_monitoring = false;
    std::unique_ptr<std::thread> m_monitor_thread;
};
}  // namespace hfc::core

#endif  // FANCONTROLLER_HPP
