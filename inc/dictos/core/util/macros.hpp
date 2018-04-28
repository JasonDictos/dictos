#pragma once

#define DCORE_ALIAS_FUNCTION(aliasSource, aliasTarget) \
template<typename... Args> \
inline auto aliasSource(Args&&... args) -> decltype(aliasTarget(std::forward<Args>(args)...)) \
{ \
    return aliasTarget(std::forward<Args>(args)...); \
}

#define DCORE_ALIAS_STATIC_FUNCTION(aliasSource, aliasTarget) \
template<typename... Args> \
inline static auto aliasSource(Args&&... args) -> decltype(aliasTarget(std::forward<Args>(args)...)) \
{ \
    return aliasTarget(std::forward<Args>(args)...); \
}
