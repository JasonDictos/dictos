rm -rf _build
mkdir _build
cmake -H. -B_build -G"Visual Studio 15 2017 Win64" -DHUNTER_STATUS_DEBUG=ON
