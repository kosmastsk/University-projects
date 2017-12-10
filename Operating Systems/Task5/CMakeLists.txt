cmake_minimum_required(VERSION 3.6)
project(task5)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        dining.c
        task5.txt)

add_executable(task5 ${SOURCE_FILES})