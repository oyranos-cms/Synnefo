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

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH
  "Installation directory for header files")
if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/CMake/synnefo)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH
  "Installation directory for CMake files")

# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

# set up include-directories
include_directories(
  "${PROJECT_SOURCE_DIR}"   # to find foo/foo.h
  "${PROJECT_BINARY_DIR}")  # to find foo/config.h

add_subdirectory(src)

export(TARGETS synnefo
  FILE "${PROJECT_BINARY_DIR}/SynnefoLibraryDepends.cmake")

export(PACKAGE synnefo)

configure_file(SynnefoBuildTreeSettings.cmake.in
  "${PROJECT_BINARY_DIR}/SynnefoTreeSettings.cmake" @ONLY)

file(RELATIVE_PATH CONF_REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
   "${INSTALL_INCLUDE_DIR}")
configure_file(SynnefoConfig.cmake.in
   "${PROJECT_BINARY_DIR}/SynnefoConfig.cmake" @ONLY)
configure_file(SynnefoConfigVersion.cmake.in
   "${PROJECT_BINARY_DIR}/SynnefoConfigVersion.cmake" @ONLY)

install(EXPORT SynnefoLibraryDepends DESTINATION
   "${INSTALL_DATA_DIR}/Synnefo/CMake"
   COMPONENT dev)

########### Uninstall the targets ###############
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake"
  @ONLY)
ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake"
) 
