#pragma once

namespace dictos {

/**
 * The size object represents a number of bytes, and provides various
 * means of converting its value, it will also self identify as a string
 * convertd size string (e.g. 1GB).
 */
class Size
{
public:
	template<typename SizeType>
	constexpr Size(SizeType byteValue) : m_byteValue(boost::numeric_cast<int64_t>(byteValue)) {}

	constexpr Size() : m_byteValue(0) { }

	constexpr operator int64_t () { return m_byteValue; }

	static constexpr Size bytes(long double number) { return Size(number); }
	static constexpr Size kilobytes(long double number) { return Size(number * 1024); }
	static constexpr Size megabytes(long double number) { return Size(number * 1024 * 1024); }
	static constexpr Size gigabytes(long double number) { return Size(number * 1024 * 1024 * 1024); }
	static constexpr Size terabytes(long double number) { return Size(number * 1024 * 1024 * 1024 * 1024); }

	Size(const Size &size) :
		m_byteValue(size.m_byteValue)
	{
	}

	Size & operator = (const Size &size)
	{
		m_byteValue = size.m_byteValue;
		return *this;
	}

	Size & operator += (const Size &size)
	{
		m_byteValue += size.m_byteValue;
		return *this;
	}

	Size & operator -= (const Size &size)
	{
		m_byteValue -= size.m_byteValue;
		return *this;
	}

	Size operator + (const Size &size) const noexcept
	{
		Size newSize(m_byteValue);

		newSize.m_byteValue += size.m_byteValue;

		return newSize;
	}

	Size operator - (const Size &size) const noexcept
	{
		Size newSize(m_byteValue);

		newSize.m_byteValue -= size.m_byteValue;

		return newSize;
	}

	template<class T = size_t>
	T asBytes() const
	{
		return numericCast<T>(m_byteValue);
	}

	constexpr bool operator < (const Size &size) const noexcept
	{
		return m_byteValue < size.m_byteValue;
	}

	constexpr bool operator <= (const Size &size) const noexcept
	{
		return m_byteValue <= size.m_byteValue;
	}

	constexpr bool operator > (const Size &size) const noexcept
	{
		return m_byteValue > size.m_byteValue;
	}

	constexpr bool operator >= (const Size &size) const noexcept
	{
		return m_byteValue >= size.m_byteValue;
	}

	constexpr bool operator == (const size_t &size) const noexcept
	{
		return m_byteValue == size;
	}

	constexpr bool operator == (const int &size) const noexcept
	{
		return m_byteValue == size;
	}

	constexpr bool operator == (const Size &size) const noexcept
	{
		return m_byteValue == size.m_byteValue;
	}

	constexpr bool operator != (const Size &size) const noexcept
	{
		return m_byteValue != size.m_byteValue;
	}

	double operator / (const time::seconds duration) const noexcept
	{
		return m_byteValue / static_cast<double>(duration.count());
	}

	explicit constexpr operator bool () const noexcept
	{
		 return m_byteValue != 0;
	}

	constexpr bool isNegative() const noexcept
	{
		return m_byteValue < 0;
	}

	constexpr bool isPositive() const noexcept
	{
		return m_byteValue > 0;
	}

	constexpr bool isZero() const noexcept
	{
		return m_byteValue == 0;
	}

	std::string __toString() const
	{
		return string::toHumanSize(m_byteValue, 2);
	}

	operator std::size_t () const { return m_byteValue; }

protected:
	int64_t m_byteValue;
};

}
