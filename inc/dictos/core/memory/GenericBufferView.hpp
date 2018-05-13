#pragma once

namespace dictos::memory {

/**
 * A generic buffer view which does not own the data (just like std::string_view). Usefule
 * when you want to avoid buffer copies. A buffer view is an immutable object.
 */
template<class Impl>
class GenericBufferView
{
public:
	GenericBufferView(const void *data, Size size) :
		m_buffer(static_cast<const std::byte *>(data)), m_size(size)
	{
	}

	GenericBufferView(GenericBufferView<Impl> &&buf)
	{
		operator = (std::move(buf));
	}

	GenericBufferView(const GenericBufferView<Impl> &buf)
	{
		operator = (buf);
	}

	GenericBufferView(const GenericBuffer<Impl> &buf)
	{
		operator = (buf);
	}

	GenericBufferView(const std::string &str)
	{
		m_begin = str.front();
		m_size = str.size();
	}

	std::string __toString() const noexcept
	{
		return std::string(m_size, m_buffer);
	}

	GenericBufferView<Impl> & operator = (const GenericBuffer<Impl> &buf)
	{
		m_buffer = buf.cast<std::byte>();
		m_size = buf.size();
		return *this;
	}

	GenericBufferView<Impl> & operator = (const GenericBufferView<Impl> &buf)
	{
		if (this == &buf)
			return *this;

		m_buffer = buf.m_buffer;
		m_size = buf.m_size;
		return *this;
	}

	GenericBufferView<Impl> & operator = (GenericBufferView<Impl> &&buf)
	{
		m_buffer = buf.m_buffer;
		m_size = buf.m_size;
		buf.m_buffer = nullptr;
		buf.m_size = 0;
		return *this;
	}

	Size size() const noexcept { return m_size; }

	template<class ImplType>
	bool operator != (const GenericBuffer<ImplType> &buf) const noexcept
	{
		return !(operator == (buf));
	}

	template<class ImplType>
	bool operator == (const GenericBuffer<ImplType> &buf) const noexcept
	{
		if (m_size != buf.size())
			return false;

		if (m_buffer == buf.cast<std::byte>())
			return true;

		return !std::memcmp(m_buffer, buf.cast<std::byte>(), m_size);
	}

	template<class ImplType>
	int compare(const GenericBuffer<ImplType> &buf) const noexcept
	{
		if (m_size < buf.size())
			return -1;
		if (m_size > buf.size())
			return 1;
		if (m_buffer == buf.cast<std::byte>())
			return 0;

		return std::memcmp(m_buffer, buf.cast<std::byte>(), m_size);
	}

	template<class ImplType>
	bool operator != (const GenericBufferView<ImplType> &buf) const noexcept
	{
		return !(operator == (buf));
	}

	template<class ImplType>
	bool operator == (const GenericBufferView<ImplType> &buf) const noexcept
	{
		if (m_size != buf.m_size)
			return false;

		if (m_buffer == buf.m_buffer)
			return true;

		return !std::memcmp(m_buffer, buf.m_buffer, m_size);
	}

	template<class ImplType>
	int compare(const GenericBufferView<ImplType> &buf) const noexcept
	{
		if (m_size < buf.m_size)
			return -1;
		if (m_size > buf.m_size)
			return 1;
		if (m_buffer == buf.m_buffer)
			return 0;

		return std::memcmp(m_buffer, buf.m_buffer, m_size);
	}

	template<class T>
	auto cast() { return reinterpret_cast<const T *>(m_buffer); }

	template<class T>
	auto cast() const { return reinterpret_cast<const T *>(m_buffer); }

	const std::byte *begin() const { return &m_buffer[0]; }
	const std::byte *end() const { return &m_buffer[m_size]; }

	const std::byte *begin() { return &m_buffer[0]; }
	const std::byte *end() { return &m_buffer[m_size]; }

protected:
	Size m_size;
	const std::byte *m_buffer = nullptr;
};

}
