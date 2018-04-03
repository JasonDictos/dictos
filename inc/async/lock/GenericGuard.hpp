#pragma once

namespace dictos::async::lock {

/**
 * The lock guard template class is a generic 'scope' lock object
 * which holds the lock for the duration of its existence.
 * It uses the impl trait rather then the lock trait as it primarily
 * interacts with the generic mutex through its impl.
 */
template<class Impl>
class GenericGuard
{
public:
	using ImplPtr = std::shared_ptr<Impl>;

	// Define our states which allow us to assert well known values of our
	// properties at each one
	enum class STATE
	{
		Init,
		Locked,
		Unlocked,
		Rewound
	};

	GenericGuard(const GenericGuard<Impl> &guard) = delete;
	GenericGuard<Impl> & operator = (const GenericGuard<Impl> &guard) = delete;

	GenericGuard(ImplPtr impl, STATE state = STATE::Init, uint32_t rewoundCount = 0) noexcept :
		m_impl(impl), m_state(state), m_rewoundCount(rewoundCount)
	{
		if (m_rewoundCount)
			DCORE_ASSERT(m_state == STATE::Rewound);
		else
			DCORE_ASSERT(m_state != STATE::Rewound);
	}

	GenericGuard(GenericGuard<Impl> &&guard)
	{
		operator = (std::move(guard));
	}

	GenericGuard<Impl> & operator = (GenericGuard<Impl> &&guard)
	{
		release();
		m_impl = std::move(guard.m_impl);
		m_state = guard.m_state;
		m_rewoundCount = guard.m_rewoundCount;

		guard.m_state = STATE::Init;
		guard.m_rewoundCount = 0;
		return *this;
	}

	~GenericGuard() noexcept
	{
		release();
	}

	void release() noexcept
	{
		switch (m_state)
		{
			case STATE::Rewound:
				fastFwd();
				break;

			case STATE::Locked:
				unlock();
				break;

			case STATE::Unlocked:
			case STATE::Init:
				// Nothing to do
				break;
		}
	}

	void rewind() noexcept
	{
		DCORE_ASSERT(m_impl);
		DCORE_ASSERT(m_state == STATE::Locked);
		m_state = STATE::Rewound;
		m_rewoundCount = m_impl->rewind();
	}

	void fastFwd() noexcept
	{
		DCORE_ASSERT(m_impl);
		DCORE_ASSERT(m_state == STATE::Rewound);
		DCORE_ASSERT(m_rewoundCount);
		m_impl->fastFwd(m_rewoundCount);
		m_state = STATE::Locked;
		m_rewoundCount = 0;
	}

	void yield() noexcept
	{
		rewind();
		m_impl->yield();
		fastFwd();
	}

	template<class Rep, class Int>
	void dispatch(time::duration<Rep, Int> wait) noexcept
	{
		rewind();
		m_impl->sleep(wait);
		fastFwd();
	}

	explicit operator bool () const noexcept { return m_state == STATE::Locked; }
	const ImplPtr &getImpl() const noexcept { return m_impl; }

	void unlock() noexcept
	{
		DCORE_ASSERT(m_impl);
		DCORE_ASSERT(m_state == STATE::Locked);
		m_state = STATE::Unlocked;
		m_impl->unlock();
	}

	void lock() noexcept
	{
		DCORE_ASSERT(m_impl);
		if (m_state == STATE::Locked)
			return;
		DCORE_ASSERT(m_state == STATE::Unlocked);
		m_impl->lock();
		m_state = STATE::Locked;
	}

protected:
	// We match the impl type of the mutex's which give us birth, and we
	// keep a strong ref back to their impl for remote commands, and for
	// ensuring destruction order doesn't cause a crash
	ImplPtr m_impl;

	// The state defines certain guarantees for asserts
	STATE m_state = STATE::Init;

	// When we get rewound we stash the number of unrolls that occurred to
	// replay them back during a fastFwd
	uint32_t m_rewoundCount = 0;
};

}
