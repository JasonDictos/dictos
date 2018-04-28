#pragma once

namespace dictos::async::task {

/**
 * The result task just exists to facilitate certain use cases whereby
 * the caller wants a result from a directly instantiated task with
 * its callback signature.
 */
template<class Impl, class Func, class ...Args>
class GenericResultTask : public GenericTask<Impl>
{
public:
	GenericResultTask(std::string name, Func func, Args...args) :
		m_invoker(Callback::allocateInvoker(std::forward<Func>(func), std::forward<Args>(args)...)),
		GenericTask<Impl>(std::move(name))
	{
		GenericTask<Impl>::setInvoker(m_invoker);
	}

	std::result_of_t<Func(Args...)> getResult()
	{
		if (m_resultGotten)
			DCORE_THROW(RuntimeError, "You cannot call getResult twice on a task");

		GenericTask<Impl>::join();
		m_resultGotten = true;

		return std::move(m_invoker->m_result);
	}

protected:
	std::shared_ptr<Callback::Invoker<Func, std::result_of_t<Func(Args...)>, Args...>> m_invoker;
	std::atomic<bool> m_resultGotten = { false };
};

}
