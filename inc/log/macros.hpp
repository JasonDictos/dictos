#pragma once

// Log with this ptr
#define LOGT(lvl, ...) ::dictos::log::writePrefix(#lvl, *this, __VA_ARGS__)

// Log with function/method name
#define LOGF(lvl, ...) ::dictos::log::writePrefix(#lvl, __FUNCTION__, __VA_ARGS__)

// Log with no this ptr or function/method name
#define LOG(lvl, ...) ::dictos::log::writePrefix(#lvl, #lvl, __VA_ARGS__)
