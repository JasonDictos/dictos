#pragma once

inline std::ostream & operator << (std::ostream &stream, const dictos::time::seconds &seconds) {
	stream << dictos::string::toHumanCount(seconds.count()) << " seconds";
	return stream;
}

inline std::ostream & operator << (std::ostream &stream, const dictos::time::minutes &minutes) {
	stream << dictos::string::toHumanCount(minutes.count()) << " minutes";
	return stream;
}

inline std::ostream & operator << (std::ostream &stream, const dictos::time::hours &hours) {
	stream << dictos::string::toHumanCount(hours.count()) << " hours";
	return stream;
}

inline std::ostream & operator << (std::ostream &stream, const dictos::time::system_clock::time_point duration) {
	auto timet = dictos::time::system_clock::to_time_t(duration);
	stream << std::ctime(&timet);
	return stream;
}
