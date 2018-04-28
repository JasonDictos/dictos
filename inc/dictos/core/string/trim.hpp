#pragma once

namespace dictos::string {

template<class Arg>
inline std::string trimTrailing(const Arg &arg)
{
	auto str = toString(arg);

	forever
	{
		switch (str.back())
		{
			case '\n':
			case '\r':
			case ' ':
				str = str.substr(0, str.size() - 1);
				break;

			default:
				return str;
		}
	}

	return str;
}

template<class Arg>
inline std::string trimLeading(const Arg &arg)
{
	auto str = toString(arg);

	forever
	{
		switch (str.front())
		{
			case '\n':
			case '\r':
			case ' ':
				str = str.substr(1);
				break;

			default:
				return str;
		}
	}

	return str;
}

template<class Arg>
inline std::string trim(const Arg &arg)
{
	auto str = toString(arg);
	return trimTrailing(trimLeading(str));
}


}
