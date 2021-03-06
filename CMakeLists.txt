cmake_minimum_required(VERSION 3.16)

project(test_raylib CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

find_package(Threads REQUIRED)
find_package(raylib CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_subdirectory(libs/raygui/projects/CMake/ EXCLUDE_FROM_ALL)
add_subdirectory(libs/nativefiledialog/ EXCLUDE_FROM_ALL)
add_subdirectory(libs/cereal-1.3.1/ EXCLUDE_FROM_ALL)

add_executable(main "" src/Constants.hh)
target_sources(main
    PRIVATE
        src/Main.cc
        src/gui/GuiUnity.cc
)

target_link_libraries(main
    PRIVATE
        EnTT::EnTT
        ${raylib_LIBRARIES}
        ${CMAKE_DL_LIBS}
        ${CMAKE_THREAD_LIBS_INIT}
        raygui
        fmt::fmt
        nativefiledialog_static
        nlohmann_json
        nlohmann_json::nlohmann_json
        cereal::cereal
)
target_include_directories(main
    PRIVATE
        ${raylib_INCLUDE_DIRS}
        src/
)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" AND NOT WIN32)
set_target_properties(main
    PROPERTIES
        COMPILE_FLAGS "-fsanitize=address,undefined"
        LINK_FLAGS "-fsanitize=address,undefined"
)
elseif(WIN32)
target_compile_definitions(main
    PRIVATE _CRT_SECURE_NO_WARNINGS=1
)
endif()
