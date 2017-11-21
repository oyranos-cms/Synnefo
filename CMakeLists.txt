#cmake file for building Synnefo and related modules
 
PROJECT(synnefo)

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules )

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

# project version
SET( ${PROJECT_NAME}_MAJOR_VERSION 1)
SET( ${PROJECT_NAME}_MINOR_VERSION 1)
SET( ${PROJECT_NAME}_MICRO_VERSION 1)
SET( ${PROJECT_NAME}_VERSION ${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_MICRO_VERSION} )

INCLUDE(GNUInstallDirs)

IF( UNIX AND NOT APPLE AND NOT ${CMAKE_SYSTEM_NAME} MATCHES Android )
  OPTION(ENABLE_QT_DBUS "Build Qt5 with DBus support" ON)
ELSE()
  OPTION(ENABLE_QT_DBUS "Build Qt5 with DBus support" OFF)
ENDIF()

IF( ${CMAKE_SYSTEM_NAME} MATCHES Android )
  OPTION(ENABLE_APP "Build stand alone App" OFF)
ELSE()
  OPTION(ENABLE_APP "Build stand alone App" ON)
ENDIF()
OPTION(ENABLE_SHARED_LIBS "Build dynamical linked library" ON)
OPTION(ENABLE_STATIC_LIBS "Build statical linked library" ON)

IF(NOT USE_Qt4)
  FIND_PACKAGE(Qt5 CONFIG COMPONENTS Widgets)
  IF(ENABLE_QT_DBUS)
    FIND_PACKAGE(Qt5 CONFIG COMPONENTS DBus)
    SET(HAVE_QT_DBUS TRUE)
  ENDIF(ENABLE_QT_DBUS)
ENDIF()
IF(NOT Qt5Widgets_FOUND)
  MESSAGE( "Fallback: Qt5 not found - trying Qt4" )
  FIND_PACKAGE(Qt4 CONFIG REQUIRED COMPONENTS Widgets DBus)
ENDIF()

FIND_PACKAGE(Oyranos REQUIRED)
INCLUDE_DIRECTORIES( ${INCLUDE_DIRECTORIES} ${OYRANOS_INCLUDE_DIRS} )
LINK_DIRECTORIES( ${LINK_DIRECTORIES} ${OYRANOS_LIBRARY_DIRS} )

SET( ${CMAKE_BUILD_TYPE} Debug)

# Offer the user the choice of overriding the installation directories
SET(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
SET(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
SET(INSTALL_INCLUDE_DIR include CACHE PATH
  "Installation directory for header files")
IF(WIN32 AND NOT CYGWIN)
  SET(DEF_INSTALL_CMAKE_DIR CMake)
ELSE()
  SET(DEF_INSTALL_CMAKE_DIR lib/CMake/synnefo)
ENDIF()
SET(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH
  "Installation directory for CMake files")

# Make relative paths absolute (needed later on)
FOREACH(p LIB BIN INCLUDE CMAKE)
  SET(var INSTALL_${p}_DIR)
  IF(NOT IS_ABSOLUTE "${${var}}")
    SET(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  ENDIF()
ENDFOREACH()

# set up include-directories
INCLUDE_DIRECTORIES(
  "${PROJECT_SOURCE_DIR}"   # to find foo/foo.h
  "${PROJECT_BINARY_DIR}")  # to find foo/config.h

# if our static code lands inside a shared lib, we need -fPIC
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(cmake)

FILE(RELATIVE_PATH CONF_REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
   "${INSTALL_INCLUDE_DIR}")

#install(EXPORT SynnefoLibraryDepends DESTINATION
#   "${INSTALL_DATA_DIR}/Synnefo/CMake"
#   COMPONENT dev)

########### Uninstall the targets ###############
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake"
  @ONLY)
ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake"
) 

ADD_CUSTOM_TARGET( dist
                COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/ && git archive --prefix=${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}/ --format=tar HEAD > ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}.tar
                COMMAND echo "git:       git clone git://gitorious.org/~beku/synnefo/bekus-synnefo.git" > ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}.tar.bz2.hashsum
                COMMAND echo "git sha1:  " | tr -d "\\n" >> ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}.tar.bz2.hashsum
                COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/ && git rev-parse HEAD >> ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}.tar.bz2.hashsum
                COMMAND rm -rf ${PROJECT_NAME}-${${PROJECT_NAME}_VERSION} && tar xf ${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}.tar
                COMMAND tar cf - ${PROJECT_NAME}-${${PROJECT_NAME}_VERSION} | bzip2 > ${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}.tar.bz2
                COMMAND echo  "size:      " | tr -d "\\n" >> ${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}.tar.bz2.hashsum
                COMMAND du -b ${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}.tar.bz2 | sed s/[[:space:]][[:print:]]*// >> ${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}.tar.bz2.hashsum
                COMMAND echo  "md5sum:    " | tr -d "\\n" >> ${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}.tar.bz2.hashsum
                COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/ && md5sum ${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}.tar.bz2 >> ${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}.tar.bz2.hashsum
                COMMAND echo  "sha1sum:   " | tr -d "\\n" >> ${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}.tar.bz2.hashsum
                COMMAND sha1sum ${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}.tar.bz2 >> ${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}.tar.bz2.hashsum
                COMMAND echo  "sha256sum: " | tr -d "\\n" >> ${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}.tar.bz2.hashsum
                COMMAND sha256sum ${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}.tar.bz2 >> ${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}.tar.bz2.hashsum
                COMMENT Make a ${PROJECT_NAME} source package by using git
                VERBATIM
                WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
                )
 
ADD_CUSTOM_TARGET( debsrc
                COMMAND test -d ${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}/ && rm -rf ${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}/
                COMMAND tar xjf ${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}.tar.bz2
                COMMAND mv ${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}.tar.bz2 ${PROJECT_NAME}_${${PROJECT_NAME}_VERSION}.orig.tar.bz2
                COMMAND cp -R ${CMAKE_SOURCE_DIR}/extras/debian ${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}/
                COMMAND dpkg-source -b ${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}/
                DEPENDS dist
                COMMENT Make a ${PROJECT_NAME} DEB source package
                VERBATIM
                )
