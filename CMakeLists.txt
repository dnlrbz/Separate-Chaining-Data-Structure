cmake_minimum_required(VERSION 3.7)
project(ADS)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        ADS_set.h
        btest.cpp
        simpletest.C)

add_executable(ADS ${SOURCE_FILES})