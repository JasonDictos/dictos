#include <boost/stacktrace.hpp>

#include <cassert>
#include <iostream>
#include <thread>
#include <atomic>

#if defined(DCORE_PLAT_LINUX)
	#include <signal.h>
#endif
