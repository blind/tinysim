cmake_minimum_required(VERSION 3.4)
project(tinysim)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

add_subdirectory(src)
add_subdirectory(examples/tinytest)
