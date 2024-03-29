@echo off
cd /d %~dp0

@rem set your own library paths
set GRPC_PATH=..\grpc\cmake\install
set YAMLCPP_PATH=..\yaml-cpp\cmake\install
set QT6_PATH=G:\1_Programs\Qt\6.6.3\msvc2019_64

@rem Debug or Release
set BUILD_TYPE=Release

@rem install location
set INSTALL_PREFIX=..\install

md cmake & cd cmake
md install
md build & cd build

call "%VS140COMNTOOLS%..\..\VC\vcvarsall.bat" x64

cmake ..\.. ^
	-GNinja ^
	-DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
	-DGRPC_PATH=%GRPC_PATH% ^
	-DYAMLCPP_PATH=%YAMLCPP_PATH% ^
	-DQT6_PATH=%QT6_PATH% ^
	-DCMAKE_INSTALL_PREFIX=%INSTALL_PREFIX%

@rem To speedup the compile set the numbers of cores or threads in your CPU with: -j XX
cmake --build .

cmake --install .

cd ..\..
pause