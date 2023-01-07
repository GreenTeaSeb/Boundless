cmake_minimum_required(VERSION 3.16)

project(Vast VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 6.2 COMPONENTS Quick Svg Widgets REQUIRED)
find_package(Threads REQUIRED)
find_package(TBB REQUIRED)

qt_add_executable(appVast
    main.cpp
    boardcanvas.hpp boardcanvas.cpp
    path.hpp path.cpp
    draw.cpp
    pan.cpp
    zoom.cpp
)

qt_add_qml_module(appVast
    URI Vast
    VERSION 1.0
    QML_FILES main.qml 
)

qt_add_resources(appVast "icons"
    PREFIX "/"
    FILES
    icons/pen.svg
    icons/eraser.svg
    icons/triangle.svg
    icons/squiggle.svg
    icons/squiggle_thin.svg
)


set_target_properties(appVast PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appVast
    PRIVATE Qt6::Quick Qt6::Svg Qt6::Widgets Threads::Threads TBB::tbb)
target_compile_features(appVast PRIVATE cxx_std_20)

install(TARGETS appVast
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
