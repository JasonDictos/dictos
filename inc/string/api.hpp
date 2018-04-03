#pragma once

namespace dictos::string {

inline bool startsWith(const std::string_view &haystack, const std::string_view &leading, bool caseAware = true)
{
	return leading.empty() == false && haystack.compare(0, leading.size(), leading) == 0;
}

inline std::string removeLeading(const std::string_view &haystack, const std::string_view &leading, bool caseAware = true)
{
	if (startsWith(haystack, leading))
	{
		auto view = haystack.substr(leading.size());
		return std::string(view.begin(), view.end());
	}
	return std::string(haystack.begin(), haystack.end());
}

inline bool endsWith(const std::string_view &haystack, const std::string_view &trailing, bool caseAware = true)
{
	return trailing.empty() == false && haystack.compare(haystack.size() - trailing.size(), trailing.size(), trailing) == 0;
}

inline std::string removeTrailing(const std::string_view &haystack, const std::string_view &trailing, bool caseAware = true)
{
	if (endsWith(haystack, trailing))
	{
		auto view = haystack.substr(0, trailing.size());
		return std::string(view.begin(), view.end());
	}
	return std::string(haystack.begin(), haystack.end());
}

}
