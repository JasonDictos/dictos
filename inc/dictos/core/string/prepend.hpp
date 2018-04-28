#pragma once

namespace dictos::string {

template<char Leading, class ...Args>
inline std::string prepend(Args&&... args)
{
	std::stringstream stream;

	auto convertEntry =
		[&stream](const auto &entry)
		{
			stream << Leading << toString(entry);
		};

	(convertEntry(args), ...);

	return stream.str();
}

}
