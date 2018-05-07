#if defined(DCORE_PLAT_WIN)
	#include <dictos/core/plat/windows.hpp>
#elif defined(DCORE_PLAT_LIN)
	#include <dictos/core/plat/linux.hpp>
#elif defined(DCORE_PLAT_MAC)
	#include <dictos/core/plat/mac.hpp>
#else
	#error "Unsupported platform"
#endif

#include <dictos/core/plat/current.hpp>
