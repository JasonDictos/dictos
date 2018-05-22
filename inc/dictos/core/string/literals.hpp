#pragma once

namespace dictos::string::literals {

using namespace std::literals::string_literals;

// String view const useable in compile time templates (neato).
constexpr std::string_view operator "" _sv(const char* path, std::size_t count)
{
	return std::string_view(path, count);
}

}
