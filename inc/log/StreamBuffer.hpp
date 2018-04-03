#pragma once

namespace dictos::log {

/**
 * The stream buffer can be used inside external ostreams to allow for
 * log redirection to our logging framework.
 */
class StreamBuffer : public std::stringbuf
{
public:
	StreamBuffer(std::string_view prefix) :
		m_prefix(prefix.begin(), prefix.end())
	{ 
	}

	virtual int sync() 
	{ 
		auto _str = str();
		writePrefix("test", m_prefix, _str);
		str("");
		return int (_str.size());
	}

protected:
	std::string m_prefix;
};

}
