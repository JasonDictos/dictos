#pragma once

namespace dictos::log {

template<class ...Lvls>
inline bool isLevelEnabled(const Lvls&... lvls)
{
	if (auto canCall = checkRecursion())
		return GlobalLogger()->isLevelEnabled(std::forward<const Lvls&>(lvls)...);;
	return false;
}

template<class ...Lvls>
inline void enableLevel(const Lvls&... lvls)
{
	if (auto canCall = checkRecursion())
		return GlobalLogger()->enableLevel(std::forward<const Lvls&>(lvls)...);
}

template<class ...Lvls>
inline void disableLevel(const Lvls&... lvls)
{
	if (auto canCall = checkRecursion())
		GlobalLogger()->disableLevel(std::forward<const Lvls&>(lvls)...);
}

/**
 * Given some thing, returns a prefix. If its an object
 * and it has a __getPrefix method on it (or if not, then __toString,)
 * that method will be able to set its string result. Falls back
 * to rtti if not a string type to return the type name.
 */
template<class From>
inline std::string getPrefix(const From &from)
{
	if constexpr(HasLogPrefixTrait<From>::value)
		return from.__logPrefix();
	else
		return string::toString(from);
}

/**
 * Logs any number of things to the central logger, using a level to
 * decide if it should be logged, and your own custom prefix 'thing'.
 */
template<class Prefix, class ...Args>
inline void writePrefix(const std::string_view &level, const Prefix &prefix, const Args&... args)
{
	if (auto canCall = checkRecursion())
	{
		auto logger = GlobalLogger();
		if (logger->isLevelEnabled(level))
			logger->writePrefix(prefix, std::forward<const Args&>(args)...);
	}
}

/**
 * Logs any number of things to the central logger, using a level to
 * decide if it should be logged.
 */
template<class ...Args>
inline void writeLevel(const std::string_view &level, const Args&... args)
{
	if (auto canCall = checkRecursion())
	{
		auto logger = GlobalLogger();

		if (logger->isLevelEnabled(level))
			logger->write(std::forward<const Args&>(args)...);
	}
}

/**
 * Logs any number of things to the central logger, always.
 */
template<class ...Args>
inline void write(const Args&... args)
{
	if (auto canCall = checkRecursion())
		GlobalLogger()->write(std::forward<const Args&>(args)...);
}

}
