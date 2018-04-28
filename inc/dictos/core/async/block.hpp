#pragma once

namespace dictos::error {

/**
 * Create a specialization for block with Callback.
 */
template<>
inline std::exception_ptr block(async::Callback &callback)
{
	try
	{
		callback.invoke();
		return std::exception_ptr();
	}
	catch (...)
	{
		return std::current_exception();
	}
}

}
