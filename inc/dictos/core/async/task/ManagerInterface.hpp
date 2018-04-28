#pragma once

namespace dictos::async::task {

/**
 * The manager interface provides the core non-templated apis required to implement
 * a task manager.
 */
class ManagerInterface : public std::enable_shared_from_this<ManagerInterface>
{
public:
	virtual void start(TaskPtr task) = 0;
	virtual void join(TaskPtr task) = 0;
	virtual void deinit() noexcept = 0;
	virtual void init() = 0;
	virtual const std::string & __logPrefix() const noexcept = 0;
};

}
