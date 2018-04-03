#pragma once

namespace dictos::async {

/**
 * The generic impl validates api objects that geto
 * change the characteristics of our generics.
 */
template<class Impl>
class GenericImpl : public Impl
{
public:
	// Verify its properties and methods
	using IdType = typename Impl::IdType;
	using SystemRunnalbleType = typename Impl::SystemRunnableType;
	using SystemConditionType = typename Impl::SystemConditionType;
	using SystemMutexType = typename Impl::SystemMutexType;
};

}
