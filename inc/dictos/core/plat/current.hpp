#pragma once

namespace dictos::plat {

enum class TYPE
{
	Windows,
	Linux,
	Mac
};

/**
 * Returns the current platform type as a constexpr
 * so it can be used in code to do platform specific
 * logic using constexpr semantics.
 */
constexpr TYPE getCurrentType()
{
#if defined(DCORE_PLAT_WIN)
	return TYPE::Windows;
#elif defined(DCORE_PLAT_LIN)
	return TYPE::Linux;
#elif defined(DCORE_PLAT_MAC)
	return TYPE::Mac;
#else
	#error "Unsupported platform"
#endif
}

}
