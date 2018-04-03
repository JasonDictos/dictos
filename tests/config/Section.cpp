#include <tests.hpp>
#include <catch.hpp>

using namespace dictos;

class MyLogger : public config::Context
{
public:
	MyLogger() :
		Context("log", {{"consoleOutput", true, "Write console output"}})
	{
	}
};

TEST_CASE("Section setup", "[config:section]")
{
	MyLogger log;
	REQUIRE(log.getOption<bool>("consoleOutput") == true);
	log.setOption("consoleOutput", false);
	REQUIRE(log.getOption<bool>("consoleOutput") == false);
}
