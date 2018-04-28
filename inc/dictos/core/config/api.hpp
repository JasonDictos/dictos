#pragma once

namespace dictos::config {

inline void put(const std::string &key, std::any value)
{
}

template<class T>
inline T get(const std::string &key)
{
	return T();
}

}
