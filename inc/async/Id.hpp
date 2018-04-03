#pragma once

namespace dictos::async {

/**
 * The id class combines ids from both the fiber and thread world into unified id.
 */
class Id
{
public:
	enum class TYPE
	{
		ThreadId,
		FiberId
	};

	Id() = default;

	Id(thread::thread_id threadId) :
		m_threadId(threadId),
		m_type(TYPE::ThreadId)
	{
	}

	Id(const Id &id) :
		m_type(id.m_type),
		m_threadId(id.m_threadId)
	{
	}

	Id & operator = (const Id &id)
	{
		if (this == &id)
			return *this;

		m_type = id.m_type;
		m_threadId = id.m_threadId;
		return *this;
	}

	Id & operator = (const thread::thread_id id)
	{
		m_type = TYPE::ThreadId;
		m_threadId = id;
		return *this;
	}

	bool operator < (const Id &id) const
	{
		if (m_type != id.m_type)
			return false;

		if (m_type == TYPE::ThreadId)
			return m_threadId < id.m_threadId;
		return false;
	}

	bool operator == (const thread::thread_id &id) const
	{
		if (m_type == TYPE::ThreadId)
			return m_threadId == id;
		return false;
	}

	bool operator == (const Id &id) const
	{
		if (m_type != id.m_type)
			return false;

		if (m_type == TYPE::ThreadId)
			return m_threadId == id.m_threadId;
		return false;
	}

	friend std::ostream & operator << (std::ostream &stream, const Id &id) noexcept
	{
		if (id.m_type == TYPE::ThreadId)
			return stream << id.m_threadId;
		return stream;
	}

protected:
	thread::thread_id m_threadId;
	TYPE m_type;
};

}
