#pragma once

namespace dictos::memory {

/**
 * Returns true if it can determine if an iterator sequence backs
 * contiguous memory or not.
 */
template<class I>
inline auto isContiguous(I first, I last)
{
    auto test = true;
    auto const n = std::distance(first, last);
    for (auto i = 0; i < n && test; ++i) {
        test &= *(std::next(first, i)) == *(std::next(std::addressof(*first), i));
    }
    return test;
}

}
