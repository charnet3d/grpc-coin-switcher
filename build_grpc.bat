@echo off

cd /d %~dp0

md cmake & cd cmake
md install
md build & cd build

call "%VS140COMNTOOLS%..\..\VC\vcvarsall.bat" x64

cmake ..\.. ^
	-GNinja ^
	-DCMAKE_BUILD_TYPE=Release ^
	-DgRPC_INSTALL=ON ^
	-DgRPC_BUILD_TESTS=OFF ^
	-DCMAKE_INSTALL_PREFIX=..\install

@rem To speedup the compile set the numbers of cores or threads in your CPU with: -j XX
cmake --build .

cmake --install .

cd ..\..
pause