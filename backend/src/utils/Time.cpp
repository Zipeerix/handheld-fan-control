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

#include "Time.hpp"

#include <google/protobuf/timestamp.pb.h>

namespace hfc::utils {
constexpr static auto k_epoch = boost::posix_time::ptime(boost::gregorian::date(1'970, 1, 1));

TimePoint getCurrentUtcTime() {
    return boost::posix_time::second_clock::universal_time();
}

TimePoint getEpochTime() {
    return k_epoch;
}

bool isInPast(const TimePoint& time) {
    return time < getCurrentUtcTime();
}

google::protobuf::Timestamp convertTimePointToProtobufTimestamp(const boost::posix_time::ptime& time) {
    const auto duration = time - k_epoch;

    google::protobuf::Timestamp timestamp;
    timestamp.set_seconds(duration.total_seconds());
    timestamp.set_nanos((duration.total_microseconds() % 1'000'000) * 1'000);

    return timestamp;
}

google::protobuf::Timestamp* convertTimePointToAllocatedProtobufTimestamp(const boost::posix_time::ptime& time) {
    return new google::protobuf::Timestamp(convertTimePointToProtobufTimestamp(time));
}

boost::posix_time::ptime convertProtobufTimestampToTimeStamp(const google::protobuf::Timestamp& timestamp) {
    const auto duration =
        boost::posix_time::seconds(timestamp.seconds()) + boost::posix_time::microseconds(timestamp.nanos() / 1'000);
    return k_epoch + duration;
}
}  // namespace hfc::utils
