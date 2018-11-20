cmake_minimum_required(VERSION 3.5)
project(PS2)

set(CMAKE_CXX_STANDARD 17)

set(BUILD_1 main)
set(SOURCE_FILES_1 Q1.cpp)
add_executable(${BUILD_1} ${SOURCE_FILES_1})

set(BUILD_2 main_2)
set(SOURCE_FILES_2 Q2.cpp)
add_executable(${BUILD_2} ${SOURCE_FILES_2})

set(BUILD_2b main_2b)
set(SOURCE_FILES_2b Q2_Alt.cpp)
add_executable(${BUILD_2b} ${SOURCE_FILES_2b})

set(BUILD_3 main_3)
set(SOURCE_FILES_3 Q3.cpp)
add_executable(${BUILD_3} ${SOURCE_FILES_3})
