#pragma once

namespace dictos::async {

/**
 * The runnable interface declares apis that anything you can run (e.g. GenericRunnable) must
 * implement.
 */
class RunnableInterface : public std::enable_shared_from_this<RunnableInterface>
{
public:
	virtual bool isStarted() const noexcept = 0;
	virtual bool isStopped() const noexcept = 0;
	virtual void start() = 0;
	virtual void stop() noexcept = 0;
	virtual void join(bool _throw = true) = 0;
	virtual void cancel() noexcept = 0;
	virtual bool isCancelled() const noexcept = 0;
	virtual void checkCancelled() const = 0;

	virtual const std::string & __logPrefix() const noexcept = 0;
	virtual std::string_view getName() const noexcept = 0;
};

}
