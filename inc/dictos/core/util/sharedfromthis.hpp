#pragma once

namespace dictos::util {

/**
 * This simple class makes the standard shared from this class a little easier to use.
 */
template<class Type>
class SharedFromThis : public std::enable_shared_from_this<Type> 
{
public:
	std::shared_ptr<Type> thisPtr() const {
		return const_cast<SharedFromThis *>(this)->enable_shared_from_this<Type>::shared_from_this();
	}
};

}
