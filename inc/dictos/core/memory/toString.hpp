#pragma once

namespace dictos::string {

template<>
inline std::string toString(const memory::Heap &heap)
{
	return std::string(heap.begin(), heap.end());
}

}
