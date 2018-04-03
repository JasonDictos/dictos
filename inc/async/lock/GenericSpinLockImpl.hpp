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
		if (getCount() && getOwnerId() == Impl::getCurrentId())
		{
			inc();
			return true;
		}

		return inc(!m_flag.test_and_set(std::memory_order_acquire));
	}

	void lock() noexcept override
	{
		// Re-use the spin logic in try by just maxing out ms
		tryLock(time::milliseconds::max());
	}

	void unlock() noexcept override
	{
		DCORE_ASSERT(getOwnerId() == Impl::getCurrentId());
		DCORE_ASSERT(getCount() != 0);

		if (getCount() == 1)
			m_flag.clear();

		dec();
	}

protected:
	std::atomic_flag m_flag = {false};
};

}
