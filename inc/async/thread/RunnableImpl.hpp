#pragma once

namespace dictos::async::thread {

/**
 * The runnable thread implements a runnable thread when plugged into
 * GenericRunnable.
 */
class RunnableImpl : public ThreadImpl
{
public:
	using Parent = ThreadImpl;
	using RunnableType = GenericRunnable<RunnableImpl>;

	// GenericImpl - requirements
	using IdType = Parent::IdType;
	using SystemRunnableType = Parent::SystemRunnableType;
	using SystemConditionType = Parent::SystemConditionType;
	using SystemMutexType = Parent::SystemMutexType;

	// GenericRunnable - requirements
	using LocalImplType = LocalImpl<GenericRunnable<RunnableImpl>, class MainThread>;
};

}
