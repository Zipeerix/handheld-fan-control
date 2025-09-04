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

#ifndef ERROR_HPP
#define ERROR_HPP

#include <format>
#include <string>
#include <type_traits>

namespace hfc {
template <typename T>
concept IsEnum = std::is_enum_v<T>;

template <IsEnum ErrorType>
struct Error {
    ErrorType type;
    std::string message;

    Error() = default;

    explicit Error(ErrorType error_type, std::string error_message = "") :
        type(error_type),
        message(std::move(error_message)) {
    }
};
}  // namespace hfc

template <hfc::IsEnum ErrorType>
struct std::formatter<hfc::Error<ErrorType> > {
    static constexpr auto parse(const std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const hfc::Error<ErrorType>& error, std::format_context& ctx) const {
        const auto error_type_name = std::format("{}", error.type);
        if (error.message.empty()) {
            return std::format_to(ctx.out(), "Error: Type {}", error_type_name);
        }

        return std::format_to(ctx.out(), "Error: Type {}, Message: \"{}\"", error_type_name, error.message);
    }
};

#endif  // ERROR_HPP
