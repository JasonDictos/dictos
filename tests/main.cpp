#define CATCH_CONFIG_MAIN
#include <tests.hpp>
#include <catch.hpp>
#include <dictos/core/test/CatchListener.hpp>

using namespace dictos;
using namespace dictos;

namespace {
	static dictos::log::StreamBuffer s_stream("Catch");
	static std::streambuf * s_oldBuffer = nullptr;
}

util::Scope init(
	[&]()
{
	// Redirect std::cout rdbuf to go to our logger
	s_oldBuffer = std::cout.rdbuf(&s_stream);

	// Enable testing output
	log::enableLevel("all");
},
	[&]() { std::cout.rdbuf(s_oldBuffer); }
);

// Register our listener for logging
CATCH_REGISTER_LISTENER(CatchListener);
