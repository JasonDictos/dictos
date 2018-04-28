#pragma once

namespace dictos::async::thread {

using Thread = GenericRunnable<RunnableImpl>;
using ThreadLocal = RunnableImpl::LocalImplType;

}
