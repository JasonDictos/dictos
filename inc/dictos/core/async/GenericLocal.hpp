#pragma once

namespace dictos::async {

/**
 * The generic local class provides opaque generic 'local' semantics
 * to a runnable implementation.
 */
template<class Impl>
class GenericLocal : public Impl
{
public:
	using Impl::set;
	using Impl::get;
};

}
