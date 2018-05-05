#pragma once

namespace dictos::config {

inline Context::Context(std::string section, Options options) :
	m_section(std::move(section)), m_options(std::move(options))
{
}

inline Context::Context(const Section &section, Options options) :
	m_section(section.__toString()), m_options(std::move(options))
{
	// The section will have already registered itself but we can
	// stash it with this constructor early
	m_cachedSection = &section;
}

inline Context::Context(const Context &context)
{
	copy(context);
}

inline Context::Context(Context &&context)
{
	move(std::move(context));
}

inline Context & Context::operator = (const Context &context)
{
	if (this == &context)
		return *this;
	copy(context);
	return *this;
}

inline Context & Context::operator = (Context &&context)
{
	move(std::move(context));
	return *this;
}

inline Options Context::getOptions() const
{
	return m_options;
}

template<class Type>
inline Type Context::getOption(const std::string &key, const Type &def) const
{
	auto guard = m_lock.lock();
	return lookupSection()->getVerify<Type>(
		key,
		m_options,
		def
	);
}

template<class Type>
inline Type Context::getOption(const std::string &key) const
{
	auto guard = m_lock.lock();
	return lookupSection()->getVerify<Type>(
		key,
		m_options
	);
}

inline void Context::setOption(std::string key, std::any value)
{
	auto guard = m_lock.lock();
	Option opt(std::move(key), std::move(value));
	m_options.erase(opt);
	m_options.insert(std::move(opt));
}

inline void Context::setOptions(Options options)
{
	auto guard = m_lock.lock();
	m_options = options;
}

inline void Context::mergeOptions(Options options)
{
	auto guard = m_lock.lock();
	m_options.insert(options.begin(), options.end());
}

inline const Section *Context::lookupSection() const
{
	if (m_cachedSection)
		return m_cachedSection;
	if (m_cachedSection = Sections::find(m_section))
		return m_cachedSection;
	DCORE_THROW(NotFound, "No section found by name: ", m_section);
}

inline void Context::copy(const Context &context)
{
	auto lock = m_lock.lock();
	auto _lock = context.m_lock.lock();

	m_options = context.m_options;
	m_section = context.m_section;
	m_cachedSection = context.m_cachedSection;
}

inline void Context::move(Context &&context)
{
	auto lock = m_lock.lock();
	auto _lock = context.m_lock.lock();

	m_options = std::move(context.m_options);
	m_section = std::move(context.m_section);
	m_cachedSection = std::move(context.m_cachedSection);
}

}
