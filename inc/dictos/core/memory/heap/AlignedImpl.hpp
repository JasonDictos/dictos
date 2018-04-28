#pragma once

namespace dictos::memory::heap {

/**
 * The algined impl allocates aligned memory on a boundary
 * that is part of the type.
 */
template<size_t Alignment = 4096>
class AlignedImpl
{
public:
	template<class T = std::byte>
	static inline T* allocate(Size size)
	{
		if (size.isNegative())
			DCORE_THROW(InvalidArgument, "Cannot allocate a buffer of negative size:", size);

		if constexpr(plat::getCurrentType() == plat::TYPE::Linux)
		{
			auto mem = aligned_alloc(size * sizeof(T), Alignment);
			if (!mem)
				DCORE_THROW(BadAllocation, "Failed to allocate:", size);
			return static_cast<T *>(mem);
		}

		DCORE_THROW(RuntimeError, "Current platform does not support aligned memory");
	}

	template<class T = std::byte>
	static inline void release(T **mem)
	{
		if constexpr(plat::getCurrentType() == plat::TYPE::Linux)
		{
			if (!mem || !*mem)
				return;

			std::free(*mem);
			*mem = nullptr;
			return;
		}

		DCORE_THROW(RuntimeError, "Current platform does not support aligned memory");
	}
};

}
