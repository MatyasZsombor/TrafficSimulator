cmake_minimum_required(VERSION 4.1)
project(core)

set(CMAKE_CXX_STANDARD 20)

add_library(core STATIC library.cpp
        Road.cpp
        Road.h
        Network.cpp
        Network.h)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})