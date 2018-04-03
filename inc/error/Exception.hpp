#pragma once

namespace dictos::error {

/**
 * The exception object is our base exception type, which derives from the
 * standard exception for compatibility.
 */
class Exception : public virtual std::exception
{
public:
	template<class ...Args>
	Exception(const char *file, uint32_t line, const Args& ...args) noexcept :
		m_message(string::toString(std::forward<const Args&>(args)...)),
		m_file(file), m_line(line)
	{
	}

	void selfThrow()
	{
		LOG(exception, "Raising exception:", *this);
		throw *this;
	}

	std::string_view __toString() const noexcept
	{
		if (m_cachedStringInfo.empty())
		{
			m_cachedStringInfo = string::toStringDelimiter(' ',
				log::getTypeName(*this), m_message,
				m_file, string::enclose<'(', ')'>(m_line));
		}
		return m_cachedStringInfo;
	}

	dev::Backtrace trace() const { return m_trace; }
	std::string traceString() const { return string::toString(m_trace); }

	const char * what() const noexcept override { return &__toString().front(); }

	std::string_view getFile() const noexcept { return m_file; }
	uint32_t getLine() const noexcept { return m_line; }

protected:
	mutable std::string m_cachedStringInfo;
	std::string m_file;
	uint32_t m_line;
	const std::string m_message;
	dev::Backtrace m_trace;
};

}
