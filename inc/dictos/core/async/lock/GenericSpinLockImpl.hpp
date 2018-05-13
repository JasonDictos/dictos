#pragma once

namespace dictos::async::lock {

/**
 * The generic spin lock class provides some common business logic
 * to be shared between any implementation of a spin lock from fiber/thread
 * trait specializations.
 */
template<class Impl>
class GenericSpinLockImpl : public GenericLockImpl<Impl>
{
public:
	using Parent = GenericLockImpl<Impl>;
	using IdType = typename Parent::IdType;
	static constexpr const int InitialSpinCount = 50;

	using Parent::getCount;
	using Parent::getOwnerId;
	using Parent::inc;
	using Parent::dec;

	template<class Rep, class Period>
	inline bool tryLock(time::duration<Rep, Period> wait)
	{
		auto start = time::high_resolution_clock::now();

		auto tryCount = 0;
		while (!tryLock())
		{
			auto waited = time::high_resolution_clock::now() - start;

			if (wait != time::milliseconds::max() && waited > wait)
				return false;

			if (tryCount > InitialSpinCount)
			{
				// Sleep a bit more if we've waited a bunch
				if (waited > time::seconds(5))
					Impl::sleep(time::milliseconds(50));
				else
					Impl::yield();
			}
			else
				tryCount++;
		}

		return true;
	}

	bool tryLock() noexcept
	{
		auto currentId = Impl::getCurrentId();
		auto count = getCount();

		// First lock the transition flag, this is a micro lock to protect
		// state in the spinlock (e.g. current value of ownerId etc.)
		while (m_transitionFlag.test_and_set(std::memory_order_acquire)) {}

#if DCORE_PLAT_WIN
		DCORE_ASSERT(m_transitionFlag._My_flag);
#endif

		// Now that we have the transition flag set we can check the current id and figure out
		// if we are recursing
		if (getCount() && getOwnerId() == Impl::getCurrentId())
		{
			inc();

			// Ok all done clear the transition flag
			m_transitionFlag.clear();
			return true;
		}

		// Not recursing, acquire the locked flag after releasing the transition flag
		m_transitionFlag.clear();

		// Attempt t grab the lock and inc at the same time
		return inc(!m_lockedFlag.test_and_set(std::memory_order_acquire));
	}

	void lock() noexcept override
	{
		// Re-use the spin logic in try by just maxing out ms
		tryLock(time::milliseconds::max());
	}

	void unlock() noexcept override
	{
		// First lock the transition flag so we can check state
		while (m_transitionFlag.test_and_set(std::memory_order_acquire)) {}

		// Now check our assumptions
		DCORE_ASSERT(getOwnerId() == Impl::getCurrentId());
		DCORE_ASSERT(getCount() != 0);

		// Safe to dec
		dec();

		// On last dec, clear our locked flag
		if (getCount() == 0) {
			m_lockedFlag.clear();
		}

		// All done, clear the transition flag
		m_transitionFlag.clear();
	}

protected:
	std::atomic_flag m_lockedFlag = {false};
	std::atomic_flag m_transitionFlag = {false};
};

}
