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

#ifndef STEAMDECKINTERFACE_HPP
#define STEAMDECKINTERFACE_HPP

#include "BaseDeviceInterface.hpp"

namespace hfc::core::device {
enum class SteamDeckSku {
    LCD,
    OLED,
};

class SteamDeckInterface final : public BaseDeviceInterface {
public:
    SteamDeckInterface();
    ~SteamDeckInterface() override = default;

    std::string getDeviceName() const override;

    std::uint64_t getMinimumFanSpeed() override;
    std::uint64_t getMaximumFanSpeed() override;
    std::uint64_t getCurrentTemperature() override;
    std::uint64_t getCurrentFanSpeed() override;

    void setFanSpeed(std::uint64_t speed_rpm) override;

private:
    SteamDeckSku m_sku;
};
}  // namespace hfc::core::device

#endif  // STEAMDECKINTERFACE_HPP
