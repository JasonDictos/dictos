#pragma once

namespace dictos::literals {

inline Size operator "" _tb(long double count)
{
	return Size::terabytes(count);
}

inline Size operator "" _gb(long double count)
{
	return Size::gigabytes(count);
}

inline Size operator "" _mb(long double count)
{
	return Size::megabytes(count);
}

inline Size operator "" _kb(long double count)
{
	return Size::kilobytes(count);
}

inline Size operator "" _b(long double count)
{
	return Size::bytes(count);
}

inline Size operator "" _tb(unsigned long long count)
{
	return Size::terabytes(boost::numeric_cast<long double>(count));
}

inline Size operator "" _gb(unsigned long long count)
{
	return Size::gigabytes(boost::numeric_cast<long double>(count));
}

inline Size operator "" _mb(unsigned long long count)
{
	return Size::megabytes(boost::numeric_cast<long double>(count));
}

inline Size operator "" _kb(unsigned long long count)
{
	return Size::kilobytes(boost::numeric_cast<long double>(count));
}

inline Size operator "" _b(unsigned long long count)
{
	return Size::bytes(boost::numeric_cast<long double>(count));
}

}
