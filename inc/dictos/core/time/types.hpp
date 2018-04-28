#pragma once

namespace dictos::time {

using namespace std::chrono;

// Create a shortcut type to specialize our simpler templates for the 'now' type
enum class CLOCK
{
	Precise,
	Utc
};

/**
 * Returns the current time with either Precise or System clocks. A precise clock
 * will have a higher granularity then seconds, where the system clock will simply
 * count seconds. Precise should be used for any timing operation.
 */
template<CLOCK ClockType = CLOCK::Precise>
inline auto now()
{
	if constexpr(ClockType == CLOCK::Precise)
		return std::chrono::high_resolution_clock::now();
	else if constexpr(ClockType == CLOCK::Utc)
		return std::chrono::system_clock::now();
	else
		DCORE_ASSERT(!"Invalid clock type");
}

}

// Create a shortcut to the time literals
namespace dictos::time::literals {

using namespace std::literals::chrono_literals;

}
