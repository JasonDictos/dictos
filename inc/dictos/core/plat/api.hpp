#pragma once

namespace dictos::plat {

inline std::string getHostName() {
  std::array<char, 128> host = {0};
  if (gethostname(&host.front(), boost::numeric_cast<int>(host.size()))) {
    DCORE_THROW(RuntimeError, "Error getting local hostname");
  }
  return &host.front();
}

}
