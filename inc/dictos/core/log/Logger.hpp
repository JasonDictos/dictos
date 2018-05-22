#pragma once

namespace dictos::log {

inline Logger::Logger(config::Options options) :
	Context(getSection(), std::move(options)) {
}

template<class ...Lvls>
inline bool Logger::isLevelEnabled(const Lvls&... lvls) {
	bool enabled = false;
	auto isEnabled = [&enabled,this](const std::string_view &t) {
		auto lvl_str = std::string(t.begin(), t.end());
		if (lvl_str == "CRITICAL" || m_enabledLevels.find(lvl_str) != m_enabledLevels.end())
			enabled = true;
	};

	auto guard = m_lock.lock();
	(isEnabled(lvls), ...);

	return enabled;
}

template<class ...Lvls>
inline void Logger::enableLevel(const Lvls&... lvls) {
	auto enable =
		[this](const std::string_view &t) {
			auto lvl_str = std::string(t.begin(), t.end());
			lvl_str = string::toUpper(lvl_str);
			m_enabledLevels.insert(lvl_str);
			if (lvl_str == "ALL")
				m_allLevelsEnabled = true;
		};
	auto guard = m_lock.lock();
	(enable(lvls), ...);
}

template<class ...Lvls>
inline void Logger::disableLevel(const Lvls&... lvls) {
	auto disable =
		[this](const std::string_view &t) {
			auto lvl_str = std::string(t.begin(), t.end());
			m_enabledLevels.erase(lvl_str);
			if (lvl_str == "ALL")
				m_allLevelsEnabled = false;
		};
	auto guard = m_lock.lock();
	(disable(lvls), ...);
}

template<class Prefix, class ...Args>
inline void Logger::writePrefix(const Prefix &prefix, const Args&... args) {
	auto t = std::time(nullptr);
	write("["s + getPrefix(prefix) + "]", args...);
}

template<class ...Args>
inline void Logger::write(const Args&... args) {
	auto logLine = std::string(" ") + string::toStringDelimiter(' ', args...);
	logLine = string::trimTrailing(logLine) + '\n';

	auto t = std::time(nullptr);
	std::stringstream stream;
	stream << std::put_time(std::localtime(&t), "%Y/%m/%d %T") << logLine;
#if defined(DCORE_PLAT_WIN)
	OutputDebugStringA(logLine.c_str());
#endif
	printf("%s", stream.str().c_str());
	fflush(stdout);
}

}
