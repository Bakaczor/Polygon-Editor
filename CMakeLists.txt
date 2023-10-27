cmake_minimum_required(VERSION 3.16)

project(Polygon-Editor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Concurrent)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appPolygon-Editor
    main.cpp
)

qt_add_resources(appPolygon-Editor "Images"
    PREFIX "/"
    FILES
        v.png
        h.png
)

qt_add_qml_module(appPolygon-Editor
    URI Polygon-Editor
    VERSION 1.0
    QML_FILES Main.qml
    SOURCES Edge.h Edge.cpp
    SOURCES Polygon.h Polygon.cpp
    SOURCES SceneManager.cpp SceneManager.h
    SOURCES Vertex.h Vertex.cpp
    SOURCES Functions.h
    SOURCES Functions.cpp
    SOURCES Enums.h
    SOURCES
    SOURCES
    SOURCES
    RESOURCES
    SOURCES Icon.h
    SOURCES Icon.cpp
    SOURCES ImageProvider.h
    SOURCES ImageProvider.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appPolygon-Editor PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appPolygon-Editor
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appPolygon-Editor
    PRIVATE Qt6::Quick
)

target_link_libraries(appPolygon-Editor
    PRIVATE Qt6::Core
)

target_link_libraries(appPolygon-Editor
    PRIVATE Qt6::Concurrent
)

include(GNUInstallDirs)
install(TARGETS appPolygon-Editor
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)



