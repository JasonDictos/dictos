#pragma once

namespace dictos::memory {

/**
 * A generic buffer represents a contiguous peice of memory.
 */
template<class Impl>
class GenericBuffer
{
public:
	GenericBuffer() = default;

	template<class IterB, class IterE>
	GenericBuffer(IterB start, IterE end)
	{
		grow(std::distance(start, end));

		if (isContiguous(start, end)) 
			std::memcpy(m_buffer, &*start, size());
		else {
			auto this_iter = begin();
			while (start != end) {
				*this_iter = boost::numeric_cast<std::byte>(*start);
				this_iter++;
				start++;
			}
		}
	}

	GenericBuffer(Size size)
	{
		grow(size);
	}

	GenericBuffer(const std::string &str)
	{
		grow(str.size());
		copy(str.c_str(), str.size());
	}

	std::string __toString() const noexcept
	{
		return std::string(m_size, m_buffer);
	}

	Size size() const noexcept { return m_size; }

	template<class T>
	void copy(const T &data, Size size)
	{
		static_assert(std::is_pod_v<T>, "Type must be POD for copy into buffer");
		if (m_size < size)
			grow(size - m_size);

		std::memcpy(m_buffer, reinterpret_cast<const void *>(data), size);
	}

	template<class T>
	void copyAt(const T &data, Size size)
	{
		static_assert(std::is_pod_v<T>, "Type must be POD for copy into buffer");
		if (m_size < size)
			grow(size - m_size);

		std::memcpy(m_buffer, reinterpret_cast<const void *>(data), size);
	}

	template<class ImplType>
	bool operator != (const GenericBuffer<ImplType> &buf) const noexcept
	{
		if (m_size == buf.m_size)
			return false;

		return std::memcmp(m_buffer, buf.m_buffer, m_size);
	}

	template<class ImplType>
	bool operator == (const GenericBuffer<ImplType> &buf) const noexcept
	{
		if (m_size != buf.m_size)
			return false;

		return !std::memcmp(m_buffer, buf.m_buffer, m_size);
	}

	template<class ImplType>
	int compare(const GenericBuffer<ImplType> &buf) const noexcept
	{
		if (m_size < buf.m_size)
			return -1;
		if (m_size > buf.m_size)
			return 1;

		return std::memcmp(m_buffer, buf.m_buffer, m_size);
	}

	template<class ImplType>
	GenericBuffer<Impl> &operator = (const GenericBuffer<ImplType> &buf)
	{
		release();
		grow(buf.m_size);
		std::memcpy(m_buffer, buf.m_buffer, buf.m_size);
		return *this;
	}

	void grow(Size amount)
	{
		// Allocate a new buffer of current + amount
		auto newSize = m_size + amount;
		auto newBuffer = Impl::allocate(newSize);

		// Copy our old buffer to it
		std::memcpy(newBuffer, m_buffer, m_size);

		// And release the old buffer now
		Impl::release(&m_buffer);

		// Apply the new buffer to this
		m_buffer = newBuffer;
		m_size = newSize;
	}

	void growLeading(Size amount)
	{
		// Allocate a new buffer of current + amount
		auto newSize = m_size + amount;
		auto newBuffer = Impl::allocate(newSize);

		// Copy our old buffer to it
		std::memcpy(newBuffer + amount.asBytes(), m_buffer, m_size);

		// And release the old buffer now
		Impl::release(&m_buffer);

		// Apply the new buffer to this
		m_buffer = newBuffer;
		m_size = newSize;
	}

	void shrink(Size amount)
	{
		// Allocate a new buffer of current - amount
		auto newSize = m_size - amount;
		if (newSize.isNegative())
			DCORE_THROW(InvalidArgument, "Cannot shrink buffer down by:", amount, "Current size:", m_size);

		auto newBuffer = Impl::allocate(newSize);

		// Copy our old buffer to it
		std::memcpy(newBuffer, m_buffer, newSize);

		// And release the old buffer now
		Impl::release(&m_buffer);

		// Apply the new buffer to this
		m_buffer = newBuffer;
		m_size = newSize;
	}

	void shrinkLeading(Size amount)
	{
		// Allocate a new buffer of current - amount
		auto newSize = m_size - amount;
		if (newSize.isNegative())
			DCORE_THROW(InvalidArgument, "Cannot shrink buffer down by:", amount, "Current size:", m_size);

		auto newBuffer = Impl::allocate(newSize);

		// Copy our old buffer to it
		std::memcpy(newBuffer, m_buffer + amount.asBytes(), newSize);

		// And release the old buffer now
		Impl::release(&m_buffer);

		// Apply the new buffer to this
		m_buffer = newBuffer;
		m_size = newSize;
	}

	void memset(int val)
	{
		if (m_buffer)
			std::memset(m_buffer, val, m_size);
	}

	void release()
	{
		Impl::release(&m_buffer);
		m_size = Size();
	}

	template<class T>
	T * cast()
	{
		return reinterpret_cast<T *>(m_buffer);
	}

	template<class T>
	const T * cast() const
	{
		return reinterpret_cast<const T *>(m_buffer);
	}

	const std::byte *begin() const { return &m_buffer[0]; }
	const std::byte *end() const { return &m_buffer[m_size]; }

	std::byte *begin() { return &m_buffer[0]; }
	std::byte *end() { return &m_buffer[m_size]; }

protected:
	Size m_size;
	std::byte *m_buffer = nullptr;
};

}
