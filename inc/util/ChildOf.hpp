#pragma once

namespace dictos::util {

/**
 * Child of is a simple class that provides a basic relationship
 * between the child and its 'parent'.
 */
template<class Parent>
class ChildOf
{
public:
	ChildOf(Parent *parent) :
		m_parent(parent)
	{
	}

	Parent *parent() noexcept { return m_parent; }
	const Parent *parent() const noexcept { return m_parent; }

private:
	Parent *m_parent;
};

}
