#pragma once

namespace dictos::string {

template<class T>
inline void recursiveCommas(std::ostream& os, T n)
{
	T rest = n % 1000; //"last 3 digits"
	n /= 1000;         //"begining"

	if (n > 0) {
		recursiveCommas(os, n); //printing "begining"

		//and last chunk
		os << ',' << std::setfill('0') << std::setw(3) << rest;
	}
	else
		os << rest; //first chunk of the number
}

template<class NumType = int64_t>
inline std::string toHumanCount(NumType value)
{
	std::ostringstream stream;
	recursiveCommas<int64_t>(stream, boost::numeric_cast<int64_t>(value));
	return stream.str();
}


template<class NumType = int64_t>
inline std::string toHumanSize(NumType size, int max_precision)
{
	const char* units[] = {"B", "kB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};

	int i = 0;
	while (size >= 1024 && i < sizeof(units) / sizeof(char *)) {
		size /= 1024;
		i++;
	}

	return toString(size, units[i]);
}

}
