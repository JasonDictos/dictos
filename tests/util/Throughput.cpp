#include "tests.hpp"
#include <catch/catch.hpp>

using namespace dictos;

/**
 * Mock out our throughput so we can advance time manually 
 */
class FakeThroughput : public util::Throughput
{
	using Throughput::Throughput;

public:
	time::seconds currentTimestamp() const override
	{
		return m_currentTimestamp;
	}

	std::vector<Bucket> getBuckets() const {
		return std::vector<Bucket>(m_buckets.begin(), m_buckets.end());
	}

	Bucket getCurrentBucket() const { return m_currentBucket; }

	using Throughput::completedBucketCount;

	time::seconds m_currentTimestamp = time::seconds(10);
};

TEST_CASE("[throughput]")
{
	FakeThroughput rate(2, time::seconds(1));
    auto buckets = rate.getBuckets();
    auto current = rate.getCurrentBucket();
    REQUIRE(buckets.size() == 0);

    rate.report(1024);

    buckets = rate.getBuckets();
	current = rate.getCurrentBucket();
	REQUIRE(buckets.size() == 0);
	REQUIRE(rate.completedBucketCount() == 0);

	REQUIRE(current.count == 1);
	REQUIRE(current.size == 1024);

    // Advance time by one second
	rate.m_currentTimestamp += time::seconds(1);
    rate.update();
	REQUIRE(rate.completedBucketCount() == 1);

    buckets = rate.getBuckets();
	current = rate.getCurrentBucket();

    REQUIRE(buckets.size() == 1);

	REQUIRE(current.count == 0);
	REQUIRE(current.size == 0);
    REQUIRE(buckets[0].count == 1);
    REQUIRE(buckets[0].size == 1024);

    // Now we should have a rate
    auto stats = rate.stats();
    REQUIRE(stats.rateCount == 1.0);
    REQUIRE(stats.rateSize == 1024.0);

    // If we report, without advancing the time, the rate shouldn't change but the first
    // bucket should be staged
    rate.report(1024);

    buckets = rate.getBuckets();
	current = rate.getCurrentBucket();

    stats = rate.stats();
	REQUIRE(rate.completedBucketCount() == 1);
    REQUIRE(stats.rateCount == 1.0);
    REQUIRE(stats.rateSize == 1024.0);

    // As the first bucket is still 'uncompleted' but it should be additive
    REQUIRE(current.count == 1);
    REQUIRE(current.size == 1024);
    REQUIRE(buckets[0].count == 1);
    REQUIRE(buckets[0].size == 1024);

    // Ok advance the clock by 1 second and update to see if it continues
    // to return the proper rate
    rate.m_currentTimestamp += time::seconds(1);
    rate.update();
    stats = rate.stats();
    buckets = rate.getBuckets();
	current = rate.getCurrentBucket();

    // Should have a new bucket at 0 with 1-2 rolled forward
    REQUIRE(current.count == 0);
    REQUIRE(current.size == 0);
    REQUIRE(buckets[0].count == 1);
    REQUIRE(buckets[0].size == 1024);
    REQUIRE(buckets[1].count == 1);
    REQUIRE(buckets[1].size == 1024);

    // Same rate as before
    REQUIRE(stats.rateSize == 1024.0);
    REQUIRE(stats.rateCount == 1.0);

    // Now advance the time without reporting anything, we should start
    // seeing the rate trend downwards
    rate.m_currentTimestamp += time::seconds(1);
    rate.update();

    stats = rate.stats();
    buckets = rate.getBuckets();
	current = rate.getCurrentBucket();

	REQUIRE(current.count == 0);
	REQUIRE(current.size == 0);
    REQUIRE(buckets[0].count == 0);
    REQUIRE(buckets[0].size == 0);
    REQUIRE(buckets[1].count == 1);
    REQUIRE(buckets[1].size == 1024);

    // Rate should be cut in half
    REQUIRE(stats.rateSize == 1024.0 / 2);
    REQUIRE(stats.rateCount == 1.0 / 2);

    // Finally verify we can render the stats
	LOG(TEST, stats);

}
