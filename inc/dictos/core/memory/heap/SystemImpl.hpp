#pragma once

namespace dictos::memory::heap {

/**
 * The system impl is a basic alloc/free hook into malloc.
 */
class SystemImpl
{
public:
	template<class T = std::byte>
	static inline T* allocate(Size size)
	{
		if (size.isNegative())
			DCORE_THROW(InvalidArgument, "Cannot allocate a buffer of negative size:", size);

		auto mem = std::malloc(size * sizeof(T));
		if (!mem)
			DCORE_THROW(BadAllocation, "Failed to allocate:", size);
		return static_cast<T *>(mem);
	}

	template<class T = std::byte>
	static inline void release(T **mem)
	{
		if (!mem || !*mem)
			return;

		std::free(*mem);
		*mem = nullptr;
	}
};

}
