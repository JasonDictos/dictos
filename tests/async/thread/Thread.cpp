#include <tests.hpp>
#include <catch/catch.hpp>

using namespace dictos;
using namespace dictos::async;

TEST_CASE("Thread::Basic")
{
	auto ran = false;
	Thread thread(
		BOOST_CURRENT_FUNCTION,
		[&]()
		{
			ran = true;
		}
	);
	thread.start();
	thread.join();
	REQUIRE(ran == true);
}

TEST_CASE("Thread::Args")
{
	auto ran = false;
	std::string arg;
	Thread thread(
		BOOST_CURRENT_FUNCTION,
		[&](const std::string &_arg)
		{
			ran = true;
			arg = _arg;
		},
		"bobo"
	);
	thread.start();
	thread.join();
	REQUIRE(ran == true);
	REQUIRE(arg == "bobo");
}

TEST_CASE("Thread::Cancel")
{
	auto ran = false;
	Thread thread(
		BOOST_CURRENT_FUNCTION,
		[&]()
		{
			LOG(debug, "Thread run in test");
			while(!isCancelled())
				yield();
			ran = true;
		}
	);

	LOG(debug, "start/stop start");
	thread.start();
	sleep(time::seconds(1));
	thread.cancel();
	thread.join();
	LOG(debug, "start/stop stop");
	REQUIRE(ran == true);
}

TEST_CASE("Thread::CheckSleep")
{
	auto ran = false;
	Thread thread(
		BOOST_CURRENT_FUNCTION,
		[&]()
		{
			ran = true;
		}
	);

	LOG(debug, "Check sleep start/stop begin");

	thread.start();
	thread.stop();

	LOG(debug, "Check sleep start/stop end");

	REQUIRE(ran == true);
}
