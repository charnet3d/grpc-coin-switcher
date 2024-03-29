if(DEFINED CMAKE_COINSWITCHER_QT6_MODULE)
else()
set(CMAKE_COINSWITCHER_QT6_MODULE 1)

if(NOT DEFINED QT6_PATH)
    set(QT6_PATH $ENV{QT6_PATH})
    if(NOT IS_DIRECTORY ${QT6_PATH})
    	get_filename_component(QT6_PATH "G:/1_Programs/Qt/6.6.3/msvc2019_64" ABSOLUTE)
    endif()
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets PATHS "${QT6_PATH}" "${QT6_PATH}/lib/cmake" "${QT6_PATH}/lib64/cmake")

qt_standard_project_setup()

# end CMAKE_COINSWITCHER_QT6_MODULE
endif()