#pragma once

namespace dictos::string {

inline std::pair<std::string, std::string> split(
	const std::string_view &delim,
	const std::string_view &str)
{
	std::pair<std::string, std::string> result;

	auto chopOffset = str.find(delim);
	if (chopOffset == std::string_view::npos)
		return result;

	auto chopLen = delim.size();

	result.second = str;
	result.first = result.second.substr(0, chopOffset);
	result.second = result.second.substr(chopOffset + chopLen);

	return result;
}

}
