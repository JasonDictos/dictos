#pragma once

namespace dictos {

/**
 * The count object represents a counted number that is rendered
 * as a human delimited string. It will also self initialize to zero.
 */
class Count
{
public:
	template<typename CountType>
	constexpr Count(CountType byteValue) : m_countValue(boost::numeric_cast<int64_t>(byteValue)) {}

	constexpr Count() : m_countValue(0) { }

	constexpr operator int64_t () { return m_countValue; }

	static constexpr Count bytes(long double number) { return Count(number); }
	static constexpr Count kilobytes(long double number) { return Count(number * 1024); }
	static constexpr Count megabytes(long double number) { return Count(number * 1024 * 1024); }
	static constexpr Count gigabytes(long double number) { return Count(number * 1024 * 1024 * 1024); }
	static constexpr Count terabytes(long double number) { return Count(number * 1024 * 1024 * 1024 * 1024); }

	Count(const Count &size) :
		m_countValue(size.m_countValue)
	{
	}

	Count & operator = (const Count &size)
	{
		m_countValue = size.m_countValue;
		return *this;
	}

	Count & operator += (const Count &size)
	{
		m_countValue += size.m_countValue;
		return *this;
	}

	Count & operator -= (const Count &size)
	{
		m_countValue -= size.m_countValue;
		return *this;
	}

	Count operator + (const Count &size) const noexcept
	{
		Count newCount(m_countValue);

		newCount.m_countValue += size.m_countValue;

		return newCount;
	}

	Count operator - (const Count &size) const noexcept
	{
		Count newCount(m_countValue);

		newCount.m_countValue -= size.m_countValue;

		return newCount;
	}

	template<class T = size_t>
	T asBytes() const
	{
		return numericCast<T>(m_countValue);
	}

	constexpr bool operator < (const Count &size) const noexcept
	{
		return m_countValue < size.m_countValue;
	}

	constexpr bool operator <= (const Count &size) const noexcept
	{
		return m_countValue <= size.m_countValue;
	}

	constexpr bool operator > (const Count &size) const noexcept
	{
		return m_countValue > size.m_countValue;
	}

	constexpr bool operator >= (const Count &size) const noexcept
	{
		return m_countValue >= size.m_countValue;
	}

	constexpr bool operator == (const Count &size) const noexcept
	{
		return m_countValue == size.m_countValue;
	}

	constexpr bool operator == (const size_t &size) const noexcept
	{
		return m_countValue == size;
	}

	constexpr bool operator == (const int &size) const noexcept
	{
		return m_countValue == size;
	}

	constexpr bool operator != (const Count &size) const noexcept
	{
		return m_countValue != size.m_countValue;
	}

	double operator / (const time::seconds duration) const noexcept
	{
		return m_countValue / static_cast<double>(duration.count());
	}

	explicit constexpr operator bool () const noexcept
	{
		 return m_countValue != 0;
	}

	constexpr bool isNegative() const noexcept
	{
		return m_countValue < 0;
	}

	constexpr bool isPositive() const noexcept
	{
		return m_countValue > 0;
	}

	constexpr bool isZero() const noexcept
	{
		return m_countValue == 0;
	}

	std::string __toString() const
	{
		return toHumanCount(m_countValue);
	}

	operator std::size_t () const { return m_countValue; }

	static std::string toHumanCount(int64_t value) 
	{
		auto strVal = string::toString(value);
		for (size_t i = 3; i < strVal.size(); i += 3) {
			strVal.insert(i, 1, ',');
		}
		return strVal;
	}

protected:
	int64_t m_countValue;
};

}
