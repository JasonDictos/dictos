#pragma once

namespace dictos::string {

template<class T>
inline T fromString(const std::string_view &string)
{
	return boost::lexical_cast<T>(string);
}

}
