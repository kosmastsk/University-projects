cmake_minimum_required(VERSION 3.6)
project(01parallila)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    data_rearrangement.c
    datasets.c
    general_functions.c
    hash_codes.c
    morton_encoding.c
    radix_sort.c
    test_octree.c
    utils.h
    verification.c)

add_executable(01parallila ${SOURCE_FILES})