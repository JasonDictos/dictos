#include "dictos/core/all.hpp"

using namespace dictos;

DPUB dev::BasicSpinLock::Guard log::checkRecursion()
{
	static dev::BasicSpinLock s_guard;
	return s_guard.checkRecursionLock();
}

DPUB log::Logger *log::GlobalLogger()
{
	static log::Logger logger;
	return &logger;
}
