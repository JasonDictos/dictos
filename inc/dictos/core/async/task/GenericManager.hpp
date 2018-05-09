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
		m_activeQueue.clear();
		m_active.clear();
		m_inactive.clear();
	}

	void start(TaskPtr task) override
	{
		auto guard = m_lock.lock();

		LOGT(debug, "Starting task:", *task);

		// Place the request to start this task in the end of the active queue
		// and stash its iterator position in the active set for quick lookup
		m_activeQueue.push_back(task);
		auto iter = m_activeQueue.end();
		iter--;
		m_active[task] = std::move(iter);

		// Atomically check if the servicer is started while we have the guard
		wakeup();

		guard.release();
	}

	void join(TaskPtr task) override
	{
		auto guard = m_lock.lock();

		DCORE_ASSERT(m_active.find(task) != m_active.end() ||
			m_inactive.find(task.get()) != m_inactive.end());

		while (m_active.find(task) != m_active.end()) {
			wakeup();
			m_condition.wait(guard);
		}

		DCORE_ASSERT(m_inactive.find(task.get()) != m_inactive.end());
		m_inactive.erase(task.get());
	}

protected:
	void serviceLoop()
	{
		auto guard = m_lock.lock();

		LOGT(debug, "Servicer active");

		while (!async::isCancelled()) {
			guard.lock();

			// Make a copy of all the items we need to service
			auto toService = m_activeQueue;

			if (toService.empty()) {
				m_condition.wait(guard);
				continue;
			}

			guard.unlock();

			// And service them
			for (auto &service : toService) {
				service->invoke();

				// Completed this active task, place it on inactive
				guard.lock();

				m_activeQueue.erase(m_active[service]);
				m_active.erase(service);

				DCORE_ASSERT(m_inactive.find(service.get()) == m_inactive.end());

				m_inactive.insert(service.get());

				m_condition.signal();

				guard.unlock();
			}

			// Destruct the tasks while we are unlocked
			toService.clear();
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

	// We keep tabs on tasks and their position in their priority queue
	std::map<TaskPtr, std::list<TaskPtr>::iterator> m_active;
	std::list<TaskPtr> m_activeQueue;

	// Once a task is serviced it gets placed in the inactive queue, when someone
	// finally joins on the task it gets removed from here
	std::set<TaskRPtr> m_inactive;

	mutable lock::GenericLock<MutexLockImpl> m_lock;
	mutable lock::GenericCondition<MutexLockImpl> m_condition;
	mutable std::vector<std::shared_ptr<ThreadType>> m_serviceThreads;
	const std::string m_name;
};

}
