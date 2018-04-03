#pragma once

namespace dictos::async::task::thread {

/**
 * The task class defines basic properties for specializing GenericTask
 * to work with threads.
 */
class TaskImpl : public async::thread::RunnableImpl
{
public:
	using Parent = RunnableImpl;

	// GenericImpl - requirements
	using IdType = Parent::IdType;
	using SystemRunnableType = Parent::SystemRunnableType;
	using SystemConditionType = Parent::SystemConditionType;
	using SystemMutexType = Parent::SystemMutexType;

	// GenericTask - requirements
	using MutexLockImplType = lock::MutexLockImpl;
	using SpinLockImplType = lock::SpinLockImpl;

	static std::shared_ptr<ManagerInterface> getManager()
	{
		return task::thread::getManager();
	}
};

}
