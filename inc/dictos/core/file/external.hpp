#if defined(DCORE_PLAT_WIN)
	#include <experimental/filesystem> // C++-standard header file name
	#include <filesystem> // Microsoft-specific implementation header file name
#else
	#include <experimental/filesystem>
#endif

