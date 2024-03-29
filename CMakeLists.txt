# CMakeList.txt : CMake project for GrpcCoinSwitcher, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.16)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("GrpcCoinSwitcher" VERSION 1.0.0 LANGUAGES CXX)

include(${PROJECT_SOURCE_DIR}/External/grpc.cmake)
include(${PROJECT_SOURCE_DIR}/External/yamlcpp.cmake)
include(${PROJECT_SOURCE_DIR}/External/qt6.cmake)

add_subdirectory(Proto)
add_subdirectory(Client)
add_subdirectory(Server)
