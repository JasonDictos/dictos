del /s /q _build
rmdir /s /q _build
mkdir _build

rmdir inc\dictos\core
mkdir inc\dictos
mklink /J inc\dictos\core inc

rmdir net\inc\dictos\net
mkdir net\inc\dictos
mklink /J net\inc\dictos\net net\inc

cmake -H. -B_build -G"Visual Studio 15 2017 Win64"
