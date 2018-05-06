#pragma once

namespace dictos::string {

template<typename S, typename T>
class CanBeOutStreamedTrait
{
    template<typename SS, typename TT>
    static auto test(int)
    -> decltype( std::declval<SS&>() << std::declval<TT>(), std::true_type() );

    template<typename, typename>
    static auto test(...) -> std::false_type;

public:
    static const bool value = decltype(test<S,T>(0))::value;
};

}
