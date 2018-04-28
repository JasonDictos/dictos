#pragma once

namespace dictos::config {

/**
 * A section is a collection of config options to config option defaults.
 */
class Section
{
public:
	friend class Context;

	Section(std::string name, std::initializer_list<Option> options) :
		m_name(std::move(name)), m_options(options)
	{
		// Register ourselves with the global sections singleton
		Sections::add(this);
	}

	const std::string & getName() const noexcept { return m_name; }

	Option getOptionObj(const std::string_view &key) const
	{
		auto guard = m_lock.lock();
		auto iter = m_options.find(key);
		if (iter == m_options.end())
			DCORE_THROW(NotFound, "Option by name:", key, "not found");
		return *iter;
	}

	std::string_view __toString() const
	{
		return m_name;
	}

protected:
	template<class Type>
	inline Type getVerify(
		const std::string_view &name,
		const std::set<Option> &context
	) const
	{
		auto guard = m_lock.lock();

		// Lookup the option in our section definition
		auto iter = m_options.find(name);
		if (iter == m_options.end())
			DCORE_THROW(NotFound, "Invalid option:", name);

		// Look it up on the context first
		auto citer = context.find(name);
		if (citer == context.end())
		{
			// Return the option default
			return iter->getDefault<Type>();
		}

		// Use the context version
		return iter->process(citer->getDefault<Type>());
	}

	template<class Type>
	inline Type getVerify(
		const std::string_view &name,
		const std::set<Option> &context,
		const Type &def
	) const
	{
		auto guard = m_lock.lock();

		// Lookup the option in our section definition
		auto iter = m_options.find(name);
		if (iter == m_options.end())
			DCORE_THROW(NotFound, "Invalid option:", name);

		// Look it up on the context first
		auto citer = context.find(name);
		if (citer == context.end())
		{
			// Return the option default
			return iter->getDefault<Type>();
		}

		// Let the option process it
		return iter->process(def);
	}

	mutable async::lock::SpinLock m_lock;
	const std::set<Option> m_options;
	const std::string m_name;
};

}
