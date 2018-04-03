#pragma once

namespace dictos::util {

/**
 * This incredibly simple but useful class just does stuff
 * on destruction, usful for quick RAII style use cases.
 */
class Scope
{
public:
	Scope(std::function<void()> post) noexcept :
		m_post(std::move(post))
	{
	}

	Scope(std::function<void()> pre, std::function<void()> post) :
		Scope(std::move(post))
	{
		pre();
	}

	~Scope() noexcept
	{
		exec();
	}

	void exec() noexcept
	{
		if (m_post)
		{
			error::block(m_post);
			cancel();
		}
	}

	void cancel() noexcept
	{
		m_post = nullptr;
	}

protected:
	std::function<void()> m_post;
};

}
