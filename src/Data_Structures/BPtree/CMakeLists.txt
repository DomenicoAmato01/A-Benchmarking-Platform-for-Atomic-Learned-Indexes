cmake_minimum_required(VERSION 3.9)
project(IndexBtree)

set(CMAKE_CXX_STANDARD 14)

add_executable(IndexBtree256 mainBtree256.cpp)
add_executable(IndexBtree8192 mainBtree8192.cpp)

target_include_directories(IndexBtree256 PUBLIC "${CMAKE_SOURCE_DIR}/stx/")
target_include_directories(IndexBtree8192 PUBLIC "${CMAKE_SOURCE_DIR}/stx/")
