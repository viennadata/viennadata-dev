
PROJECT(viennadata)

cmake_minimum_required(VERSION 2.4)

#if you don't want the full compiler output, remove the following line
SET(CMAKE_VERBOSE_MAKEFILE ON)

#specify include and source directory
INCLUDE_DIRECTORIES(".")


#add definitions, compiler switches, etc.
IF(DEFINED CMAKE_BUILD_TYPE)
 SET (CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE})
ELSE()
 SET (CMAKE_BUILD_TYPE Release)
ENDIF()

IF(CMAKE_COMPILER_IS_GNUCXX)
   #ADD_DEFINITIONS(-Wall -O0 -g)
   ADD_DEFINITIONS(-Wall -pedantic -O3)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

ADD_EXECUTABLE(oopbench            examples/benchmarks/oop.cpp)
ADD_EXECUTABLE(densebench          examples/benchmarks/dense.cpp)
ADD_EXECUTABLE(sparseaddressbench  examples/benchmarks/sparse-address.cpp)
ADD_EXECUTABLE(sparsecustombench   examples/benchmarks/sparse-custom.cpp)


