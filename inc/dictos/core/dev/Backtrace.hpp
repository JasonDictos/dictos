#pragma once

namespace dictos::dev {

/**
 * The backtrace class generates backtraces and is itself a
 * representation of a backtrace.
 */
class Backtrace
{
public:
	Backtrace() = default;

	std::string __toString() const
	{
		if (m_stringRender.empty())
		{
			std::stringstream stream;
			stream << m_trace;
			m_stringRender = stream.str();
		}
		return m_stringRender;
	}

	friend std::ostream & operator << (std::ostream &stream, const Backtrace &trace)
	{
		return stream << trace.__toString();
	}

protected:
	boost::stacktrace::stacktrace m_trace;
	mutable std::string m_stringRender;
};

}
