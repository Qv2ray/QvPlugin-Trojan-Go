project(QvPlugin-TrojanGo)
cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(interface/QvPluginInterface.cmake)
include(interface/QvGUIPluginInterface.cmake)
include_directories(${QVPLUGIN_INTERFACE_INCLUDE_DIR})

add_library(${PROJECT_NAME} MODULE
    TrojanGoPlugin.hpp
    TrojanGoPlugin.cpp
    core/Common.hpp
    core/EventHandler.cpp
    core/EventHandler.hpp
    core/Serializer.cpp
    core/Serializer.hpp
    core/Kernel.cpp
    core/Kernel.hpp
    widgets/TrojanGoSettingsWidget.cpp
    widgets/TrojanGoSettingsWidget.hpp
    widgets/TrojanGoSettingsWidget.ui
    widgets/TrojanGoOutboundWidget.cpp
    widgets/TrojanGoOutboundWidget.hpp
    widgets/TrojanGoOutboundWidget.ui
    ui/GUIInterface.hpp
    resx.qrc
    ${QVPLUGIN_INTERFACE_HEADERS}
    ${QVGUIPLUGIN_INTERFACE_HEADERS})
    
if(APPLE)
    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_INSTALL_NAME_TOOL} -add_rpath "@executable_path/../Frameworks/" $<TARGET_FILE:${PROJECT_NAME}>)
endif()

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets)
