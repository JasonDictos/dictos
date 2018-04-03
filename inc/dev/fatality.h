#pragma once

namespace dictos::dev {

/**
 * This is the end all call, the app will exit, the debugger will be entered into
 * if its attached.
 */
[[noreturn]] void fatality(const std::string &reason, const char *filename, int line);

}
