#pragma once

#define DCORE_ASSERT(x)									\
	do													\
	{													\
		if ((x))										\
			break;										\
														\
		::dictos::dev::fatality(					\
			::dictos::string::toString(			\
				"Assertion failed for:", #x				\
			),     										\
			__FILE__,									\
			__LINE__									\
		);												\
	} while (false)										\

#define DCORE_ASSERTM(x, msg)							\
	do													\
	{													\
		if ((x))										\
			break;										\
														\
		::dictos::dev::fatality(					\
			::dictos::string::toString(			\
				"Assertion failed for:", #x,			\
				"Message:", msg							\
			),											\
			__FILE__,									\
			__LINE__									\
		);												\
	} while (false)


