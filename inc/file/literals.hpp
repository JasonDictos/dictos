#pragma once

namespace dictos::file::literals {

inline path operator "" _path(const char* _path, std::size_t count)
{
	return path(std::string(_path, count));
}

}
