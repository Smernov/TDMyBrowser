cmake_minimum_required(VERSION 3.21)

project(MyCoolBrowser VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    WebEngineWidgets
)

add_executable(MyCoolBrowser
    main.cpp
)

target_link_libraries(MyCoolBrowser PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::WebEngineWidgets
)

if(WIN32)
    set_target_properties(MyCoolBrowser PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()
