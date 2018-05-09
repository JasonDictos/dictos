#pragma once

namespace {

	static std::atomic<int> s_nextGenerationId = { 0 };

}

namespace dictos::async::lock {

/**
 * The generic lock implementation just manages the owner id and counting
 * portions of mutex/spin locks.
 */
template<class Impl>
class GenericLockImpl : public GenericImpl<Impl>
{
public:
	using Parent = GenericImpl<Impl>;
	using IdType = typename Parent::IdType;

	uint32_t getCount() const noexcept { return m_count.load(); }
	IdType getOwnerId() const noexcept { return m_ownerId; }

	GenericLockImpl() :
			m_generationId(s_nextGenerationId++)
	{
	}

	std::string __toString() const
	{
		return string::toString(
			"LockId:", string::toString(m_generationId),
			"OwnerId:", m_ownerId,
			"ThreadId:", getCurrentId()
		);
	}

	virtual void unlock() noexcept = 0;
	virtual void lock() noexcept = 0;

	uint32_t rewind() noexcept
	{
		auto unlockTotal = getCount();

		auto currentId = Parent::getCurrentId();
		auto ownerId = getOwnerId();

		for (uint32_t unlockCount = 0; unlockCount < unlockTotal; unlockCount++)
			unlock();
		return unlockTotal;
	}

	void fastFwd(uint32_t count) noexcept
	{
		for (uint32_t lockCount = 0; lockCount < count; lockCount++)
			lock();

		auto currentId = Parent::getCurrentId();
		auto ownerId = getOwnerId();
	}

protected:
	bool inc(bool tryResult = true) noexcept
	{
		auto currentId = Parent::getCurrentId();
		auto ownerId = getOwnerId();

		if (tryResult) {
			if (m_count++ == 0) {
				m_ownerId = Parent::getCurrentId();
			}
			DCORE_ASSERT(m_ownerId == currentId);
		}

		return tryResult;
	}

	void dec() noexcept
	{
		auto currentId = Parent::getCurrentId();
		auto ownerId = getOwnerId();

		DCORE_ASSERT(getOwnerId() == Parent::getCurrentId());
		DCORE_ASSERT(m_count != 0);

		m_count--;
	}

	IdType m_ownerId;
	std::atomic<uint32_t> m_count = {0};
	uint32_t m_generationId;
};

}
