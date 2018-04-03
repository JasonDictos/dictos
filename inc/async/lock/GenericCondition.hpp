#pragma once

namespace dictos::async::lock {

/**
 * The generic condition object is a waitable event that can be signalled and
 * is bound to a lock, it requires an api trait to become hole.
 */
template<class Impl>
class GenericCondition : public GenericImpl<Impl>
{
public:
	using SystemConditionType = typename Impl::SystemConditionType;
	using GuardType = GenericGuard<Impl>;

	/**
	 * Wakes up all waiters.
	 */
	void signal()
	{
		m_cond.notify_all();
	}

	/**
	 * Waits on the condition, will return when someone calls signal.
	 */
	void wait(GuardType &guard, bool ccheck = true)
	{
		auto &impl = *guard.getImpl();
		if (ccheck && async::isCancelled())
			DCORE_THROW(Cancelled, "Thread cancelled");

		// Unroll to 1 lock
		auto count = impl.unroll();
		m_cond.wait(impl.getBackingLock());
		impl.roll(count);
	}

	/**
	 * Waits with a timeout, will return false if it timed out, true if it was signalled.
	 */
	template<class Rep, class Period>
	bool waitFor(GuardType &guard, const time::duration<Rep, Period> &timeout)
	{
		// Unroll to 1 lock
		auto uctx = guard.getImpl()->unroll(guard);
		return m_cond.wait_for(guard, timeout) != std::cv_status::timeout;
	}

protected:

	// Our actual system condition, defined by the trait type
	SystemConditionType m_cond;
};

}
