rm -rf _build
mkdir _build
cmake -H. -B_build -G"Visual Studio 15 2017 Win64" -DBRTBUILD_WINDOWS:STATIC:=1
