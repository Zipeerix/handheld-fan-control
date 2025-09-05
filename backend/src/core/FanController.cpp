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

#include "FanController.hpp"

namespace hfc::core {
FanController::FanController(GeneralSettings general_settings, FanSettings fan_settings) :
    m_logger(utils::createLogger("FanController")),
    m_general_settings(std::move(general_settings)),
    m_fan_settings(std::move(fan_settings)),
    m_device_interface(device::getDeviceInterface()) {
    m_logger->info("Initialized device interface '{}': [min fan speed: {} RPM, max fan speed: {} RPM]",
                   m_device_interface->getDeviceName(),
                   m_device_interface->getMinimumFanSpeed(),
                   m_device_interface->getMaximumFanSpeed());
}

FanController::~FanController() {
    m_continue_monitoring = false;
    if (m_monitor_thread != nullptr && m_monitor_thread->joinable()) {
        m_logger->info("Stopping monitor thread");
        m_monitor_thread->join();
    }
}

void FanController::startMonitor() {
    m_continue_monitoring = true;
    m_logger->info("Starting monitor thread");
    m_monitor_thread = std::make_unique<std::thread>([this]() {
        while (m_continue_monitoring) {
            const auto temp = m_device_interface->getCurrentTemperature();
            {
                const auto lock = std::scoped_lock(m_fan_settings_mutex);
                m_logger->info(
                    "Temperature: {}C, setting fan speed to: {}", temp, m_fan_settings.getSpeedForTemperature(temp));
                std::this_thread::sleep_for(std::chrono::milliseconds(m_general_settings.temp_update_interval_ms));
            }
        }
    });
}

FanSpeedData FanController::getCurrentFanSpeed() {
    const auto temp = m_device_interface->getCurrentTemperature();
    const auto fan_speed_data = FanSpeedData{.current_speed = m_device_interface->getCurrentFanSpeed(),
                                             .user_target_speed = m_fan_settings.getSpeedForTemperature(temp)};

    return fan_speed_data;
}

void FanController::setTargetFanSpeed(const std::uint64_t temperature, const std::uint64_t target_speed) {
    const auto lock = std::scoped_lock(m_fan_settings_mutex);
    m_fan_settings.temperatures_to_speeds[temperature] = target_speed;
}

const std::unique_ptr<device::BaseDeviceInterface>& FanController::getDeviceInterface() const {
    return m_device_interface;
}
}  // namespace hfc::core