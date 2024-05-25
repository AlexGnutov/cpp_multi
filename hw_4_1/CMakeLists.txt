cmake_minimum_required(VERSION 3.5)

project(hw_4_1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS shape.h transform.h)
set(SOURCES shape.cpp transform.cpp)

add_executable(hw_4_1 main.cpp ${HEADERS} ${SOURCES})
