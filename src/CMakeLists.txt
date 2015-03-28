project (synnefo)

IF(QT4_FOUND)
    INCLUDE(   ${QT_USE_FILE}   )
ENDIF()

set( synnefo-common_SRCS
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
 
set( synnefo-common_PUBLIC
    # config
    classes/syconfig.h

    # module config
    modules/symoduleconfig.h

    # modules
    modules/database/sy_database.h
    modules/devices/sy_devices.h
    modules/information/sy_info.h
    modules/settings/sy_settings.h    
)

set( synnefo-common_MOC_HEADERS
    # config
    ${synnefo-common_PUBLIC}

    modules/devices/sy_devices_config.h

    modules/information/sy_info_config.h
    modules/information/sy_info_dialog.h

    modules/settings/sy_settings_config.h
    modules/database/sy_database_config.h
)

set( synnefo-common_UIs
    # config ui file
    ui/synnefo-config.ui

    # module config UI file 
    modules/symoduleconfig.ui

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

set(synnefo-common_RCCS
      modules/information/icons.qrc      
      modules/devices/icons.qrc            
      )

IF(QT4_FOUND)
  QT4_ADD_RESOURCES(synnefo-common_RCC_SRCS ${synnefo-common_RCCS})
  QT4_WRAP_UI (synnefo-common_SRCS ${synnefo-common_UIs})
  QT4_WRAP_CPP(synnefo-common_SRCS ${synnefo-common_MOC_HEADERS})
ENDIF()
IF(Qt5Widgets_FOUND)
  QT5_ADD_RESOURCES(synnefo-common_RCC_SRCS ${synnefo-common_RCCS})
  SET(CMAKE_AUTOMOC ON)
  QT5_WRAP_UI(synnefo-common_SRCS ${synnefo-common_UIs})
  INCLUDE_DIRECTORIES( ${Qt5Widgets_INCLUDE_DIRS} )
ENDIF()

ADD_LIBRARY(synnefo-common SHARED ${synnefo-common_SRCS} ${synnefo-common_RCC_SRCS})
ADD_LIBRARY(synnefo-common-static STATIC ${synnefo-common_SRCS} ${synnefo-common_RCC_SRCS})

# versions for libraries
SET_TARGET_PROPERTIES(   synnefo-common synnefo-common-static
    PROPERTIES VERSION   ${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_MICRO_VERSION}
               SOVERSION ${${PROJECT_NAME}_MAJOR_VERSION}
                     )

IF(WIN32)
  TARGET_LINK_LIBRARIES(synnefo-common-static ${OYRANOS_LIBRARIES} ${QT_LIBRARIES} ${QT_GIF_PLUGIN} ${QT_JPEG_PLUGIN})
  TARGET_LINK_LIBRARIES(synnefo-common ${OYRANOS_LIBRARIES} ${QT_LIBRARIES} ${QT_GIF_PLUGIN} ${QT_JPEG_PLUGIN})
ELSE()
  TARGET_LINK_LIBRARIES(synnefo-common-static ${OYRANOS_LIBRARIES} ${QT_LIBRARIES})
  TARGET_LINK_LIBRARIES(synnefo-common ${OYRANOS_LIBRARIES} ${QT_LIBRARIES})
ENDIF()

IF(Qt5Widgets_FOUND)
  TARGET_LINK_LIBRARIES(synnefo-common-static ${OYRANOS_LIBRARIES} Qt5::Widgets)
  TARGET_LINK_LIBRARIES(synnefo-common ${OYRANOS_LIBRARIES} Qt5::Widgets)
ENDIF()


INSTALL (TARGETS synnefo-common synnefo-common-static DESTINATION lib${LIB_SUFFIX})
INSTALL (FILES ${synnefo-common_PUBLIC}
               modules/symodule.h
               modules/devices/sy_devices_config.h
               modules/devices/sy_devices_item.h
               modules/information/sy_info_dialog.h
               modules/settings/sy_settings_config.h
         DESTINATION include/${PROJECT_NAME})

add_subdirectory(app)

