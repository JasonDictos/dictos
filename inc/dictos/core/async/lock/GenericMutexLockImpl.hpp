#pragma once

namespace dictos::async::lock {

/**
 * The generic mutex lock defines the basic business flow for a recursive
 * counting lock.
 */
template<class Impl>
class GenericMutexLockImpl : public GenericLockImpl<Impl>
{
public:
	using SystemMutexType = typename Impl::SystemMutexType;

	using GenericLockImpl<Impl>::getCount;
	using GenericLockImpl<Impl>::getOwnerId;
	using GenericLockImpl<Impl>::inc;
	using GenericLockImpl<Impl>::dec;
	using GenericLockImpl<Impl>::fastFwd;
	using GenericLockImpl<Impl>::m_count;
	using GenericLockImpl<Impl>::m_ownerId;

	template<class Rep, class Period>
	inline bool tryLock(time::duration<Rep, Period> wait)
	{
		return inc(m_mutex.try_lock_for(wait));
	}

	bool tryLock() noexcept
	{
		return inc(m_mutex.try_lock());
	}

	void lock() noexcept override
	{
		m_mutex.lock();
		inc();
	}

	void unlock() noexcept override
	{
		dec();
		m_mutex.unlock();
	}

	uint32_t unroll() noexcept
	{
		LOGT(lock, "UNROLL-Pre:", m_count);
		DCORE_ASSERT(m_count >= 1);
		auto count = m_count.exchange(0) - 1;
		for (uint32_t i = 0; i < count; i++)
			m_mutex.unlock();
		LOGT(lock, "UNROLL-Post:", m_count, "(", count, ")");
		return count;
	}

	void roll(uint32_t count)
	{
		LOGT(lock, "ROLL-Pre:", m_count, "(", count, ")");
		fastFwd(count);
		m_count++;
		if (!count)
			m_ownerId = Impl::getCurrentId();
		LOGT(lock, "ROLL-Post:", m_count, "(", count, ")");
	}

	SystemMutexType & getBackingLock() { return m_mutex; }

protected:
	SystemMutexType m_mutex;
};

}
