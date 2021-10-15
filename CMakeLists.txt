cmake_minimum_required(VERSION 3.11)

project(test_raylib CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)
find_package(raylib CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)

add_executable(main "")

if(WIN32)
    set(real_raylib_lib ${raylib_LIBRARIES})
    set(real_raylib_include ${raylib_INCLUDE_DIRS})
else()
    set(real_raylib_lib ${RAYLIB_LIBRARIES})
    set(real_raylib_include ${RAYLIB_INCLUDE_DIRS})
endif()


target_sources(main PRIVATE src/main.cc)
target_link_libraries(main PRIVATE ${real_raylib_lib} EnTT::EnTT ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(main PRIVATE ${real_raylib_include})
