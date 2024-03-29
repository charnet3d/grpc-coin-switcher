
include(${CMAKE_CURRENT_SOURCE_DIR}/Client.cmake)

include(${PROJECT_SOURCE_DIR}/Common/Common.cmake)
include(${PROJECT_SOURCE_DIR}/Proto/Proto.cmake)

set(CMAKE_INCLUDE_CURRENT_DIR YES)

set(CLIENT_SRC
    MinerClient.cpp
    main.cpp
)
set(CLIENT_HDR
    MinerClient.h
    main.h
)

qt_add_executable(GrpcCoinSwitcher_Client
    ${CLIENT_SRC}
)
#add_executable (GrpcCoinSwitcher_Client ${CLIENT_SRC} ${CLIENT_HDR})

add_dependencies(GrpcCoinSwitcher_Client Proto)
target_link_libraries(GrpcCoinSwitcher_Client PRIVATE Proto)

target_link_libraries(GrpcCoinSwitcher_Client PRIVATE ${_REFLECTION})
target_link_libraries(GrpcCoinSwitcher_Client PRIVATE ${_GRPC_GRPCPP})
target_link_libraries(GrpcCoinSwitcher_Client PRIVATE ${_PROTOBUF_LIBPROTOBUF})
target_link_libraries(GrpcCoinSwitcher_Client PRIVATE yaml-cpp::yaml-cpp)
target_link_libraries(GrpcCoinSwitcher_Client PRIVATE Qt6::Core)
target_link_libraries(GrpcCoinSwitcher_Client PRIVATE Qt6::Gui)
target_link_libraries(GrpcCoinSwitcher_Client PRIVATE Qt6::Widgets)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET GrpcCoinSwitcher_Client PROPERTY CXX_STANDARD 20)
endif()

# Whether to show the console or not
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set_target_properties(GrpcCoinSwitcher_Client PROPERTIES WIN32_EXECUTABLE OFF)
else()
    set_target_properties(GrpcCoinSwitcher_Client PROPERTIES WIN32_EXECUTABLE ON)
endif()

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/client_config.yaml" DESTINATION ${CMAKE_INSTALL_PREFIX})

add_custom_command(TARGET GrpcCoinSwitcher_Client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/client_config.yaml" "$<TARGET_FILE_DIR:GrpcCoinSwitcher_Client>/client_config.yaml"
)

install(TARGETS GrpcCoinSwitcher_Client
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
)

# One way to deploy qt files, not used for now as it forces a specific folder structure, with binaries in bin subdir
#if(CMAKE_CROSSCOMPILING)
#    qt_generate_deploy_app_script(TARGET GrpcCoinSwitcher_Client OUTPUT_SCRIPT deploy_script NO_UNSUPPORTED_PLATFORM_ERROR)
#else()
#    qt_generate_deploy_app_script(TARGET GrpcCoinSwitcher_Client OUTPUT_SCRIPT deploy_script)
#endif()
#install(SCRIPT ${deploy_script})

# With this method we just copy the files generated during build in the windeployqt subdir
install(
    DIRECTORY
    "${PROJECT_BINARY_DIR}/windeployqt/"
    DESTINATION ${CMAKE_INSTALL_PREFIX}
) 

# Split windeployqt into 2 parts to fix issue with deploying debug plugins
add_custom_command(TARGET GrpcCoinSwitcher_Client POST_BUILD
    COMMAND "${QT6_PATH}/bin/windeployqt" --dir "${PROJECT_BINARY_DIR}/windeployqt" --compiler-runtime --no-plugins --no-translations --no-quick-import --no-system-d3d-compiler --no-opengl-sw $<TARGET_FILE:GrpcCoinSwitcher_Client>)
if (CMAKE_GENERATOR STREQUAL "Ninja")
    # Ninja is a single-config generator so we can use CMAKE_BUILD_TYPE to generate different commands
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_custom_command(TARGET GrpcCoinSwitcher_Client POST_BUILD
            COMMAND "${QT6_PATH}/bin/windeployqt" --dir "${PROJECT_BINARY_DIR}/windeployqt" --debug --no-compiler-runtime --no-translations --no-libraries --no-quick-import --no-system-d3d-compiler --no-opengl-sw $<TARGET_FILE:GrpcCoinSwitcher_Client>)
    else()
        add_custom_command(TARGET GrpcCoinSwitcher_Client POST_BUILD
            COMMAND "${QT6_PATH}/bin/windeployqt" --dir "${PROJECT_BINARY_DIR}/windeployqt" --release --no-compiler-runtime --no-translations --no-libraries --no-quick-import --no-system-d3d-compiler --no-opengl-sw $<TARGET_FILE:GrpcCoinSwitcher_Client>)
    endif()
else()
    # if in MSVC we have to check the configuration at runtime instead of generating different commands
    add_custom_command(TARGET GrpcCoinSwitcher_Client POST_BUILD
        COMMAND cmd.exe /c if "$(Configuration)" == "Debug" "${QT6_PATH}/bin/windeployqt" --dir "${PROJECT_BINARY_DIR}/windeployqt" --debug --no-compiler-runtime --no-translations --no-libraries --no-quick-import --no-system-d3d-compiler --no-opengl-sw $<TARGET_FILE:GrpcCoinSwitcher_Client>)
    add_custom_command(TARGET GrpcCoinSwitcher_Client POST_BUILD
        COMMAND cmd.exe /c if not "$(Configuration)" == "Debug" "${QT6_PATH}/bin/windeployqt" --dir "${PROJECT_BINARY_DIR}/windeployqt" --release --no-compiler-runtime --no-translations --no-libraries --no-quick-import --no-system-d3d-compiler --no-opengl-sw $<TARGET_FILE:GrpcCoinSwitcher_Client>)
endif()

# Copy the files in the root folder so running/debugging in IDE works
add_custom_command(TARGET GrpcCoinSwitcher_Client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_BINARY_DIR}/windeployqt" "${PROJECT_BINARY_DIR}")