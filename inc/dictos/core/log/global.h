#pragma once

namespace dictos::log {

DPUB dev::BasicSpinLock::Guard checkRecursion();
DPUB Logger *GlobalLogger();

}
