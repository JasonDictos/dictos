#include <tests.hpp>
#include <catch/catch.hpp>
#include "LockInterfaceTests.hpp"

TEST_CASE("Mutex test", "[mutex]")
{
	LockInterfaceTests::basic<dictos::async::lock::MutexLock>();
}

TEST_CASE("Mutex link test", "[mutex_link]")
{
	LockInterfaceTests::link<dictos::async::lock::MutexLock>();
}
