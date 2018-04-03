namespace dictos::config {

/**
 * The config context holds config settings and is usually inherited
 * from but can act as a stand alone config object for any section.
 */
class Context
{
public:
	Context(std::string section, Options options = Options());
	Context(const Section &section, Options options = Options());

	template<class Type>
	Type getOption(const std::string &key, const Type &def) const;

	template<class Type>
	Type getOption(const std::string &key) const;

	void setOption(std::string key, std::any value);

	void setOptions(Options options);
	Options getOptions() const;

	void mergeOptions(Options options);

protected:
	const Section *lookupSection() const;

	mutable dev::BasicSpinLock m_lock;
	Options m_options;
	const std::string m_section;
	mutable const Section *m_cachedSection = nullptr;
};

}
