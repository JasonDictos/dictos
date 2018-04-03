rm -rf _build 2>/dev/null
mkdir _build 2>/dev/null
cmake -H. -B_build -DCMAKE_BUILD_TYPE=Debug
