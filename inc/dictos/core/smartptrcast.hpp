#pragma once

namespace dictos {

/*
template<class To, class From>
using staticPtrCast = std::static_pointer_cast<To, From>;

template<class To, class From>
using dynamicPtrCast = std::dynamic_pointer_cast<To, From>;
*/

template<typename To, typename From>
const To * staticUPtrCast(const std::unique_ptr<From> &old)
{
    return static_cast<const To *>(old.get());
}

template<typename To, typename From>
To * staticUPtrCast(std::unique_ptr<From> &old)
{
    return static_cast<To *>(old.get());
}

}
