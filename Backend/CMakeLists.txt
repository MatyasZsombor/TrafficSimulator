cmake_minimum_required(VERSION 4.1)
project(Backend)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(core)

add_executable(Backend main.cpp)

target_link_libraries(Backend PRIVATE core)
