setlocal && exit /b
git submodule update --init net
rm -rf _build 2>/dev/null
mkdir _build 2>/dev/null
rm inc/dictos/core
ln -s ../ inc/dictos/core
rm net/inc/dictos/net
ln -s ../ net/inc/dictos/net
cmake -H. -B_build -DCMAKE_BUILD_TYPE=Debug
