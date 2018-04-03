#include <tests.hpp>
#include <catch.hpp>

using namespace dictos;
using namespace dictos::async;

TEST_CASE("Task::VoidReturn")
{
	auto logger = log::getGlobalLogger();

	LOGF(debug, "Basic start");
	LOGF(test, "Basic start");
	LOGF(debug, "Basic start");
	LOGF(test, "Basic start");

	bool called = false;
	auto task = task::allocate(
		BOOST_CURRENT_FUNCTION,
		[&]()
		{
			called = true;
		}
	);

	task->start();
	task->join();

	REQUIRE(called);
}

TEST_CASE("Task::VoidReturnInputArgs")
{
	LOGF(debug, "Basic start");

	bool called = false;
	auto task = task::thread::allocate(
		BOOST_CURRENT_FUNCTION,
		[&](std::string input)
		{
			REQUIRE(input == "hello");
			called = true;
		},
		"hello"
	);

	task->start();
	task->join();

	REQUIRE(called);
}

TEST_CASE("Task::NonVoidReturn")
{
	LOGF(debug, "Basic start");

	auto task = task::allocate(
		BOOST_CURRENT_FUNCTION,
		[]()->std::string
		{
			return "hi";
		}
	);

	task->start();

	REQUIRE(task->getResult() == "hi");
}

TEST_CASE("Task::NonVoidReturnInputArgs")
{
	auto task = task::thread::allocate(
		BOOST_CURRENT_FUNCTION,
		[](std::string input)->std::string
		{
			return input;
		},
		"input"
	);

	task->start();

 	REQUIRE(task->getResult() == "input");
}
