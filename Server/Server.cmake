if(DEFINED CMAKE_COINSWITCHER_SERVER_MODULE)
else()
set(CMAKE_COINSWITCHER_SERVER_MODULE 1)

include_directories(
    ${PROJECT_SOURCE_DIR}/Server
)

endif(DEFINED CMAKE_COINSWITCHER_SERVER_MODULE)