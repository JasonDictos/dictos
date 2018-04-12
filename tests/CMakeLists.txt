# Our core file list
file(GLOB_RECURSE CoreTestDeps *.cpp *.h *.hpp)

add_executable(
	CoreTest
	${CoreTestDeps}
)

file(GLOB_RECURSE CoreDeps ../../core/inc/*.h ../../core/inc/*.hpp)

source_group("Core" FILES ${CoreDeps})

# Fetch Catch2 testing framework
hunter_add_package(Catch2)
find_package(Catch2 CONFIG REQUIRED)

# Link to it
target_link_libraries(CoreTest DictosCore Catch2::Catch)

# Register our tests with cmake
add_test(CoreTest CoreTest)

include_directories(
	.
)

# Turn on the latest c++ features
target_compile_features(CoreTest PUBLIC cxx_std_17)

# Precompiled header
set_target_properties(CoreTest PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "tests.hpp")
cotire(CoreTest)
