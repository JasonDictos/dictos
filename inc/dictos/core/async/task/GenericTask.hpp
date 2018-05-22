#pragma once

namespace dictos::async::task {

/**
 * The generic task defines the basics for a task,
 * just like GenericRunnable and friends.
 */
template<class Impl>
class GenericTask : public TaskInterface
{
public:
	// Shortcut to parent and grab our trait types
	using MutexLockImplType = typename Impl::MutexLockImplType;
	using SpinLockImplType = typename Impl::SpinLockImplType;

	GenericTask(std::shared_ptr<ManagerInterface> manager, std::string name) :
		m_manager(manager), m_name(std::move(name))
	{
	}

	GenericTask(std::string name) :
		m_manager(Impl::getManager()), m_name(std::move(name))
	{
	}

	~GenericTask()
	{
		join(false);
	}

	const std::string & __logPrefix() const noexcept override
	{
		return m_name;
	}

	bool isStarted() const noexcept override
	{
		return m_started;
	}

	bool isStopped() const noexcept override
	{
		return !m_started;
	}

	void start() override
	{
		auto guard = m_lock.lock();

		if (m_started)
			return;

		DCORE_ASSERT(m_callback);

		m_started = true;
		m_manager->start(getTaskPtr());
	}

	void stop() noexcept override
	{
		cancel();
		join(false);
	}

	void join(bool _throw = true) override
	{
		auto guard = m_lock.lock();

		if (!m_started)
		{
			if (m_exception && _throw)
				std::rethrow_exception(m_exception);
			return;
		}

		try
		{
			m_manager->join(getTaskPtr());
		}
		catch (...)
		{
			m_exception = std::current_exception();
		}

		m_started = false;

		if (m_exception && _throw)
			std::rethrow_exception(m_exception);
	}

	void cancel() noexcept override
	{
		auto guard = m_lock.lock();
		m_cancelled = true;
	}

	bool isCancelled() const noexcept override
	{
		return m_cancelled;
	}

	void checkCancelled() const override
	{
		if (m_cancelled)
			DCORE_THROW(Cancelled, "Task was cancelled");
	}

	void invoke() noexcept override
	{
		DCORE_ASSERT(m_started);

		try
		{
			if (m_cancelled)
				DCORE_THROW(Cancelled, "Task cancelled");

			m_callback.invoke();
		}
		catch (...)
		{
			m_exception = std::current_exception();
		}
	}

	std::string_view getName() const noexcept override
	{
		return m_name;
	}

	/**
	 * This is called by the task managers, the first one to grab the flag
	 * gets to service the task. This returns true on the first task manager
	 * to call it.
	 */
	bool pendingAquire() noexcept override 
	{
		auto aquired = !m_pendingFlag.test_and_set(std::memory_order_acquire);
		if (aquired) {
			DCORE_ASSERT(m_completed == false);
		}
		return aquired;
	}

	/**
	 * Called by the task managers to report a completed task service. The
	 * pending flag is left locked.
	 */
	void setCompleted() noexcept override 
	{
		DCORE_ASSERT(m_completed == false);
		m_completed = true;
	}

	bool isCompleted() const noexcept override { return m_completed; }

protected:

	void setInvoker(std::shared_ptr<Callback::InvokerBase> invoker) noexcept override
	{
		auto guard = m_lock.lock();
		m_callback = Callback(std::move(invoker));
	}

	Callback m_callback;
	mutable lock::GenericLock<SpinLockImplType> m_lock;
	std::exception_ptr m_exception;
	std::shared_ptr<ManagerInterface> m_manager;

	std::atomic<bool> m_cancelled = {false}, m_started = {false}, m_completed = {false};
	std::atomic_flag m_pendingFlag = {false};

	const std::string m_name;
};

}
