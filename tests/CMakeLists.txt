# Our core file list
file(GLOB_RECURSE CoreTestDeps *.cpp *.h *.hpp)

add_executable(
	CoreTest
	${CoreTestDeps}
)

file(GLOB_RECURSE CoreDeps ../../core/inc/*.h ../../core/inc/*.hpp)

source_group("Core" FILES ${CoreDeps})

# Link to Catch2
hunter_add_package(Catch)
find_package(Catch CONFIG REQUIRED)

target_link_libraries(CoreTest DictosCore Catch::Catch)

add_test(CoreTest CoreTest)

include_directories(
	.
)

# Build a solution tree
#source_group(TREE src FILES ${DictosCoreSrc})
#source_group(TREE inc FILES ${DictosCoreInc})

target_compile_features(CoreTest PUBLIC cxx_std_17)

# Precompiled header
set_target_properties(CoreTest PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "tests.hpp")
cotire(CoreTest)
