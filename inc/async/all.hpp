// Include generic base types
#include <dictos/core/async/external.hpp>
#include <dictos/core/async/Id.hpp>
#include <dictos/core/async/current.h>
#include <dictos/core/async/RunnableInterface.hpp>
#include <dictos/core/async/Callback.hpp>
#include <dictos/core/async/block.hpp>
#include <dictos/core/async/GenericImpl.hpp>
#include <dictos/core/async/GenericLocal.hpp>

// Thread specializations
#include <dictos/core/async/thread/all.hpp>

// Lock support
#include <dictos/core/async/lock/all.hpp>

// Foundational generics which use locks can now be included
#include <dictos/core/async/GenericRunnable.hpp>
#include <dictos/core/async/GenericCurrentRunnable.hpp>

// Now that we got locks, the rest of our specializations in thread
#include <dictos/core/async/thread/all2.hpp>

// Safe to setup our types
#include <dictos/core/async/thread/type.hpp>
#include <dictos/core/async/type.hpp>

// The current unified api
#include <dictos/core/async/current.hpp>

// And finally task
#include <dictos/core/async/task/all.hpp>
