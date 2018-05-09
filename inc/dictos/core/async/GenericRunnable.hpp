#pragma once

namespace dictos::async {

/**
 * The runnable object is a generic runnable object that can be started
 * stopped joined and cancelled. The actual os specific, or implementation
 * specific details are granted to this by the template type.
 */
template<class Impl>
class GenericRunnable :
	public RunnableInterface
{
public:
	// Pull our attributes from our traits
	using Parent = GenericImpl<Impl>;
	using IdType = typename Parent::IdType;
	using SystemRunnableType = typename Parent::SystemRunnableType;
	using LocalImplType = typename Parent::LocalImplType;

	explicit GenericRunnable() = default;

	// No copying/moving a runnable
	GenericRunnable(const GenericRunnable &runnable) = delete;
	GenericRunnable & operator = (const GenericRunnable &runnable) = delete;
	GenericRunnable(GenericRunnable &&runnable) = delete;
	GenericRunnable &operator = (GenericRunnable &&runnable) = delete;

	explicit GenericRunnable(std::string name) noexcept :
		m_name(std::move(name))
	{
	}

	const std::string &__logPrefix() const noexcept override
	{
		return m_name;
	}

	template<class Function, class ...Args>
	GenericRunnable(std::string name, Function func, Args&&... args) noexcept :
		m_callback(std::forward<Function>(func), std::forward<Args&&>(args)...),
		m_name(std::move(name))
	{
	}

	~GenericRunnable() noexcept { join(false); }

	bool isStarted() const noexcept override { return m_started == true; }
	bool isStopped() const noexcept override { return m_started == false; }

	void start() override
	{
		auto guard = m_lock.lock();

		DCORE_ASSERT(m_callback);
		if (isStarted())
			return;

		m_cancelled = false;
		m_started = true;
		m_exception = 0;
		m_runnable = std::make_unique<SystemRunnableType>(std::bind(&GenericRunnable<Impl>::run, this));
		m_id = m_runnable->get_id();
	}

	void stop() noexcept override
	{
		auto guard = m_lock.lock();

		if (!m_started)
			return;

		LOGT(debug, "Stopping runnable", m_id);

		cancel();

		join(false);

		LOGT(debug, "Stopped runnable", m_id);
	}

	void cancel() noexcept override
	{
		auto guard = m_lock.lock();

		if (isStarted())
		{
			LOGT(debug, "Runnable cancelled");
			m_cancelled = true;
		}
	}

	void join(bool _throw = true) override
	{
		// Grab the lock for the duration of the join
		auto guard = m_lock.lock();

		if (!m_runnable)
			return;

		if (m_runnable->joinable())
			m_runnable->join();

		DCORE_ASSERT(m_started == false);

		m_runnable.reset();

		if (m_exception && _throw)
			std::rethrow_exception(m_exception);
	}

	template<class UserDataType>
	void associate(async::Id id, UserDataType *data)
	{
		auto guard = m_lock.lock();
		m_workspace[id] = data;
	}

	void disassociate(async::Id id)
	{
		auto guard = m_lock.lock();
		m_workspace.erase(id);
	}

	auto getAssociatedCount() const
	{
		auto guard = m_lock.lock();
		return m_workspace.size();
	}

	std::string_view getName() const noexcept override { return m_name; }
	bool isCancelled() const noexcept override { return m_cancelled; }

	void checkCancelled() const override
	{
		if (m_cancelled)
			DCORE_THROW(Cancelled, "Thread canncelled:", *this);
	}

	IdType getId() const { return m_id; }

protected:
	void run() noexcept
	{
		LOGT(debug, "Runnable run starting");

		DCORE_ASSERT(m_started == true);

		// Call pre/post for runnable setup
		util::Scope prePost(
			std::bind(&GenericRunnable<Impl>::preRun, this),
			std::bind(&GenericRunnable<Impl>::postRun, this)
		);

		// Invoke the callback and stash its exception if one is set
		m_exception = error::block(m_callback);

		if (m_exception)
			LOGT(debug, "Runnable run completed (exception)");
		else
			LOGT(debug, "Runnable run completed");

		prePost.exec();

		DCORE_ASSERT(m_started == false);
	}

	/**
	 * Sets the current runnable as the thread local
	 */
	void preRun() noexcept
	{
		LocalImplType::set(this);
	}

	/**
	 * Clears the runnable in the current impl.
	 */
	void postRun() noexcept
	{
		LocalImplType::set(nullptr);

		// Clear the callback held args
		m_callback.reset();

		m_started = false;
	}

	Callback m_callback;

	// We allow the api type to define the primary lock for our state
	mutable lock::SpinLock m_lock;

	// Our id set when we start, defined by the api
	IdType m_id;

	// This is the actual run implementation (e.g. std::thread)
	std::unique_ptr<SystemRunnableType> m_runnable;

	// A generic id collection of thread specific items
	std::map<async::Id, void *> m_workspace;

	std::string m_name = "Un-named runnable";
	std::atomic<bool> m_cancelled = false, m_started = false;
	std::exception_ptr m_exception;
};

}
