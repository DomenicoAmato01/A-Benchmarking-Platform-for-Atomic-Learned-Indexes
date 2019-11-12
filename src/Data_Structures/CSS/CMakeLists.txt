cmake_minimum_required(VERSION 3.9)
project(IndexCSS)

set(CMAKE_CXX_STANDARD 14)

add_executable(IndexCSS256 mainCSS256.cpp)
add_executable(IndexCSS8192 mainCSS8192.cpp)

target_include_directories(IndexCSS256 PUBLIC "${CMAKE_SOURCE_DIR}/stx/")
target_include_directories(IndexCSS8192 PUBLIC "${CMAKE_SOURCE_DIR}/stx/")
