project (synnefo)

find_package(Qt4)
INCLUDE(   ${QT_USE_FILE}   )



INCLUDE(${QT_USE_FILE})

set( synnefo_SRCS
    # core sources
    main.cpp    
    synnefo.cpp   

    classes/syconfig.cpp

    # synnefo class
    modules/symodule.cpp
    modules/symoduleconfig.cpp

    # synnefo modules
    modules/database/sy_database.cpp
    modules/devices/sy_devices.cpp
    modules/information/sy_info.cpp
    modules/settings/sy_settings.cpp    

    modules/devices/sy_devices_item.cpp
    modules/information/sy_info_dialog.cpp    

    modules/database/sy_database_config.cpp
    modules/devices/sy_devices_config.cpp
    modules/information/sy_info_config.cpp
    modules/settings/sy_settings_config.cpp
)
 
SET( synnefo_MOC_HEADERS
    # main window
    synnefo.h

    # config 
    classes/syconfig.h

    # modules
    modules/database/sy_database.h
    modules/devices/sy_devices.h
    modules/information/sy_info.h
    modules/settings/sy_settings.h    

    modules/devices/sy_devices_config.h

    modules/information/sy_info_config.h
    modules/information/sy_info_dialog.h

    modules/settings/sy_settings_config.h
    modules/database/sy_database_config.h
)

set( synnefo_UIs
    # main ui file
    ui/synnefo.ui
 
    # config ui files
    ui/synnefo-config.ui

    # module UI files
    modules/database/ui/sy_database.ui
    modules/database/ui/sy_database_config.ui
    modules/devices/ui/sy_devices.ui
    modules/devices/ui/sy_devices_config.ui
    modules/information/ui/sy_info.ui
    modules/information/ui/sy_info_config.ui
    modules/information/ui/sy_info_dialog.ui    
    modules/settings/ui/sy_settings.ui
    modules/settings/ui/sy_settings_config.ui    
)

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    # modules
    "${CMAKE_CURRENT_SOURCE_DIR}/classes"
    "${CMAKE_CURRENT_SOURCE_DIR}/modules"
    "${CMAKE_CURRENT_SOURCE_DIR}/modules/database"
    "${CMAKE_CURRENT_SOURCE_DIR}/modules/database/ui"
    "${CMAKE_CURRENT_SOURCE_DIR}/modules/devices"
    "${CMAKE_CURRENT_SOURCE_DIR}/modules/devices/ui"
    "${CMAKE_CURRENT_SOURCE_DIR}/modules/information"
    "${CMAKE_CURRENT_SOURCE_DIR}/modules/information/ui"
    "${CMAKE_CURRENT_SOURCE_DIR}/modules/settings"
    "${CMAKE_CURRENT_SOURCE_DIR}/modules/settings/ui"
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(synnefo_RCCS
      ui/resources.qrc
      modules/information/icons.qrc      
      modules/devices/icons.qrc            
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

IF(WIN32)
  target_link_libraries(synnefo ${OYRANOS_LIBRARIES} ${QT_LIBRARIES} ${QT_GIF_PLUGIN} ${QT_JPEG_PLUGIN})
ELSE()
  target_link_libraries(synnefo ${OYRANOS_LIBRARIES} ${QT_LIBRARIES})
ENDIF()
