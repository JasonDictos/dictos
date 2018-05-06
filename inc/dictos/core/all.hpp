#pragma once

// I've always been a fan of the forever definition
#define forever while(true)

// Setup our foundation
#include <dictos/core/external.hpp>
#include <dictos/core/smartptrcast.hpp>
#include <dictos/core/dev/all.hpp>
#include <dictos/core/plat/all.hpp>
#include <dictos/core/string/all.hpp>
#include <dictos/core/file/all.hpp>
#include <dictos/core/config/all.hpp>
#include <dictos/core/log/all.hpp>
#include <dictos/core/string/all2.hpp>

// Bind string literals into dictos namespace
namespace dictos {
	using namespace string::literals;
}

#include <dictos/core/dev/all2.hpp>
#include <dictos/core/error/all.hpp>
#include <dictos/core/time/all.hpp>

#include <dictos/core/numericCast.hpp>
#include <dictos/core/Size.hpp>
#include <dictos/core/Uuid.hpp>
#include <dictos/core/literals.hpp>
#include <dictos/core/memory/all.hpp>

// Bootstrap some util stuff that doesn't depend on async directly
#include <dictos/core/util/macros.hpp>
#include <dictos/core/util/external.hpp>
#include <dictos/core/util/Scope.hpp>

// From here on out we start getting higher level, locks etc. so need to
// start including async
#include <dictos/core/async/all.hpp>

// Now we can include the rest of log as we have mutex now
#include <dictos/core/config/all2.hpp>
#include <dictos/core/log/all2.hpp>

#include <dictos/core/util/all.hpp>
