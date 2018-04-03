#pragma once

namespace dictos::string {

template<char Leading, char Trailing = Leading, class ...Args>
inline std::string enclose(Args&&... args)
{
	std::stringstream stream;

	auto convertEntry =
		[&stream](const auto &entry)
		{
			stream << Leading << toString(entry) << Trailing;
		};

	(convertEntry(args), ...);

	return stream.str();
}

}
