#pragma once

namespace dictos::config {

void put(const std::string &key, std::any value);

template<class T>
T get(const std::string &key);

}
