#include "tests.hpp"
#include <catch/catch.hpp>

using namespace dictos;

TEST_CASE("[throughput]")
{
	util::Throughput rate;

	forever {
		rate.report();
		LOG(TEST, "Current rate:", rate.stats());
	}
}
