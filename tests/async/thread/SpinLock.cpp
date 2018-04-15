#include <tests.hpp>
#include <catch/catch.hpp>
#include "LockInterfaceTests.hpp"

TEST_CASE("SpinLock basic test", "[spinlock_basic]")
{
	LockInterfaceTests::basic<dictos::async::lock::SpinLock>();
}

TEST_CASE("SpinLock link test", "[spinlock_link]")
{
	LockInterfaceTests::link<dictos::async::lock::SpinLock>();
}
