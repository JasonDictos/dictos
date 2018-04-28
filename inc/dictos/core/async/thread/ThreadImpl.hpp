#pragma once

namespace dictos::async::thread {

/**
 * This thread impl class is the key to allowing threaded specializations for
 * the async space. It provides the lowest level primitive yield/sleep
 * and id apis for all generics.
 */
class ThreadImpl
{
public:
	// GenericImpl - requirements
	using IdType = thread_id;
	using SystemRunnableType = std::thread;
	using SystemConditionType = std::condition_variable_any;
	using SystemMutexType = std::recursive_timed_mutex;

	// Bind our api to this implementation
	template<class Rep, class Int>
	static inline void sleep(time::duration<Rep, Int> wait)
	{
		std::this_thread::sleep_for(wait);
	}

	static inline void yield() noexcept
	{
		std::this_thread::yield();
	}

	static inline thread_id getCurrentId() noexcept
	{
		return std::this_thread::get_id();
	}

	static inline void setName(const std::string &name) noexcept
	{
		// @@ TODO
	}

	static inline std::string getName() noexcept
	{
		// @@ TODO
		return "";
	}
};

}
