cmake_minimum_required(VERSION 3.11)

project(test_raylib CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)
find_package(raylib CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

set(BUILD_RAYGUI_EXAMPLES OFF CACHE BOOL "Building raylib GUI examples")
add_subdirectory(libs/raygui/projects/CMake/ EXCLUDE_FROM_ALL)

add_executable(main "" src/Constants.hh)
target_sources(main
    PRIVATE
        src/Main.cc
        src/gui/DebugGUI.cc
        src/gui/EntityWindow.cc
        src/gui/DebugMenu.cc
)
target_link_libraries(main
    PRIVATE
        EnTT::EnTT
        ${RAYLIB_LIBRARIES}
        ${CMAKE_DL_LIBS}
        ${CMAKE_THREAD_LIBS_INIT}
        raygui
        fmt::fmt
)
target_include_directories(main
    PRIVATE
        ${RAYLIB_LIBRARIES}
        src/
)
