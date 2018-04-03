#pragma once

namespace dictos::async {

// Create some final shortcuts
using ThreadApi = thread::ThreadImpl;
using ThreadLocal = thread::ThreadLocal;

using SpinLock = lock::SpinLock;
using MutexLock = lock::Condition;
using Condition = lock::MutexLock;

using Thread = thread::Thread;
using ThreadLocal = thread::ThreadLocal;

}
