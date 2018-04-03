#pragma once

namespace dictos::file {

// Work around the experimental nature of msvc's implementation of filesystem
#if defined(DCORE_PLAT_WINDOWS)
	using namespace std::experimental::filesystem::v1;
#else
	using namespace std::experimental::filesystem;
#endif


}
