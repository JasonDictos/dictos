#pragma once

namespace dictos::async::task::thread {

using Task = GenericTask<TaskImpl>;
using TaskManager = GenericManager<TaskImpl>;

}
