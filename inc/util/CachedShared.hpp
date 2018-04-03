#pragma once

namespace dictos::util {

/**
 * The cache shared class provides a lazy weak ref to an
 * object, allowing repeated use of the original instantiation
 * so long as at least one object has a reference to it.
 */
template<class Type>
class CachedShared
{
public:
	CachedShared() = default;

	std::shared_ptr<Type> get()
	{
		auto guard = m_lock.lock();

		// Continue to re-use the shared ptr if we locked it
		auto type = m_weak.lock();
		if (type)
			return type;

		// Allocate a new one and weak ref it
		auto typeShared = std::make_shared<Type>();
		m_weak = typeShared;
		return typeShared;
	}

protected:
	async::SpinLock m_lock;
	std::weak_ptr<Type> m_weak;
};

}
