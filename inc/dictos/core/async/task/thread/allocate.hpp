#pragma once

namespace dictos::async::task::thread {

template<class Func, class ...Args>
inline auto allocate(std::string name, Func func, Args... args)
{
	return std::make_shared<GenericResultTask<thread::TaskImpl, Func, Args...>>(
		std::forward<std::string>(name),
		std::forward<Func>(func),
		std::forward<Args&&>(args)...
	);
}

template<class Func, class ...Args>
inline auto start(std::string name, Func func, Args... args)
{
	auto task = std::make_shared<GenericResultTask<thread::TaskImpl, Func, Args...>>(
		std::forward<std::string>(name),
		std::forward<Func>(func),
		std::forward<Args&&>(args)...
	);

	task->start();
	return task;
}

}
