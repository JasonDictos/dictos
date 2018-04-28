#pragma once

namespace dictos::async {

/**
 * The generic current runnable is a generic template defines how
 * a current api for thread should work.
 */
template<class Impl>
class GenericCurrentRunnable : public Impl
{
public:
	using Impl::RunnableType;
	using Impl::isCancelled;
	using Impl::cancel;
	using Impl::checkCancelled;
	using Impl::get;
	using Impl::set;
};

}
