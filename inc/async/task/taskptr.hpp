#pragma once

namespace dictos::async::task {

typedef std::shared_ptr<class TaskInterface> TaskPtr;
typedef std::weak_ptr<class TaskInterface> TaskWPtr;
typedef class TaskInterface* TaskRPtr;

}
