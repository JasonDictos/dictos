#pragma once

namespace dictos::log {

/**
 * Given any type, returns its name using RTTI.
 */
template <class T>
inline std::string getTypeName()
{
	typedef typename std::remove_reference<T>::type TR;

#if defined(DCORE_PLAT_LIN)
	int32_t status = 0;
    return abi::__cxa_demangle(typeid(TR).name(), 0, 0, &status);
#else
	auto name = typeid(TR).name();
	return string::removeLeading(name, "class ");
#endif
}
template <class T>
inline std::string getTypeName(const T &thing)
{
	return getTypeName<T>();
}

}
