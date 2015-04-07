PROJECT(Synnefo-Oyranos)
SET( Synnefo-Oyranos_VERSION ${synnefo_VERSION} )

cmake_minimum_required(VERSION 2.4)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )

# Qt5Widgets_FOUND comes from libsynnefo
IF(NOT Qt5Widgets_FOUND)
  find_package(Qt4 REQUIRED)
  INCLUDE(   ${QT_USE_FILE}   )
ENDIF()

# test standalone
IF (NOT OYRANOS_FOUND)
  find_package(Synnefo REQUIRED)
  include_directories(${SYNNEFO_INCLUDE_DIR})
ENDIF (NOT OYRANOS_FOUND)

# rely on library in tar ball
IF    (NOT SYNNEFO_FOUND)
  SET( SYNNEFO_LIBRARY synnefo-common )
# set up include-directories
  set( synnefo_modules_path
       ../modules )
  include_directories(
  "${PROJECT_SOURCE_DIR}"   # to find foo/foo.h
  "${PROJECT_BINARY_DIR}"   # to find foo/config.h
  "${synnefo_modules_path}/"# modules
  "${synnefo_modules_path}/database/"
  "${synnefo_modules_path}/devices/"
  "${synnefo_modules_path}/information/"
  "${synnefo_modules_path}/settings/"
  "../classes"
                     )
  LINK_DIRECTORIES( "${CMAKE_CURRENT_SOURCE_DIR}/../../build/src/" )
ENDIF (NOT SYNNEFO_FOUND)

set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")

include_directories(
  "${PROJECT_SOURCE_DIR}"   # to find foo/foo.h
  "${PROJECT_BINARY_DIR}")  # to find foo/config.h

set( synnefo_SRCS
    # core sources
    main.cpp
    synnefo.cpp
)
 
set( synnefo_MOC_HEADERS
    # main window
    synnefo.h
)

set( synnefo_UIs
    # main ui file
    ui/synnefo.ui
)

set(synnefo_RCCS
      ui/resources.qrc
      )

IF(QT4_FOUND)
  QT4_ADD_RESOURCES(synnefo_RCC_SRCS ${synnefo_RCCS})
  QT4_WRAP_UI (synnefo_SRCS ${synnefo_UIs})
  QT4_WRAP_CPP(synnefo_SRCS ${synnefo_MOC_HEADERS})
ENDIF()
IF(Qt5Widgets_FOUND)
  QT5_ADD_RESOURCES(synnefo_RCC_SRCS ${synnefo_RCCS})
  SET(CMAKE_AUTOMOC ON)
  QT5_WRAP_UI(synnefo_SRCS ${synnefo_UIs})
ENDIF()



IF(APPLE)
  # icon files to copy in the bundle
  set( OSX_ICON_FILES ${PROJECT_SOURCE_DIR}/extras/osx/synnefo.icns  )
  set( OSX_SCRIPT_FILES "${PROJECT_SOURCE_DIR}/extras/osx/Synnefo.sh" )
  set( OSX_PKG_FILES ${PROJECT_SOURCE_DIR}/extras/osx/PkgInfo )

  add_executable( Synnefo MACOSX_BUNDLE ${synnefo_SRCS} ${synnefo_RCC_SRCS} ${OSX_ICON_FILES} )

  INSTALL( TARGETS Synnefo
  EXPORT SynnefoLibraryDepends
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  BUNDLE DESTINATION  "${INSTALL_BIN_DIR}" COMPONENT bin)

  set_target_properties( Synnefo PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME Synnefo )
  set_target_properties( Synnefo PROPERTIES MACOSX_BUNDLE_INFO_STRING "Synnefo front end to Oyranos Color Management System, www.oyranos.org" )
  set_target_properties( Synnefo PROPERTIES MACOSX_BUNDLE_EXECUTABLE_NAME Synnefo.sh )
  set_target_properties( Synnefo PROPERTIES MACOSX_BUNDLE_ICON_FILE synnefo.icns )
  set_target_properties( Synnefo PROPERTIES MACOSX_BUNDLE_GUI_IDENTIFIER org.oyranos.synnefo )
  set_target_properties( Synnefo PROPERTIES MACOSX_BUNDLE_LONG_VERSION_STRING "Synnefo ${synnefo_VERSION}" )
  set_target_properties( Synnefo PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME Synnefo )
  set_target_properties( Synnefo PROPERTIES MACOSX_BUNDLE_SHORT_VERSION_STRING ${synnefo_VERSION} )
  set_target_properties( Synnefo PROPERTIES MACOSX_BUNDLE_BUNDLE_VERSION 1.0 )
  set_target_properties( Synnefo PROPERTIES MACOSX_BUNDLE_COPYRIGHT "2011-2013 Joseph Simon, 2012-2015 Kai-Uwe Behrmann" )

  # configure CMake to use a custom Info.plist
  #set_target_properties( Synnefo PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/extras/osx/Info.plist )

  # set where in the bundle to put the icns files
  set_source_files_properties( ${OSX_ICON_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

# Now the work of copying dependencies into the bundle/package
# The quotes are escaped and variables to use at install time have their $ escaped
# An alternative is the do a configure_file() on a script and use install(SCRIPT  ...).
# Note that the image plugins depend on QtSvg and QtXml, and it got those copied
# over.
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/Synnefo.app")  # paths to executables
  SET(DIRS "\${CMAKE_INSTALL_PREFIX}/lib \${CMAKE_CURRENT_BINARY_DIR}")   # directories to search for prerequisites
  SET(DIRS "\${CMAKE_INSTALL_PREFIX}/lib")   # directories to search for prerequisites
  INSTALL(CODE "
   include(BundleUtilities)
   fixup_bundle(\"${APPS}\"   \"\"   \"${DIRS}\")
   " COMPONENT Runtime)

  INSTALL( FILES "${OSX_SCRIPT_FILES}" DESTINATION "bin/Synnefo.app/Contents/MacOS" COMPONENT bin )
  INSTALL( FILES "${OSX_PKG_FILES}" DESTINATION "bin/Synnefo.app/Contents" COMPONENT bin )

  #INSTALL( TARGETS Synnefo
  #EXPORT SynnefoLibraryDepends
  #RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  #BUNDLE DESTINATION  "${INSTALL_BIN_DIR}" COMPONENT bin )

  #add_executable(Synnefo ${synnefo_SRCS} ${synnefo_RCC_SRCS})
ELSEIF(UNIX)
  add_executable(Synnefo ${synnefo_SRCS} ${synnefo_RCC_SRCS})
ELSEIF (WIN32)
  add_executable(Synnefo WIN32 ${synnefo_SRCS} ${synnefo_RCC_SRCS})
ENDIF ()

export(TARGETS Synnefo
  FILE "${PROJECT_BINARY_DIR}/SynnefoLibraryDepends.cmake")

export(PACKAGE Synnefo)


target_link_libraries(Synnefo ${SYNNEFO_LIBRARY} ${QT_LIBRARIES})

IF( NOT APPLE )
  INSTALL( TARGETS Synnefo
  EXPORT SynnefoLibraryDepends
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
ENDIF()
