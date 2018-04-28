#pragma once

namespace dictos::config {

class Option
{
public:
	friend class Section;

	Option(std::string_view name, std::any value = std::any(), std::string_view description = std::string_view()) noexcept :
		m_name(name),
		m_description(description),
		m_value(std::move(value))
	{
	}

	bool operator < (const Option &option) const noexcept { return m_name < option.m_name; }
	bool operator == (const Option &option) const noexcept { return m_name == option.m_name; }

	template<class Type>
	inline Type process(Type value) const
	{
		if (!m_value.has_value())
			return value;

		getDefault<Type>();
		return value;
	}

	template<class Type>
	inline Type getDefault() const
	{
		if (!m_value.has_value())
			DCORE_THROW(RuntimeError, "Option:", m_name, "has no default");

		try
		{
			return std::any_cast<Type>(m_value);
		}
		catch (const std::bad_any_cast &e)
		{
			DCORE_THROW(InvalidCast,
				"Failed to cast option:", m_name,
				"to", log::getTypeName<Type>(),
				"(", e.what(), ")"
			);
		}
	}

	const std::any & getValue() const noexcept { return m_value; }
	std::string_view getName() const noexcept { return m_name; }
	std::string_view getDescription() const noexcept { return m_description; }

protected:
	std::string_view m_name;
	std::string_view m_description;
	std::any m_value;
};

}

