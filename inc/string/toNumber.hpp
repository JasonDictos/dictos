#pragma once

namespace dictos::string {

template<class Result>
Result toNumber(const std::string_view &str)
{
	static_assert(std::is_arithmetic_v<Result>, "Type must be arithmatic");

	std::stringstream stream;

	if (startsWith(str, "0x", false))
		stream << std::hex;

	Result result;
	stream << str;
	stream >> result;
	return result;
}

}
