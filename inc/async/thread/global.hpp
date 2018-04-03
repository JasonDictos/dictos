#pragma once


namespace dictos::async::thread {

/**
 * There is one global manager that is by default used.
 */
inline TaskManager &getTaskManager()
{
	static TaskManager manager;
	return manager;
}

}
