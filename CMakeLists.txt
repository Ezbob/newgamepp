CMAKE_MINIMUM_REQUIRED(VERSION 3.11)

project(test_raylib CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)
find_package(raylib CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)

add_executable(main "")

target_sources(main PRIVATE src/main.cc)
target_link_libraries(main PRIVATE ${raylib_LIBRARIES} EnTT::EnTT)
target_include_directories(main PRIVATE ${raylib_INCLUDE_DIRS})