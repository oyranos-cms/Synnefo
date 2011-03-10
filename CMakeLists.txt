 #cmake file for building Synnefo and related modules
 
PROJECT(synnefo)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules )

cmake_minimum_required(VERSION 2.4)

# project version
SET( ${PROJECT_NAME}_MAJOR_VERSION 0)
SET( ${PROJECT_NAME}_MINOR_VERSION 1)

find_package(Qt4 REQUIRED)
find_package(Oyranos REQUIRED)

include_directories(${OYRANOS_INCLUDE_DIR} ${OYRANOS_INCLUDE_DIR}/alpha/object)

SET( ${CMAKE_BUILD_TYPE} Debug)

SUBDIRS(src modules)
 

 
