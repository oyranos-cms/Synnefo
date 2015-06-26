PROJECT(Synnefo-Oyranos)
SET( Synnefo-Oyranos_VERSION ${synnefo_VERSION} )

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )

# Qt5Widgets_FOUND comes from libsynnefo
IF(NOT Qt5Widgets_FOUND)
  FIND_PACKAGE(Qt4 REQUIRED)
  INCLUDE(   ${QT_USE_FILE}   )
ENDIF()

# test standalone
IF (NOT OYRANOS_FOUND)
  FIND_PACKAGE(Synnefo REQUIRED)
  INCLUDE_DIRECTORIES(${SYNNEFO_INCLUDE_DIR})
ENDIF (NOT OYRANOS_FOUND)

# rely on library in tar ball
IF    (NOT SYNNEFO_FOUND)
  SET( SYNNEFO_LIBRARY ${SY_LIB_BASE} )
# set up include-directories
  SET( synnefo_modules_path
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

SET(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")

INCLUDE_DIRECTORIES(
  "${PROJECT_SOURCE_DIR}"   # to find foo/foo.h
  "${PROJECT_BINARY_DIR}")  # to find foo/config.h

SET( synnefo_SRCS
    # core sources
    main.cpp
    synnefo.cpp
)
 
SET( synnefo_MOC_HEADERS
    # main window
    synnefo.h
)

SET( synnefo_UIs
    # main ui file
    ui/synnefo.ui
)

SET(synnefo_RCCS
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
  SET( OSX_ICON_FILES ${PROJECT_SOURCE_DIR}/extras/osx/synnefo.icns  )
  CONFIGURE_FILE (
                "${CMAKE_CURRENT_SOURCE_DIR}/extras/osx/Synnefo.sh.in"
                "${CMAKE_CURRENT_BINARY_DIR}/extras/osx/Synnefo.sh"
                @ONLY
                 )
  SET( OSX_SCRIPT_FILES "${CMAKE_CURRENT_BINARY_DIR}/extras/osx/Synnefo.sh" )
  SET( OSX_PKG_FILES ${PROJECT_SOURCE_DIR}/extras/osx/PkgInfo )

  SET( SyAPP Synnefo )
  ADD_EXECUTABLE( ${SyAPP} MACOSX_BUNDLE ${synnefo_SRCS} ${synnefo_RCC_SRCS} ${OSX_ICON_FILES} )

  INSTALL( TARGETS ${SyAPP}
           EXPORT SynnefoLibraryDepends
           RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
           BUNDLE DESTINATION  "${INSTALL_BIN_DIR}" COMPONENT bin
           RENAME ${SyAPP}.bin
         )

  SET_TARGET_PROPERTIES( ${SyAPP} PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME Synnefo )
  SET_TARGET_PROPERTIES( ${SyAPP} PROPERTIES MACOSX_BUNDLE_INFO_STRING "Synnefo front end to Oyranos Color Management System, www.oyranos.org" )
  SET_TARGET_PROPERTIES( ${SyAPP} PROPERTIES MACOSX_BUNDLE_EXECUTABLE_NAME Synnefo )
  SET_TARGET_PROPERTIES( ${SyAPP} PROPERTIES MACOSX_BUNDLE_ICON_FILE synnefo.icns )
  SET_TARGET_PROPERTIES( ${SyAPP} PROPERTIES MACOSX_BUNDLE_GUI_IDENTIFIER org.oyranos.synnefo )
  SET_TARGET_PROPERTIES( ${SyAPP} PROPERTIES MACOSX_BUNDLE_LONG_VERSION_STRING "Synnefo ${synnefo_VERSION}" )
  SET_TARGET_PROPERTIES( ${SyAPP} PROPERTIES MACOSX_BUNDLE_SHORT_VERSION_STRING ${synnefo_VERSION} )
  SET_TARGET_PROPERTIES( ${SyAPP} PROPERTIES MACOSX_BUNDLE_BUNDLE_VERSION 1.0 )
  SET_TARGET_PROPERTIES( ${SyAPP} PROPERTIES MACOSX_BUNDLE_COPYRIGHT "2011-2013 Joseph Simon, 2012-2015 Kai-Uwe Behrmann" )

  # configure CMake to use a custom Info.plist
  #SET_TARGET_PROPERTIES( ${SyAPP} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/extras/osx/Info.plist )

  # set where in the bundle to put the icns files
  SET_SOURCE_FILES_PROPERTIES( ${OSX_ICON_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

# Now the work of copying dependencies into the bundle/package
# The quotes are escaped and variables to use at install time have their $ escaped
# An alternative is the do a configure_file() on a script and use install(SCRIPT  ...).
# Note that the image plugins depend on QtSvg and QtXml, and it got those copied
# over.
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/${SyAPP}.app")  # paths to executables
  SET(DIRS "\${CMAKE_INSTALL_PREFIX}/lib;\${CMAKE_CURRENT_BINARY_DIR};\${CMAKE_INSTALL_PREFIX}/Library/Frameworks")   # directories to search for prerequisites
  INSTALL(CODE "
   include(BundleUtilities)
   fixup_bundle(\"${APPS}\"   \"\"   \"${DIRS}\")
   " COMPONENT Runtime)

  # this is the script to setup the environment and call the real binary
  INSTALL( PROGRAMS "${OSX_SCRIPT_FILES}" DESTINATION "bin/${SyAPP}.app/Contents/MacOS"
           RENAME ${SyAPP} )
  INSTALL( FILES "${OSX_PKG_FILES}" DESTINATION "bin/${SyAPP}.app/Contents" COMPONENT bin )

  # place the binary
  INSTALL( PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${SyAPP}.app/Contents/MacOS/Synnefo
           DESTINATION "bin/${SyAPP}.app/Contents/MacOS"
           RENAME ${SyAPP}.bin )

  # install Oyranos modules
  FILE( GLOB OYRANOS_ALL_CMMS ${OYRANOS_CMM_DIR}/liboyranos_*_cmm* )
  INSTALL( FILES ${OYRANOS_ALL_CMMS} DESTINATION "bin/${SyAPP}.app/Contents/Resources/${OYRANOS_CMM_SUBDIR}" COMPONENT lib )
  FILE( GLOB OYRANOS_ALL_META ${OYRANOS_META_DIR}/liboyranos_*_cmm* )
  INSTALL( FILES ${OYRANOS_ALL_META} DESTINATION "bin/${SyAPP}.app/Contents/Resources/${OYRANOS_META_SUBDIR}" COMPONENT lib )
  FOREACH( oy_dep_lib ${OYRANOS_MODULE_LIBRARIES} )
    FILE( GLOB OYRANOS_MODULE_DEP_LIBRARIES ${OYRANOS_META_DIR}/../lib${oy_dep_lib}*.dylib )
    INSTALL( FILES ${OYRANOS_MODULE_DEP_LIBRARIES} DESTINATION "bin/${SyAPP}.app/Contents/MacOS" COMPONENT lib )
  ENDFOREACH( oy_dep_lib )

  INSTALL( PROGRAMS ${OYRANOS_META_DIR}/../../bin/oyranos-profile-graph DESTINATION "bin/${SyAPP}.app/Contents/MacOS" COMPONENT bin )

  FIND_PACKAGE(PkgConfig)
  PKG_CHECK_MODULES(ELEKTRA elektra)
  INSTALL( DIRECTORY ${ELEKTRA_LIBDIR}/elektra DESTINATION "bin/${SyAPP}.app/Contents/Resources" COMPONENT lib )


ELSEIF(UNIX)
  SET( SyAPP oyranos-config-synnefo${SY_QT_SUFFIX} )
  ADD_EXECUTABLE(${SyAPP} ${synnefo_SRCS} ${synnefo_RCC_SRCS})
ELSEIF (WIN32)
  SET( SyAPP oyranos-config-synnefo${SY_QT_SUFFIX} )
  ADD_EXECUTABLE(${SyAPP} WIN32 ${synnefo_SRCS} ${synnefo_RCC_SRCS})
ENDIF ()

SET( SyAPP ${SyAPP} PARENT_SCOPE )

EXPORT(TARGETS ${SyAPP}
  FILE "${PROJECT_BINARY_DIR}/SynnefoLibraryDepends.cmake")

EXPORT(PACKAGE ${SyAPP})


TARGET_LINK_LIBRARIES(${SyAPP} ${SYNNEFO_LIBRARY} ${QT_LIBRARIES})

IF( NOT APPLE )
  INSTALL( TARGETS ${SyAPP}
  EXPORT SynnefoLibraryDepends
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
  INSTALL( FILES extras/${SyAPP}.desktop DESTINATION share/applications)
  INSTALL( FILES extras/oyranos-config-synnefo.svg DESTINATION share/pixmaps )
  INSTALL( FILES extras/oyranos-config-synnefo.png DESTINATION share/pixmaps )
  INSTALL( FILES extras/${SyAPP}.1 DESTINATION share/man/man1)
ENDIF()
