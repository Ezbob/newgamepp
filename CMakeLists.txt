cmake_minimum_required(VERSION 3.11)

project(test_raylib CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)
find_package(raylib CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)

add_library(raygui INTERFACE)
target_include_directories(raygui
    INTERFACE ${CMAKE_CURRENT_LIST_DIR}/libs/raygui/src/
)

add_executable(main "")
target_sources(main
    PRIVATE
        src/main.cc
        src/gui/fields/SliderField.cc
        src/gui/fields/TextField.cc
        src/gui/DebugGUI.cc
        src/gui/EntityWindow.cc
)
target_link_libraries(main
    PRIVATE
        EnTT::EnTT
        ${RAYLIB_LIBRARIES}
        ${CMAKE_DL_LIBS}
        ${CMAKE_THREAD_LIBS_INIT}
        raygui
)
target_include_directories(main
    PRIVATE
        ${RAYLIB_LIBRARIES}
        src/
)
