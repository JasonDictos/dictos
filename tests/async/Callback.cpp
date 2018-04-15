#include <tests.hpp>
#include <catch/catch.hpp>

using namespace dictos::async;

TEST_CASE("Callback::Basic")
{
	Callback callback(
		[&](std::string val)
		{
			REQUIRE(val == "bobo");
		},
		"bobo"
	);

	callback.invoke();
}

TEST_CASE("Callback::Basic2")
{
	Callback callback(
		[&](std::string val, int henry)
		{
			REQUIRE(val == "bobo");
			REQUIRE(henry == 5);
		},
		"bobo",
		5
	);

	callback.invoke();
}

TEST_CASE("Callback::Basic3")
{
	auto called = true;
	Callback callback(
		[&]()
		{
			called = true;
		}
	);

	callback.invoke();
}

TEST_CASE("Callback::InvokerNonVoidReturn")
{
	auto invoker = Callback::allocateInvoker(
		[&]()->std::string
		{
			return "hello";
		}
	);

	invoker->invoke();

	REQUIRE(invoker->m_result == "hello");
}

TEST_CASE("Callback::InvokerVoidReturn")
{
	bool called = false;
	auto cb = [&]()
		{
			called = true;
		};

	auto invoker = Callback::allocateInvoker(cb);

	invoker->invoke();
	REQUIRE(called == true);
}

TEST_CASE("Callback::InvokerVoidReturnWithArgs")
{
	bool called = false;
	auto cb = [&](const std::string &question)
		{
			REQUIRE(question == "huh?");
			called = true;
		};

	auto invoker = Callback::allocateInvoker(cb, "huh?");

	invoker->invoke();
	REQUIRE(called == true);
}

TEST_CASE("Callback::InvokerNonVoidReturnWithArgs")
{
	auto cb = [](const std::string &question)->std::string
		{
			if (question == "huh?")
				return "what?";
			else
				return "no idea";
		};

	auto invoker = Callback::allocateInvoker(cb, "huh?");

	invoker->invoke();
	REQUIRE(invoker->m_result == "what?");

	invoker = Callback::allocateInvoker(cb, "");

	invoker->invoke();
	REQUIRE(invoker->m_result == "no idea");
}
