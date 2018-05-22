#pragma once

namespace {

template<class Entry>
inline void toStringInternal(std::stringstream &stream, const Entry &entry)
{
	// The priority for how to string something is
	// 1. __toString
	// 2. << operator
	// 4. rtti type name
	if constexpr(dictos::string::HasToStringTrait<decltype(entry)>::value)
		stream << entry.__toString();
	else if constexpr(dictos::string::CanBeOutStreamedTrait<std::stringstream, decltype(entry)>::value)
		stream << entry;
	else
		stream << dictos::log::getTypeName(entry);
}

}

namespace dictos::string {

inline std::string toLower(const std::string_view &string) {
	std::string copy(string.begin(), string.end());
	boost::to_lower(copy);
	return copy;
}

inline std::string toUpper(const std::string_view &string) {
	std::string copy(string.begin(), string.end());
	boost::to_upper(copy);
	return copy;
}

/**
 * This function takes any random args and combines them into a string, allowing a delimiter
 * to be specified (anything that itself can be stringable) to be placed in between the entries.
 */
template<class Delimiter, class ...Args>
inline std::string toStringDelimiter(const Delimiter &delimiter, Args&&... args)
{
	std::stringstream stream;
	bool addDelimiter = false;

	auto convertEntry =
		[&delimiter,&addDelimiter,&stream](const auto &entry)
		{
			if (addDelimiter)
			{
				toStringInternal(stream, delimiter);
				addDelimiter = false;
			}

			toStringInternal(stream, entry);

			addDelimiter = true;
		};

	(convertEntry(args), ...);

	return stream.str();
}

/**
 * This function takes any random args and combines them into a string, no delimiter is
 * used between the converted entries.
 */
template<class ...Args>
inline std::string toString(Args&&... args)
{
	std::stringstream stream;

	auto convertEntry =
		[&stream](const auto &entry)
		{
			toStringInternal(stream, entry);
		};

	(convertEntry(args), ...);

	return stream.str();
}

}
