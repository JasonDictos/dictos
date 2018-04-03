#pragma once

namespace dictos::async::lock {

/**
 * The generic lock template describes an api in which some lockable
 * object may be used alongside a lock guard scope class.
 */
template<class Impl>
class GenericLock
{
public:
	using ImplType = GenericImpl<Impl>;
	using GuardType = GenericGuard<Impl>;
	using ImplPtr = std::shared_ptr<Impl>;

	/**
	 * This constructor links this lock object to another
	 * lock object of the same type. By doing so both locks
	 * act as if they were the same lock.
	 */
	GenericLock(GenericLock<Impl> &lock) noexcept :
		m_impl(lock.m_impl)
	{
	}

	GenericLock(ImplPtr impl) noexcept
	{
		operator = (std::move(impl));
	}

	/**
	 * Default constructor instantiates the lock impl of the
	 * templated type.
	 */
	GenericLock() :
		m_impl(std::make_shared<Impl>())
	{
	}

	// Locks can be moved
	GenericLock<Impl> & operator = (ImplPtr &&impl) noexcept
	{
		DCORE_ASSERT(isLocked() == false);
		DCORE_ASSERT(impl->getCount() == 0);
		m_impl = std::move(impl);
		return *this;
	}

	// Cannot re-assign a lockable or copy it
	GenericLock<Impl> & operator = (const GenericLock<Impl> &lock) = delete;

	// Operator bool maps to isLocked
	explicit operator bool () const noexcept { return isLocked(); }

	GuardType lock() noexcept
	{
		DCORE_ASSERT(m_impl);
		m_impl->lock();
		return GuardType(m_impl, GuardType::STATE::Locked);
	}

	template<class Rep, class Ratio>
	GuardType tryLock(time::duration<Rep, Ratio> wait) noexcept
	{
		DCORE_ASSERT(m_impl);
		if (m_impl->tryLock(wait))
			return GuardType(m_impl, GuardType::STATE::Locked);
		else
			return GuardType(m_impl, GuardType::STATE::Unlocked);
	}

	GuardType tryLock() noexcept
	{
		DCORE_ASSERT(m_impl);
		if (m_impl->tryLock())
			return GuardType(m_impl, GuardType::STATE::Locked);
		else
			return GuardType(m_impl, GuardType::STATE::Unlocked);
	}

	GuardType rewind() noexcept
	{
		DCORE_ASSERT(m_impl);
		if (isLockedByMe())
			return GuardType(m_impl, GuardType::STATE::Rewound, m_impl->rewind());

		return GuardType(m_impl);
	}

	bool isLockedByMe() const noexcept
	{
		DCORE_ASSERT(m_impl);
		return isLocked() && m_impl->getOwnerId() == Impl::getCurrentId();
	}

	bool isLocked() const noexcept
	{
		DCORE_ASSERT(m_impl);
		return m_impl->getCount() != 0;
	}

	uint32_t getLockCount() const noexcept
	{
		DCORE_ASSERT(m_impl);
		return m_impl->getCount();
	}

protected:
	// Our impl is what maintains state, and is the very essence
	// of the lock
	ImplPtr m_impl;
};

}
