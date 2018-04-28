#pragma once

namespace dictos::log {

/**
 * This singleton class manages global log prefixes.
 */
class Prefix
{
public:
	Prefix() = default;

	static Prefix * instance()
	{
		static thread_local Prefix prefix;
		return &prefix;
	}

	static auto push(const std::string_view &prefix)
	{
		return instance()->_push(prefix);
	}

	static auto pop()
	{
		return instance()->_pop();
	}

	static auto get()
	{
		return instance()->_get();
	}

	template<class PrefixInput>
	static auto get(const PrefixInput &input)
	{
		return instance()->_get<PrefixInput>(input);
	}

protected:
	void _push(const std::string_view &prefix)
	{
		m_stack.push(std::string(prefix.begin(), prefix.end()));
		m_last.clear();
	}

	void _pop()
	{
		m_stack.pop();
		m_last.clear();
	}

	const std::string & _get() const
	{
		m_lastRaw = nullptr;
		if (m_last.empty())
		{
			if (m_stack.empty())
				return m_last = "";

			return m_last = "[" + m_stack.top() + "]: ";
		}

		return m_last;
	}

	template<class PrefixInput>
	const std::string & _get(const PrefixInput &input) const
	{
		if (m_last.empty() == false && m_lastRaw == &input)
			return m_last;

		m_lastRaw = const_cast<PrefixInput *>(&input);

		if (m_stack.empty())
			return m_last = "[" + getPrefix(input) + "]: ";

		return m_last = "[" + m_stack.top() + "-" + getPrefix(input) + "]: ";
	}

	// No locking here, this prefix is per thread
	std::stack<std::string> m_stack;
	mutable std::string m_last;
	mutable void *m_lastRaw = nullptr;
};

}
