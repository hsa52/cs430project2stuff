cmake_minimum_required(VERSION 3.6)
project(cs430proj2things)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c)
add_executable(cs430proj2things ${SOURCE_FILES})