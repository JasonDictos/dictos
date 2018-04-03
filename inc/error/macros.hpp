#pragma once

// This throws makes an exception capturing the current file/line. It assumes
// the absolute namespace for the exception class.
#define DCORE_THROW(ExceptionType, ...)													\
	do																					\
	{																					\
		throw ::dictos::error::ExceptionType(											\
			__FILE__, __LINE__, ::dictos::string::toStringDelimiter(' ', __VA_ARGS__)	\
		);																				\
	} while (false)

// This throws makes an exception capturing the current file/line. The
// exception type is expected to take file/line/message.
#define DCORE_ERR_THROW(ExceptionType, ...)													\
	do																						\
	{																						\
		throw ExceptionType(																\
			__FILE__, __LINE__, ::dictos::string::toStringDelimiter(' ', __VA_ARGS__)		\
		);																					\
	} while (false)

// This macro makes an exception capturing the current file/line. The
// exception type is expected to take file/line/message.
#define DCORE_ERR_MAKE(ExceptionType, ...)													\
		ExceptionType(																		\
			__FILE__, __LINE__, ::dictos::string::toStringDelimiter(' ', __VA_ARGS__)		\
		)
