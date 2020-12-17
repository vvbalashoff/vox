cmake_minimum_required(VERSION 3.2)

project(simplex)

#set(LIB_TYPE SHARED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(
    SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/simplexnoise.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/simplexnoise.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/simplextextures.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/simplextextures.cpp"
   )

add_library(simplex ${SOURCES})
