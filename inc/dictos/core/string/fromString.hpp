#pragma once

namespace dictos::string {

template<class T>
inline T fromString(const std::string_view &string) {
	// @@ TODO
	return boost::lexical_cast<T>(string);
}

}
