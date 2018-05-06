# This sets up the core build config and platform definitions in the cmake global scope
# used everywhere to setup basic setup for all signalwire native projects.
macro(dictosutil_setup_platform)
	if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
		set(DCORE_PLAT_LIN 1 CACHE INTERNAL "Platform definition" FORCE)
		set(CMAKE_POSITION_INDEPENDENT_CODE YES)

		add_compile_options("$<$<CONFIG:RELEASE>:-O2>")
		add_compile_options("$<$<CONFIG:RELEASE>:-g>")

		add_compile_options("$<$<CONFIG:DEBUG>:-O0>")
		add_compile_options("$<$<CONFIG:DEBUG>:-g>")
		set(DCORE_COMPILE_DEFINITIONS -DDCORE_PLAT_LIN=1)

		set(CMAKE_POSITION_INDEPENDENT_CODE YES)

	elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
		add_compile_options("$<$<CONFIG:RELEASE>:-O2>")
		add_compile_options("$<$<CONFIG:RELEASE>:-g>")

		add_compile_options("$<$<CONFIG:DEBUG>:-O0>")
		add_compile_options("$<$<CONFIG:DEBUG>:-g>")

		set(DCORE_PLAT_MAC 1 CACHE INTERNAL "Platform definition" FORCE)
		set(CMAKE_POSITION_INDEPENDENT_CODE YES)

		set(DCORE_COMPILE_DEFINITIONS -DDCORE_PLAT_MAC=1)
	elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
		set(DCORE_PLAT_WIN 1 CACHE INTERNAL "Platform definition" FORCE)

		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

		set(DCORE_COMPILE_DEFINITIONS -D_WIN32_winn=0X0600 -DDCORE_PLAT_WINDOWS -D_SCL_SECURE_NO_WARNINGS -DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE -D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS -DNOMAXMIN -D_CRT_SECURE_NO_WARNINGS)
		set(DCORE_PLAT_WINDOWS 1)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /permissive- -DDCORE_PLAT_WINDOWS -D_SCL_SECURE_NO_WARNINGS -DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE -D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS -DNOMAXMIN -D_CRT_SECURE_NO_WARNINGS")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ")
	else()
		message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
	endif()

	# Default to debug if not specified
	if (NOT CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE "Debug" CACHE INTERNAL "Build config setting" FORCE)
	endif()

	# Select a default install prefix of /usr
	if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
		set(CMAKE_INSTALL_PREFIX "/usr" CACHE INTERNAL "Prefix prepended to install directories" FORCE)
		set(CMAKE_PREFIX_PATH "/usr" CACHE INTERNAL "Prefix search path" FORCE)
	endif()
endmacro()
