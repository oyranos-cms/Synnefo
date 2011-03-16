project (synnefo)

find_package(Qt4)

INCLUDE(${QT_USE_FILE})

set( synnefo_SRCS
    # core sources
    main.cpp    
    synnefo.cpp   

    # synnefo class
    modules/symodule.cpp

    # synnefo modules
    modules/devices/sy_devices.cpp
    modules/information/sy_info.cpp
    modules/settings/sy_settings.cpp    
)
 
SET( synnefo_MOC_HEADERS
    # main window
    synnefo.h

    # modules
    modules/devices/sy_devices.h
    modules/information/sy_info.h
    modules/settings/sy_settings.h
)

set( synnefo_UIs
    # main ui file
    ui/synnefo.ui
    # module UI files
    modules/devices/sy_devices.ui
    modules/information/sy_info.ui
    modules/settings/sy_settings.ui
)

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    # modules
    "${CMAKE_CURRENT_SOURCE_DIR}/modules"
    "${CMAKE_CURRENT_SOURCE_DIR}/modules/devices"
    "${CMAKE_CURRENT_SOURCE_DIR}/modules/settings"
    "${CMAKE_CURRENT_SOURCE_DIR}/modules/information"
    ${CMAKE_CURRENT_BINARY_DIR}
)

QT4_WRAP_UI (synnefo_SRCS ${synnefo_UIs})

QT4_WRAP_CPP(synnefo_SRCS ${synnefo_MOC_HEADERS})


#set(synnefo_RCCS
#      main_resources.qrc
#      )

#QT4_ADD_RESOURCES(synnefo_RCC_SRCS ${synnefo_RCCS})


IF(UNIX)
  add_executable(synnefo ${synnefo_SRCS})
ELSEIF(APPLE)
  add_executable(synnefo MACOSX_BUNDLE ${synnefo_SRCS})
ELSEIF (WIN32)
  add_executable(synnefo WIN32 ${synnefo_SRCS})
ENDIF ()

IF(WIN32)
  target_link_libraries(synnefo ${OYRANOS_LIBRARIES} ${QT_LIBRARIES} ${QT_GIF_PLUGIN} ${QT_JPEG_PLUGIN})
ELSE()
  target_link_libraries(synnefo ${OYRANOS_LIBRARIES} ${QT_LIBRARIES})
ENDIF()
