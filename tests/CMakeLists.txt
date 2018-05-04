# Our core file list
file(GLOB_RECURSE DictosCoreTestDeps *.cpp *.h *.hpp)

add_executable(
	DictosCoreTest
	${DictosCoreTestDeps}
)

# Fetch Catch2 testing framework
hunter_add_package(Catch)
find_package(Catch2 CONFIG REQUIRED)

# Link to it
target_link_libraries(DictosCoreTest DictosCore Catch2::Catch)

# Register our tests with cmake
add_test(DictosCoreTest DictosCoreTest)

target_include_directories(
	DictosCoreTest
	PRIVATE
	${CMAKE_CURRENT_LIST_DIR}
)

# Turn on the latest c++ features
target_compile_features(DictosCoreTest PUBLIC cxx_std_17)

# Precompiled header
set_target_properties(DictosCoreTest PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "tests.hpp")
cotire(DictosCoreTest)
