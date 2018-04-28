#pragma once

namespace dictos::async::lock {

using SpinLockImpl = GenericSpinLockImpl<thread::ThreadImpl>;
using MutexLockImpl = GenericMutexLockImpl<thread::ThreadImpl>;

using MutexLock = GenericLock<MutexLockImpl>;
using SpinLock = GenericLock<SpinLockImpl>;
using Condition = GenericCondition<MutexLockImpl>;

}
