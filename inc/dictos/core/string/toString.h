#pragma once

namespace dictos::string {

template<class ...Args>
std::string toString(Args&&...);

template<class Delimiter, class ...Args>
std::string toStringDelimiter(const Delimiter &, Args&&...);

}
