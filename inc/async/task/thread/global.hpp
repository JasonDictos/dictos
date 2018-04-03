#pragma once

namespace dictos::async::task::thread {

inline std::shared_ptr<ManagerInterface> getManager()
{
	static util::CachedShared<TaskManager> manager;
	return manager.get();
}

}
