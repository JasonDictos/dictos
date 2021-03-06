cmake_minimum_required(VERSION 3.10.2)

include(cmake/cotire.cmake)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# Setup hunter gate to auto grab our 3rd party dependencies
# (may be included by our super project so check HUNTER_WIKI)
if (NOT HUNTER_WIKI)
	include(cmake/HunterGate.cmake)
	HunterGate(
		URL "https://github.com/ruslo/hunter/archive/v0.20.39.tar.gz"
		SHA1 "b49c4b58e17c1473e822f8b21fcde92fee25791d"
	)
endif()

# Declare our project and version
project(DictosCore VERSION 0.1 LANGUAGES C CXX)

# Setup our own platform settings
include(cmake/dictosutil.cmake)
dictosutil_setup_platform()

hunter_add_package(Boost COMPONENTS context system)
find_package(Boost CONFIG REQUIRED context system)

# Our core file list
file(GLOB_RECURSE DictosCoreSrc [LIST_DIRECTORIES false]
	${CMAKE_CURRENT_LIST_DIR}/src/*.cpp
)
file(GLOB_RECURSE DictosCoreInc [LIST_DIRECTORIES false]
	${CMAKE_CURRENT_LIST_DIR}/inc/*.hpp
	${CMAKE_CURRENT_LIST_DIR}/inc/*.h
)

# Declare dictos lib
add_library(
	DictosCore SHARED
	${DictosCoreSrc}
	${DictosCoreInc}
)

# Require c++17
target_compile_features(DictosCore PUBLIC cxx_std_17)

# Enable extended stacktrace info for boost, requires libdl, and pass in the platform specific definitions
target_compile_definitions(DictosCore PUBLIC ${DCORE_COMPILE_DEFINITIONS})
target_compile_definitions(DictosCore PUBLIC -DBOOST_STACKTRACE_USE_BACKTRACE)

# Require clang libs
target_link_libraries(DictosCore PUBLIC
	Boost::context
	Boost::system
)

if (DCORE_PLAT_LIN)
	target_link_libraries(DictosCore PUBLIC
		dl
		backtrace
		pthread
		stdc++fs
	)
endif()

# Define headers for this library. PUBLIC headers are used for
# compiling the library, and will be added to consumers' build
# paths.
target_include_directories(
	DictosCore PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/inc>
)

# Transfer all definitions to this target so all projects inherit them from our exported
# config file
target_compile_definitions(
	DictosCore
  	PUBLIC
	${DCORE_COMPILE_DEFINITIONS}
)

# Setup a useful export file for inclusion in other projects
export(TARGETS DictosCore
	FILE DictosCoreConfig.cmake
	EXPORT_LINK_INTERFACE_LIBRARIES
)

# Include the bin dir for config discovery
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")

# Set install targets @@ TODO
#install(TARGETS DictosCore EXPORT Foundation DESTINATION lib)
#install(DIRECTORY inc/ DESTINATION include FILES_MATCHING PATTERN "*.h*")

# Load tests
enable_testing()
add_subdirectory(tests)
