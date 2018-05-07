#pragma once

namespace dictos::string {

template<>
inline std::string toString(const memory::Heap &heap)
{
	if (heap.size())
		std::string(heap.cast<char>(), heap.size());
	else
		return std::string();
}

}
