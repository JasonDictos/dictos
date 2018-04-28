#pragma once

namespace dictos::async {

inline bool isCancelled() noexcept
{
	return ThreadLocal::get()->isCancelled();
}

inline void cancel() noexcept
{
	return ThreadLocal::get()->cancel();
}

inline void checkCancelled()
{
	return ThreadLocal::get()->checkCancelled();
}

template<class Rep, class Int>
inline void sleep(time::duration<Rep, Int> wait, bool ccheck)
{
	return ThreadApi::sleep(wait);
}

inline void yield(bool ccheck)
{
	return ThreadApi::yield();
}

inline Id getCurrentId() noexcept
{
	return ThreadApi::getCurrentId();
}

}
