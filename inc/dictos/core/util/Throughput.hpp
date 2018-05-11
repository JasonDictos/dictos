#pragma once

namespace dictos::util {

/**
 * The throughput class is a rolling window averaging rate
 * calculator capable of calculating a throughout or a basic
 * per second rate.
 */
class Throughput
{
public:
	// Declare our bucket entry, this describes a point in time
	// with a max and min amount of entries
	struct Bucket
	{
		Size size;
		Count count;
	};

	/**
	 * The stats entry gets returned to snapshot the
	 * current state of the throughput during a run.
	 */
	struct Stats
	{
		// These fields are set as the 'current size/count/rate' of the active buckets while the throughput is
		// started, when it is stopped they represent the 'summary size/count/rate' which is calculated
		// by the total run time
		Size size;
		Count count;
		Size rateSize;
		Count rateCount;

		// These fields represent copies of the values from the Throughput object itself
		Count totalCount;
		Size totalSize;
		time::seconds startTime;
		time::seconds stopTime;
		time::seconds runTime;
	};

	bool started() const { return m_started; }

	Size totalSize() const
	{
		auto guard = m_spinLock.lock();
		return m_totalSize;
	}

	uint32_t totalCount() const
	{
		auto guard = m_spinLock.lock();
		return m_totalCount;
	}

	// Sets up the throughput for use, sets the bucket count
	// on the rolling window.
	Throughput(uint32_t maxBuckets = 32, time::seconds interval = time::seconds(1)) :
		m_maxBuckets(maxBuckets), m_interval(interval)
	{
		// Initialize our buckets with our bucket entry template
		// we add 1 since one is always the current bucket being
		// aggregated, which is not used in computations of stats
		m_buckets.push_front(Bucket());
	}

	// We provide a now call mostly so unit testst can mock this so we can
	// manually be in control of the clock.
	virtual time::seconds currentTimestamp() const
	{
		return time::duration_cast<time::seconds>(time::now<time::CLOCK::Utc>().time_since_epoch());
	}

	// Starts the throughput virtual timer (no actual thread is allocated)
	// and initializes our state.
	void start()
	{
		auto guard = m_spinLock.lock();

		if (m_started) {
			DCORE_THROW(RuntimeError, "Throughput has already been started");
		}

		initializeBuckets();

		// Initialize our state
		m_startTime = currentTimestamp();
		m_stopTime = time::seconds(0);
		m_started = true;
		m_totalSize = 0;
		m_totalCount = 0;
		m_lastUpdateTime = m_startTime;
	}

	// Stops the virtual timer and sets the final stop time for final
	// summary rate calculations.
	void stop()
	{
		auto guard = m_spinLock.lock();
		if (m_started) {
			m_started = false;
			m_stopTime = currentTimestamp();
		}
	}

	// Stops/starts the virtual rate timer.
	void restart()
	{
		stop();
		start();
	}

	// Returns the total size processed till now.
	Size currentSize()
	{
		Size size;

		auto guard = m_spinLock.lock();

		// If we've stopped, return total size
		if (!m_started && m_stopTime != time::seconds(0))
			size = m_totalSize;
		else {
			// Update while we're here
			update();

			// Add up all the buckets sizes
			for (auto &bucket : m_buckets)
				size += bucket.size;
		}

		return size;
	}

	// Returns the total count of processed items till now.
	Count currentCount()
	{
		Count count;

		auto guard = m_spinLock.lock();

		// If we've stopped, return total count
		if (!m_started && m_stopTime != time::seconds(0))
			count = m_totalCount;
		else {
			// Update while we're here
			update();

			// Add up all the buckets sizes
			for (auto &bucket : m_buckets)
				count += bucket.count;
		}

		return count;
	}

	// This function is what gets called by the user as they complete work.
	// It will populate the current bucket or roll to the next bucket as needed.
	bool report(Size size = 0, bool implicitStart = true)
	{
		auto guard = m_spinLock.lock();

		// Start implicitly if need be
		if (m_started == false) {
			if (implicitStart)
				start();
			else
				return false;
		}

		// Roll our windows forward if needed
		update();

		// Update the current bucket (the first one)
		auto &bucket = m_buckets.front();

		bucket.count += 1;
		bucket.size += size;

		m_totalSize += size;
		m_totalCount += 1;
		return true;
	}

	// Moves time forward based on the current time distance from
	// next interval. Once we cross that threshold we move new buckets
	// in front, expiring old ones off the end as we go. This is the 'tick'
	// function of this class and its what drives our sliding window forward.
	void update()
	{
		auto guard = m_spinLock.lock();

		// If we've been stopped, keep the buckets exactly as they are
		if (m_started == false)
			return;

		// Compare that to our last update, and divide that by our interval, thats how many
		// buckets we have to move forward
		auto elapsedTime = currentTimestamp() - m_lastUpdateTime;
		auto elapsedBuckets = elapsedTime / m_interval;

		// If we've gone beyond the current one, push as many in as we've elapsed
		if (elapsedBuckets) {
			// We'll progress in fixed chunks of our interval time
			m_lastUpdateTime += m_interval * elapsedBuckets;

			// Roll our buckets forward x times
			rollForward(elapsedBuckets);
		}
	}

	// Calculates an average throughput.
	template<class Type>
	static double calculateAverage(Type size, time::seconds duration)
	{
		if (duration != time::seconds(0))
			return size / duration;
		return 0;
	}

	// Returns the duration of the total time ran between start and now
	// or start and stop.
	time::seconds runTime() const
	{
		if (m_started)
			return currentTimestamp() - m_startTime;
		else if (m_stopTime != time::seconds(0))
			return m_stopTime - m_startTime;
		else
			return time::seconds(0);
	}

	// Returns a summarized stats structure of all possible
	// stored statistics, it is with this api that we allow for a
	// custom bucket limit, allowing the caller to get stats for
	// portions of the window.
	Stats stats()
	{
		Stats stats;

		auto guard = m_spinLock.lock();

		update();

		stats.startTime = m_startTime;
		stats.stopTime = m_stopTime;
		stats.runTime = runTime();
		stats.totalSize = m_totalSize;
		stats.totalCount = m_totalCount;

		if (!m_started && stats.runTime != time::seconds(0)) {
			// Inactive, but have been ran before, calculate based on
			// totals and run time
			stats.size = m_totalSize;
			stats.count = m_totalCount;

			stats.rateSize = calculateAverage(stats.size, stats.runTime);
			stats.rateCount = calculateAverage(stats.count, stats.runTime);
		} else if (completedBucketCount()) {
			// Active so, size/count/rates all represent the 'current' calculation based on
			// active buckets
			stats.size = currentSize();
			stats.count = currentCount();

			// If we're active and something got completed, report the current rate up till now
			stats.rateSize = calculateRate(stats.size);
			stats.rateCount = calculateRate(stats.count);
		}

		return stats;
	}

private:
	// Returns the number of buckets if we are started, otherwise we
	// return 0 since the buckets are inactive when stopped.
	uint32_t completedBucketCount()
	{
		auto guard = m_spinLock.lock();
		uint32_t completedBuckets = 0;

		if (m_started)
			completedBuckets = boost::numeric_cast<uint32_t>(m_buckets.size()) - 1;

		return completedBuckets;
	}

	void rollForward(uint32_t count)
	{
		auto guard = m_spinLock.lock();
		for (auto i = 0; i < count; i++)
			m_buckets.emplace_front(Bucket());
		while (m_buckets.size() >= m_maxBuckets)
			m_buckets.pop_back();
	}

	// Internal function for calculating rates based on our interval limit.
	template<class Type>
	double calculateRate(Type amount)
	{
		double rate = 0;
		auto completedSeconds = m_interval * completedBucketCount();

		if (completedSeconds != time::seconds(0))
			return amount / completedSeconds;

		return rate;
	}

	// Clears out all the buckets with default values.
	void initializeBuckets()
	{
		auto guard = m_spinLock.lock();
		m_buckets = { Bucket() };
	}

private:
	// The interval for the bucket calculator, anytime we get
	// called to update we'll figure out how long its been since the
	// last and roll the buckets forward
	time::seconds m_interval;

	// This flag indicates whether we are started or not
	std::atomic<bool> m_started;

	// Timestamps for calls to start and stop.
	time::seconds m_startTime, m_stopTime;

	time::seconds startTime() const { return m_startTime; }
	time::seconds stopTime() const { return m_stopTime; }

	// Totals kept between start/stop runs.
	Size m_totalSize;
	Count m_totalCount;

	// This is our 'current position' in time.
	time::seconds m_lastUpdateTime;

	// Since ruby is threaded, this class is locked
	mutable async::SpinLock m_spinLock;

	// Our fixed queue, where each bucket lives
	std::list<Bucket> m_buckets;

	// Max cap on the buckets
	uint32_t m_maxBuckets;
};

}
