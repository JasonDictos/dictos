#pragma once

namespace dictos::log {

/**
 * The logger is the thing that sends logs some place, in the default
 * use case it logs to the console or a file (depending on the config).
 */
class Logger : public config::Context
{
public:
	Logger(config::Options = config::Options());

	template<class ...Lvls>
	bool isLevelEnabled(const Lvls&... lvls);

	template<class ...Lvls>
	void enableLevel(const Lvls&... lvls);

	template<class ...Lvls>
	void disableLevel(const Lvls&... lvls);

	template<class Prefix, class ...Args>
	void writePrefix(const Prefix &prefix, const Args&... args);

	template<class ...Args>
	void write(const Args&... args);

protected:
	static const config::Section &getSection()
	{
		if (auto section = config::Sections::find("log"))
			return *section;

		static config::Section section("log", {
				{"consoleOutput",  false, "Writes log output to stdout"},
				{"enabledLevels", std::vector<std::string>(), "Array of enabled level keys"},
				{"dateTimeFormat", std::string(), "Array of enabled level keys"},
			}
		);

		return section;
	}

	static void registerSection();
	mutable async::lock::SpinLock m_lock;
	std::atomic<bool> m_allLevelsEnabled = { false };
	std::set<std::string> m_enabledLevels;
};

}
