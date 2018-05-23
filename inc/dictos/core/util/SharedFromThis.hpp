#pragma once

namespace dictos::util {

/**
 * This simple class makes the standard shared from this class a little easier to use.
 */
template<class SharedType>
class SharedFromThis : public std::enable_shared_from_this<SharedType>
{
public:
	std::shared_ptr<SharedType> thisPtr() const {
		return const_cast<SharedFromThis *>(this)->shared_from_this();
	}
};

}
