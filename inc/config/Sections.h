#pragma once

namespace dictos::config {

class Section;

/**
 * The sections class manages the storing and referencing of section objects.
 */
class Sections
{
protected:
	Sections() = default;

public:
	static Sections &instance();
	static void add(Section *section);
	static void remove(Section *section) noexcept;
	static const Section *find(const std::string_view &name) noexcept;

protected:
	void registerInternal(Section *section);
	void deregisterInternal(Section *section) noexcept;
	const Section *findInternal(const std::string_view &name) const noexcept;

	std::map<std::string_view, Section *> m_sections;
	mutable async::lock::SpinLock m_lock;
};

}
