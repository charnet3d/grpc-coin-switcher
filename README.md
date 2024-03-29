Grpc Coin Switcher
===========================

Small utility to control multiple remote PCs that are running mining software. With the config setup correctly, you can switch the coin currently being mined on all the PCs at once. Each PC runs a server that receives the request to switch a coin, then it runs the script to kill the miner and another to run the new miner. Each coin can have its specific script or command line setup in the config file.
There are sample yaml configs for the client and the server with some simple .bat scripts to simulate real ones for testing purposes.

# Pre-requisites

gRPC and Yaml-cpp can be compiled pretty easily using the included scripts `build_grpc.bat` and `build_yamlcpp.bat`. Just make sure to install the pre-requisites for gRPC.

* [gRPC](https://github.com/grpc/grpc/blob/master/BUILDING.md#windows)

* [Yaml-cpp](https://github.com/jbeder/yaml-cpp/tree/master#how-to-build)

* [Qt 6.6.3 - msvc2019_64](https://www.qt.io/download-qt-installer-oss)
Download the installer and make sure to make a custom selection of what to install then select version 6.6.3 and msvc2019_64
In `build.bat` you can set the path like this: `[INSTALL_LOCATION]\Qt\6.6.3\msvc2019_64`

# Building

## Windows

A `build.bat` script has been provided, which sets up Visual Studio environment variables, runs cmake configure, build and install. You just need to setup the location of the libraries being used: gRPC, Yaml-cpp and Qt6.6.3

## Linux, Others ...

Other systems haven't been tested for now, though at least linux should be simple enough to make it work (Qt deployment especially should be checked, as it relies on the Windows specific `windeployqt`)
