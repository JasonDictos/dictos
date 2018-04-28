#pragma once

namespace dictos::log {

template<class ...Lvls>
bool isLevelEnabled(const Lvls &... lvls);

template<class ...Lvls>
void enableLevel(const Lvls &... lvls);

template<class ...Lvls>
void disableLevel(const Lvls &... lvls);

template<class From>
std::string getPrefix(const From &from);

template<class Prefix, class ...Args>
void writePrefix(const std::string_view &level, const Prefix &prefix, const Args&... args);

template<class ...Args>
void write(const std::string_view &level, const Args&... args);

template<class ...Args>
void write(const Args&... args);

}
