#pragma once

namespace dictos::dev {

/**
 * This is the end all call, the app will exit, the debugger will be entered into
 * if its attached.
 */
inline void fatality(const std::string &reason, const char *filename, int line)
{
	log::write(reason, "File:", filename, "Line:", line);
#if defined(DCORE_PLAT_WIN)
	DebugBreak();
#else
	kill(getpid(), 2);
#endif
	abort();
}

}
