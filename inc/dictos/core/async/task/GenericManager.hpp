#pragma once

namespace dictos::async::task {

/**
 * The generic manager object provides the first layer of generic programming
 * using a templated type for the service run interface.
 */
template<class Impl>
class GenericManager : public ManagerInterface
{
public:
	// Shortcut to parent and grab our trait types
	using MutexLockImpl = typename Impl::MutexLockImplType;
	using ThreadType = GenericRunnable<typename Impl::Parent>;

	GenericManager(std::string name = "Global task manager") :
		m_name(std::move(name))
	{
		init();
	}

	~GenericManager()
	{
		deinit();
	}

	void init()
	{
		if (m_serviceThreads.empty()) {
			LOG(task, "Manager initializing with 5 threads");

			// For now jus hard core 5 threads
			for (auto i = 0; i < 5; i ++) {
				m_serviceThreads.emplace_back(std::make_shared<ThreadType>(
						string::toString("Task serviver: ", i),
						std::bind(&GenericManager::serviceLoop, this))
					);
			}

			wakeup();
		}
	}

	const std::string & __logPrefix() const noexcept override
	{
		return m_name;
	}

	void deinit() noexcept override
	{
		// Cancel all service threads
		auto guard = m_lock.lock();

		// Snap shot our thread list then unlock and cancel them all
		auto threads = std::move(m_serviceThreads);

		// Now wake  them up
		m_condition.signal();

		guard.release();

		// Cancel and block on them now
		for (auto &thread : threads)
			thread->cancel();
		for (auto &thread : threads)
			thread->stop();

		guard = m_lock.lock();

		// Free any tasks we are holding
		m_tasks.clear();
		m_taskPositions.clear();
	}

	void start(TaskPtr task) override
	{
		auto guard = m_lock.lock();

		LOGT(debug, "Starting task:", *task);

		// Place the request to start this task in the end of the active queue
		// and stash its iterator position in the active set for quick lookup
		m_tasks.push_back(task);
		auto iter = m_tasks.end();
		iter--;
		m_taskPositions[task] = std::move(iter);

		// Atomically check if the servicer is started while we have the guard
		wakeup();

		guard.release();
	}

	void join(TaskPtr task) override
	{
		auto guard = m_lock.lock();

		DCORE_ASSERT(m_taskPositions.find(task) != m_taskPositions.end());

		while (!task->isCompleted()) {
			wakeup();
			m_condition.wait(guard);
		}
	}

protected:
	void serviceLoop()
	{
		auto guard = m_lock.lock();

		LOGT(debug, "Servicer active");

		while (!async::isCancelled()) {
			guard.lock();

			// Find the first task we can aquire a service lock on
			TaskPtr task;
			for (auto &_task : m_tasks) {
				if (_task->pendingAquire()) {
					task = _task;
					break;
				}
			}

			if (!task) {
				m_condition.wait(guard);
				continue;
			}

			// Service it
			guard.unlock();
			task->invoke();
			guard.lock();

			// One more completed task
			task->setCompleted();

			// Wake up anyone who is waiting for this
			m_condition.signal();

			guard.unlock();

			// Destruct the tasks while we are unlocked
			task.reset();
		}

		LOGT(debug, "Servicer inactive");
	}

	void wakeup()
	{
		auto guard = m_lock.lock();
		m_condition.signal();

		for(auto &thread : m_serviceThreads)
			thread->start();
	}

	// We track tasks in a list so that the order they were submitted is the order
	// we process them
	std::list<TaskPtr> m_tasks;

	// We keep all the tasks we're processing be they active/pending/inactive in a set
	// for easy lookup, with an iterator to their location in the work queue
	std::map<TaskPtr, std::list<TaskPtr>::iterator> m_taskPositions;

	mutable lock::GenericLock<MutexLockImpl> m_lock;
	mutable lock::GenericCondition<MutexLockImpl> m_condition;
	mutable std::vector<std::shared_ptr<ThreadType>> m_serviceThreads;
	const std::string m_name;
};

}
