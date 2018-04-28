#pragma once

namespace dictos::async::thread {

/**
 * The main thread is the runnable that is
 * returned when the current thread is the main thread.
 */
class MainThread : public GenericRunnable<RunnableImpl>
{
public:
	using Parent = GenericRunnable<RunnableImpl>;
	using GenericRunnableType = GenericRunnable<RunnableImpl>;

	MainThread()
	{
		Parent::m_id = ThreadImpl::getCurrentId();
		Parent::m_name = "Main";
	}

	void start() override {}
	void stop() noexcept override {}
	bool isStarted() const noexcept override { return true; }
	void join(bool _throw = true) override { }
};

}
