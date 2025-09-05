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

#include "SteamDeckInterface.hpp"

namespace hfc::core::device {
namespace {
// TODO: are they the same? I thought they are different
constexpr auto k_steam_deck_lcd_max_fan_speed = 7'000;
constexpr auto k_steam_deck_oled_max_fan_speed = 7'000;
}  // namespace

SteamDeckInterface::SteamDeckInterface() {
    // TODO: detect sku properly
    m_sku = SteamDeckSku::OLED;
}

std::string SteamDeckInterface::getDeviceName() const {
    return m_sku == SteamDeckSku::OLED ? "Steam Deck (OLED)" : "Steam Deck (LCD)";
}

std::uint64_t SteamDeckInterface::getMinimumFanSpeed() {
    return 0;
}

std::uint64_t SteamDeckInterface::getMaximumFanSpeed() {
    return m_sku == SteamDeckSku::OLED ? k_steam_deck_oled_max_fan_speed : k_steam_deck_lcd_max_fan_speed;
}

std::uint64_t SteamDeckInterface::getCurrentTemperature() {
    throw std::runtime_error("Not implemented");
}

std::uint64_t SteamDeckInterface::getCurrentFanSpeed() {
    throw std::runtime_error("Not implemented");
}

void SteamDeckInterface::setFanSpeed(const std::uint64_t speed_rpm) {
    throw std::runtime_error("Not implemented");
}
}  // namespace hfc::core::device