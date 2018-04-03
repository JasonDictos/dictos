#include <tests.hpp>
#include <catch.hpp>

using namespace dictos;

TEST_CASE("Size Basics")
{
	auto mb = Size::megabytes(1);
	REQUIRE(mb.asBytes() == 1024 * 1024);
}
