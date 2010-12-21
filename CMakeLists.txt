
PROJECT(viennadata)

cmake_minimum_required(VERSION 2.4)

#if you don't want the full compiler output, remove the following line
SET(CMAKE_VERBOSE_MAKEFILE ON)

#specify include and source directory
INCLUDE_DIRECTORIES(".")

# find needed packages
FIND_PACKAGE(Boost REQUIRED COMPONENTS unit_test_framework)

aux_source_directory("./tests" TOTAL_SRC)

#add definitions, compiler switches, etc.
IF(DEFINED CMAKE_BUILD_TYPE)
 SET (CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE})
ELSE()
 SET (CMAKE_BUILD_TYPE Release)
ENDIF()

IF(CMAKE_COMPILER_IS_GNUCXX)
   ADD_DEFINITIONS(-Wall -O0 -g)
   #ADD_DEFINITIONS(-Wall -pedantic -O3)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

ADD_EXECUTABLE(test ${TOTAL_SRC} testviennadata.cpp)

TARGET_LINK_LIBRARIES(test ${Boost_LIBRARIES})

