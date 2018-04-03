#pragma once

namespace dictos::async {

/**
 * The callback class allows for opaque storage and execution
 * of a templated callback with bound arguments.
 */
class Callback
{
public:
	Callback() = default;

	/**
	 * The invoker is our means of encapsulating a type specific callback
	 * with a non type specific Callback object. The invoker can be returned
	 * to the caller if the caller needs to fetch the result post invocation.
	 */
	struct InvokerBase
	{
		virtual void invoke() = 0;
	};

	template<class Func, class Result, class ...Args>
	struct Invoker;

	/**
	 * Constructs a callback from a callback and a series of args.
	 */
	template<class Func, class ...Args>
	Callback(Func  func, Args... args) :
		m_invoker(std::static_pointer_cast<InvokerBase>(allocateInvoker(std::forward<Func>(func), std::forward<Args>(args)...)))
	{
	}

	/**
	 * Constructs from a pre-allocated invoker object.
	 */
	Callback(std::shared_ptr<InvokerBase> invoker) :
		m_invoker(std::move(invoker))
	{
	}

	/**
	 * Allocates a callback invoker object.
	 */
	template<class Func, class ...Args>
	static std::shared_ptr<Invoker<Func, std::result_of_t<Func(Args...)>, Args...>> allocateInvoker(Func  func, Args...args)
	{
		return std::make_shared<Invoker<Func, std::result_of_t<Func(Args...)>, Args...>>(std::forward<Func >(func), std::forward<Args>(args)...);
	}

	Callback(Callback &&cb) :
		m_invoker(std::move(cb.m_invoker))
	{
	}

	Callback & operator = (Callback &&cb)
	{
		m_invoker = std::move(cb.m_invoker);
		return *this;
	}

	Callback &operator = (const Callback &cb) = delete;
	Callback(const Callback &cb) = delete;

	void invoke() const
	{
		DCORE_ASSERT(m_invoker);
		m_invoker->invoke();
	}

	explicit operator bool () const { return static_cast<bool>(m_invoker); }
	void reset() { m_invoker.reset(); }

protected:
	// We keep a base ptr to our invoker so that the Callback object itself does
	// not need to be templated
	std::shared_ptr<InvokerBase> m_invoker;
};

// Declare the invoker template specializations, primarily based around arg and return types.

/**
 * This template struct gets selected when the result of the callback is
 * void, it provides no return value capture.
 */
template<class Func, class ...Args>
struct Callback::Invoker<Func, std::enable_if_t<std::is_void_v<std::result_of_t<Func(Args...)>>, std::result_of_t<Func(Args...)>>, Args...> : public InvokerBase
{
	Invoker(Func func, Args...args) :
		m_args(std::make_tuple(std::forward<Args>(args)...)),
		m_func(std::forward<Func&&>(func))
	{
	}

	void invoke() override { invokeInternal(m_args); }

	// Invoke when there are args
	template<class Tuple>
	typename std::enable_if<0 != std::tuple_size<Tuple>::value, void>::type
	invokeInternal(const Tuple &tp) { return std::apply(m_func, m_args); }

	// Invoke when there are no args
	template<class Tuple>
	typename std::enable_if<0 == std::tuple_size<Tuple>::value, void>::type
	invokeInternal(const Tuple &tp) { m_func(); }

	Func m_func;
	std::tuple<Args...> m_args;
};

/**
 * This template struct gets selected when the result of the callback is
 * non-void, it captures the result.
 */
template<class Func, class ...Args>
struct Callback::Invoker<Func, std::enable_if_t<!std::is_void_v<std::result_of_t<Func(Args...)>>, std::result_of_t<Func(Args...)>>, Args...> : public InvokerBase
{
	Invoker(Func func, Args...args) :
		m_args(std::make_tuple(std::forward<Args>(args)...)),
		m_func(std::forward<Func>(func))
	{
	}

	void invoke() override { invokeInternal(m_args); }

	// Invoke when there are args
	template<class Tuple>
	typename std::enable_if<0 != std::tuple_size<Tuple>::value, void>::type
	invokeInternal(const Tuple &tp) { m_result = std::apply(m_func, m_args); }

	// Invoke when there are no args
	template<class Tuple>
	typename std::enable_if<0 == std::tuple_size<Tuple>::value, void>::type
	invokeInternal(const Tuple &tp) { m_result = m_func(); }

	Func m_func;
	std::result_of_t<Func(Args...)> m_result;
	std::tuple<Args...> m_args;
};

}
