#pragma once

inline std::ostream & operator << (std::ostream &stream, const std::string_view &string)
{
	for (auto &chr : string)
		stream << chr;
	return stream;
}
