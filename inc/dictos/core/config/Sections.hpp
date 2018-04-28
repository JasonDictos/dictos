#pragma once

namespace dictos::config {

inline Sections &Sections::instance()
{
	static Sections sections;
	return sections;
}

inline void Sections::add(class Section *section)
{
	instance().registerInternal(section);
}

inline void Sections::remove(class Section *section) noexcept
{
	instance().deregisterInternal(section);
}

inline const class Section *Sections::find(const std::string_view &name) noexcept
{
	return instance().findInternal(name);
}

inline void Sections::registerInternal(Section *section)
{
	auto guard = m_lock.lock();
	auto iter = m_sections.find(section->getName());
	if (iter != m_sections.end() && iter->second != section)
		return;
	m_sections[section->getName()] = section;
}

inline void Sections::deregisterInternal(Section *section) noexcept
{
	auto guard = m_lock.lock();
	m_sections.erase(section->getName());
}

inline const Section *Sections::findInternal(const std::string_view &name) const noexcept
{
	auto guard = m_lock.lock();
	auto iter = m_sections.find(name);
	if (iter == m_sections.end())
		return nullptr;
	return iter->second;
}

}
