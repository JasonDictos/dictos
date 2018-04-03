#pragma once

namespace dictos::async {

bool isCancelled() noexcept;
void cancel() noexcept;
void checkCancelled();
template<class Rep, class Int>
void sleep(time::duration<Rep, Int> wait, bool ccheck = true);
void yield(bool ccheck = true);
Id getCurrentId() noexcept;

}
