#pragma once

namespace dictos::async::thread {

/**
 * The thread local class manages a templated static behind a type
 * to generically provide slotted thread local storage.
 * Type - This is the type to store in thread local, each type gets
 * its own slot.
 * DefaultType - The default type for when there isn't a local slot
 * set.
 */
template<class Type, class DefaultType>
class LocalImpl
{
public:
	// GenericLocal - requirements
	static Type *get() noexcept
	{
		auto type = *current();
		if (type)
			return type;

		static DefaultType def;
		DCORE_ASSERT(getCurrentId() == def.getId());
		return &def;
	}

	static void set(Type *type) noexcept
	{
		*current() = type;
	}

protected:
	static Type **current() noexcept
	{
		static thread_local Type *type = nullptr;
		return &type;
	}
};

}
