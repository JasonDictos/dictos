#pragma once

namespace dictos::string {

/**
 * Appends the variadic ards together, using toString semantics
 * on each, aand appends a trailing character delimiter type.
 */
template<char Trailing, class ...Args>
inline std::string append(Args&&... args)
{
	std::stringstream stream;

	auto convertEntry =
		[&stream](const auto &entry)
		{
			stream << toString(entry) << Trailing;
		};

	(convertEntry(args), ...);

	return stream.str();
}

}
