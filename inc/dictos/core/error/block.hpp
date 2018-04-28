#pragma once

namespace dictos::error {

template<class Call, class ...Args>
inline std::exception_ptr block(Call &call, Args&&... args)
{
	try
	{
		std::invoke(call, std::forward<Args>(args)...);
		return std::exception_ptr();
	}
	catch (...)
	{
		return std::current_exception();
	}
}

}
