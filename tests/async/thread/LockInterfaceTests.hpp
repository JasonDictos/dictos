#pragma once

struct LockInterfaceTests
{
	template<class LockType>
	static void basic()
	{
		LockType lock;
		REQUIRE(lock.isLocked() == false);
		REQUIRE(lock.isLockedByMe() == false);
		REQUIRE(lock.getLockCount() == 0);
		auto guard1 = lock.lock();
		REQUIRE(lock.isLocked() == true);
		REQUIRE(lock.isLockedByMe() == true);
		REQUIRE(lock.getLockCount() == 1);
		auto guard2 = lock.lock();
		REQUIRE(lock.isLocked() == true);
		REQUIRE(lock.isLockedByMe() == true);
		REQUIRE(lock.getLockCount() == 2);
//		auto guard3 = lock.rewind();
//		REQUIRE(lock.isLocked() == false);
//		REQUIRE(lock.isLockedByMe() == false);
//		REQUIRE(lock.getLockCount() == 0);
//		guard3.release();
//		REQUIRE(lock.isLocked() == true);
//		REQUIRE(lock.isLockedByMe() == true);
//		REQUIRE(lock.getLockCount() == 2);
		guard2.release();
		REQUIRE(lock.isLocked() == true);
		REQUIRE(lock.isLockedByMe() == true);
		REQUIRE(lock.getLockCount() == 1);
		guard1.release();
		REQUIRE(lock.isLocked() == false);
		REQUIRE(lock.isLockedByMe() == false);
		REQUIRE(lock.getLockCount() == 0);
	}

	template<class LockType>
	static void link()
	{
		LockType lock;
		LockType lock2(lock);
		REQUIRE(lock.isLocked() == false);
		REQUIRE(lock.isLockedByMe() == false);
		REQUIRE(lock2.isLocked() == false);
		REQUIRE(lock2.isLockedByMe() == false);
		auto guard1 = lock.lock();
		REQUIRE(lock.isLocked() == true);
		REQUIRE(lock.isLockedByMe() == true);
		REQUIRE(lock.getLockCount() == 1);
		REQUIRE(lock2.isLocked() == true);
		REQUIRE(lock2.isLockedByMe() == true);
		REQUIRE(lock2.getLockCount() == 1);
		auto guard2 = lock.lock();
		REQUIRE(lock.isLocked() == true);
		REQUIRE(lock.isLockedByMe() == true);
		REQUIRE(lock.getLockCount() == 2);
		REQUIRE(lock2.isLocked() == true);
		REQUIRE(lock2.isLockedByMe() == true);
		REQUIRE(lock2.getLockCount() == 2);
//		auto guard3 = lock.rewind();
//		REQUIRE(lock.isLocked() == false);
//		REQUIRE(lock.isLockedByMe() == false);
//		REQUIRE(lock.getLockCount() == 0);
//		REQUIRE(lock2.isLocked() == false);
//		REQUIRE(lock2.isLockedByMe() == false);
//		REQUIRE(lock2.getLockCount() == 0);
//		guard3.release();
//		REQUIRE(lock.isLocked() == true);
//		REQUIRE(lock.isLockedByMe() == true);
//		REQUIRE(lock.getLockCount() == 2);
//		REQUIRE(lock2.isLocked() == true);
//		REQUIRE(lock2.isLockedByMe() == true);
//		REQUIRE(lock2.getLockCount() == 2);
		guard2.release();
		REQUIRE(lock.isLocked() == true);
		REQUIRE(lock.isLockedByMe() == true);
		REQUIRE(lock.getLockCount() == 1);
		REQUIRE(lock2.isLocked() == true);
		REQUIRE(lock2.isLockedByMe() == true);
		REQUIRE(lock2.getLockCount() == 1);
		guard1.release();
		REQUIRE(lock.isLocked() == false);
		REQUIRE(lock.isLockedByMe() == false);
		REQUIRE(lock.getLockCount() == 0);
		REQUIRE(lock2.isLocked() == false);
		REQUIRE(lock2.isLockedByMe() == false);
		REQUIRE(lock2.getLockCount() == 0);
	}
};
