#include <tests.hpp>
#include <catch/catch.hpp>

using namespace dictos;

TEST_CASE("memory::Heap Basics")
{
	auto oneMb = memory::Heap(Size::megabytes(1));
	REQUIRE(oneMb.size() == Size::megabytes(1));
}
