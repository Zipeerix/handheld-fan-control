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

#ifndef TIME_HPP
#define TIME_HPP

#include <boost/date_time/posix_time/posix_time.hpp>
#include <google/protobuf/timestamp.pb.h>

namespace hfc::utils {
using TimePoint = boost::posix_time::ptime;

TimePoint getCurrentUtcTime();
TimePoint getEpochTime();

bool isInPast(const TimePoint& time);

google::protobuf::Timestamp convertTimePointToProtobufTimestamp(const TimePoint& time);
google::protobuf::Timestamp* convertTimePointToAllocatedProtobufTimestamp(const TimePoint& time);

TimePoint convertProtobufTimestampToTimeStamp(const google::protobuf::Timestamp& timestamp);
}  // namespace hfc::utils

template <>
struct std::formatter<hfc::utils::TimePoint> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const hfc::utils::TimePoint& time, FormatContext& ctx) const {
        return format_to(ctx.out(), "{}Z", boost::posix_time::to_iso_extended_string(time));
    }
};

template <>
struct std::formatter<google::protobuf::Timestamp> {
    bool raw_format = false;

    constexpr auto parse(format_parse_context& ctx) {
        auto it = ctx.begin();
        if (it != ctx.end() && *it == 'r') {
            raw_format = true;
            ++it;
        }
        return it;
    }

    template <typename FormatContext>
    auto format(const google::protobuf::Timestamp& timestamp, FormatContext& ctx) const {
        if (raw_format) {
            return format_to(ctx.out(), "[seconds: {}, nanos: {}]", timestamp.seconds(), timestamp.nanos());
        }

        const auto time = hfc::utils::convertProtobufTimestampToTimeStamp(timestamp);
        return format_to(ctx.out(), "{}Z", boost::posix_time::to_iso_extended_string(time));
    }
};

#endif  // TIME_HPP
