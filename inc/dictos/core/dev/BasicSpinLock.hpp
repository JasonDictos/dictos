#pragma once

namespace dictos::dev {

/**
 * This basic spinlock has zero dependencies to the rest of the system and
 * is suitable for very low level uses in places where recursion is a problem
 * or where some fundamentally low level locking situation is warranted.
 * It supports no recursion, and is based off of std::thread for the api type.
 */
class BasicSpinLock
{
public:
	class Guard
	{
	public:
		Guard(BasicSpinLock *lock = nullptr) :
			m_lock(lock)
		{
		}

		~Guard()
		{
			if (m_lock)
			{
				m_lock->unlock();
				m_lock = nullptr;
			}
		}

		Guard(const Guard &guard) = delete;
		Guard & operator = (const Guard &guard) = delete;

		Guard(Guard &&guard)
		{
			operator = (std::move(guard));
		}

		Guard & operator = (Guard &&guard)
		{
			if (m_lock)
				m_lock->unlock();

			m_lock = guard.m_lock;
			guard.m_lock = nullptr;
			return *this;
		}

		explicit operator bool () const { return m_lock ? true : false; }

	protected:
		BasicSpinLock *m_lock = nullptr;
	};

	Guard tryLock()
	{
		if (m_flag.test_and_set(std::memory_order_acquire))
			return Guard();
		if (m_ownerId != std::this_thread::get_id())
			m_ownerId = std::this_thread::get_id();
		return Guard(this);
	}

	/**
	 * The recurse lock acts like a failed try lock if the current thread
	 * already has the lock, the caller should check the boolean result
	 * of the Guard and if false it means recursion is afoot.
	 */
	Guard checkRecursionLock()
	{
		auto guard = tryLock();
		if (guard)
			return guard;
		if (m_ownerId == std::this_thread::get_id())
			return Guard();
		return lock();
	}

	Guard lock()
	{
		while (!tryLock()) {}
		return Guard(this);
	}

	bool isLockedByMe() const
	{
		return m_ownerId == std::this_thread::get_id();
	}

protected:
	void unlock()
	{
		// Thanks stl for giving us no way to change this variable other then a memset
		memset(&m_ownerId, 0, sizeof(m_ownerId));

		m_flag.clear();
	}

	std::atomic_flag m_flag = { false };
	std::thread::id m_ownerId;
};

}
