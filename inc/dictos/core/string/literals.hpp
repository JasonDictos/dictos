#pragma once

namespace dictos::string::literals {

using namespace std::literals::string_literals;

constexpr std::string_view operator "" _sv(const char* path, std::size_t count)
{
	return std::string_view(path, count);
}

}
