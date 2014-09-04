PROJECT(synnefo-app)

cmake_minimum_required(VERSION 2.4)

find_package(Qt4 REQUIRED)
INCLUDE(   ${QT_USE_FILE}   )

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

QT4_ADD_RESOURCES(synnefo_RCC_SRCS ${synnefo_RCCS})

QT4_WRAP_UI (synnefo_SRCS ${synnefo_UIs})

QT4_WRAP_CPP(synnefo_SRCS ${synnefo_MOC_HEADERS})



IF(UNIX)
  add_executable(synnefo ${synnefo_SRCS} ${synnefo_RCC_SRCS})
ELSEIF(APPLE)
  add_executable(synnefo MACOSX_BUNDLE ${synnefo_SRCS} ${synnefo_RCC_SRCS})
ELSEIF (WIN32)
  add_executable(synnefo WIN32 ${synnefo_SRCS} ${synnefo_RCC_SRCS})
ENDIF ()

target_link_libraries(synnefo ${SYNNEFO_LIBRARY} ${QT_LIBRARIES})

install(TARGETS synnefo
  EXPORT SynnefoLibraryDepends
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
