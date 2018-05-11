#pragma once

namespace dictos::async {

// Create some final shortcuts
using ThreadApi = thread::ThreadImpl;
using ThreadLocal = thread::ThreadLocal;

using SpinLock = lock::SpinLock;
using MutexLock = lock::MutexLock;
using Condition = lock::Condition;

using Thread = thread::Thread;
using ThreadLocal = thread::ThreadLocal;

}
