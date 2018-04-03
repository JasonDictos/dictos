#pragma once

namespace dictos::async::task {

/**
 * The task interface just adds an invoke method but is otherwise a run interface.
 */
class TaskInterface : public RunnableInterface
{
public:
	virtual void invoke() noexcept = 0;
	virtual void setInvoker(std::shared_ptr<Callback::InvokerBase> invoker) noexcept = 0;

	TaskPtr getTaskPtr() const
	{
		return std::static_pointer_cast<TaskInterface>(const_cast<TaskInterface *>(this)->shared_from_this());
	}
};

}
