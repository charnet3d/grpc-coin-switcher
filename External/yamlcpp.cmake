if(DEFINED CMAKE_COINSWITCHER_YAMLCPP_MODULE)
else()
set(CMAKE_COINSWITCHER_YAMLCPP_MODULE 1)

if(NOT DEFINED YAMLCPP_PATH)
    set(YAMLCPP_PATH $ENV{YAMLCPP_PATH})
    if(NOT IS_DIRECTORY ${YAMLCPP_PATH})
		if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
			get_filename_component(YAMLCPP_PATH "${PROJECT_SOURCE_DIR}/../yaml-cpp/cmake/install_debug" ABSOLUTE)
		else()
			get_filename_component(YAMLCPP_PATH "${PROJECT_SOURCE_DIR}/../yaml-cpp/cmake/install" ABSOLUTE)
		endif()
	endif()
endif()

find_package(yaml-cpp REQUIRED PATHS "${YAMLCPP_PATH}" "${YAMLCPP_PATH}/lib/cmake" "${YAMLCPP_PATH}/lib64/cmake")

# end CMAKE_COINSWITCHER_YAMLCPP_MODULE
endif()