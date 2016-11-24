PROJECT( synnefo )

IF(QT4_FOUND)
    INCLUDE(   ${QT_USE_FILE}   )
ENDIF()

SET( synnefo-common_SRCS
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
 
SET( synnefo-common_PUBLIC
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
SET( synnefo-common_PUBLIC_NON_MOC
    modules/symodule.h
    modules/devices/sy_devices_config.h
    modules/devices/sy_devices_item.h
    modules/information/sy_info_dialog.h
    modules/settings/sy_settings_config.h
)

SET( synnefo-common_MOC_HEADERS
    # config
    ${synnefo-common_PUBLIC}

    modules/devices/sy_devices_config.h

    modules/information/sy_info_config.h
    modules/information/sy_info_dialog.h

    modules/settings/sy_settings_config.h
    modules/database/sy_database_config.h
)

SET( synnefo-common_UIs
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

SET(synnefo-common_RCCS
      modules/information/icons.qrc      
      modules/devices/icons.qrc            
      )

IF(QT4_FOUND)
  QT4_ADD_RESOURCES(synnefo-common_RCC_SRCS ${synnefo-common_RCCS})
  QT4_WRAP_UI (synnefo-common_SRCS ${synnefo-common_UIs})
  QT4_WRAP_CPP(synnefo-common_SRCS ${synnefo-common_MOC_HEADERS})
  SET( SY_QT_SUFFIX -qt4 )
ENDIF()
IF(Qt5Widgets_FOUND)
  QT5_ADD_RESOURCES(synnefo-common_RCC_SRCS ${synnefo-common_RCCS})
  SET(CMAKE_AUTOMOC ON)
  QT5_WRAP_UI(synnefo-common_SRCS ${synnefo-common_UIs})
  INCLUDE_DIRECTORIES( ${Qt5Widgets_INCLUDE_DIRS} )
ENDIF()

SET( SY_LIB_BASE OyranosSynnefo${SY_QT_SUFFIX} )
SET( SY_LIB_BASE ${SY_LIB_BASE} PARENT_SCOPE )

ADD_LIBRARY( ${SY_LIB_BASE} SHARED ${synnefo-common_SRCS} ${synnefo-common_RCC_SRCS}
                                  ${synnefo-common_PUBLIC} ${synnefo-common_PUBLIC_NON_MOC} )
ADD_LIBRARY( ${SY_LIB_BASE}-static STATIC ${synnefo-common_SRCS} ${synnefo-common_RCC_SRCS}
                                  ${synnefo-common_PUBLIC} ${synnefo-common_PUBLIC_NON_MOC} )

# OS X Framework
SET_TARGET_PROPERTIES( ${SY_LIB_BASE} PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION ${${PROJECT_NAME}_VERSION}
  PRIVATE_HEADER ""
  PUBLIC_HEADER "${synnefo-common_PUBLIC};${synnefo-common_PUBLIC_NON_MOC}"
  RESOURCE ""
  INSTALL_NAME_DIR "@executable_path/../../../../Library/Frameworks"
  #DEBUG_POSTFIX -d
)

# versions for libraries
SET_TARGET_PROPERTIES(   ${SY_LIB_BASE} ${SY_LIB_BASE}-static
    PROPERTIES VERSION   ${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_MICRO_VERSION}
               SOVERSION ${${PROJECT_NAME}_MAJOR_VERSION}
                     )

IF(Qt5Widgets_FOUND)
  SET( QT_ADD Qt5::Widgets Qt5::DBus )
ELSE()
  SET( QT_ADD QtDBus )
ENDIF()


IF(WIN32)
  TARGET_LINK_LIBRARIES(${SY_LIB_BASE}-static ${OYRANOS_LIBRARIES} ${QT_LIBRARIES} ${QT_GIF_PLUGIN} ${QT_JPEG_PLUGIN} ${QT_ADD})
  TARGET_LINK_LIBRARIES(${SY_LIB_BASE} ${OYRANOS_LIBRARIES} ${QT_LIBRARIES} ${QT_GIF_PLUGIN} ${QT_JPEG_PLUGIN} ${QT_ADD})
ELSE()
  TARGET_LINK_LIBRARIES(${SY_LIB_BASE}-static ${OYRANOS_LIBRARIES} ${QT_LIBRARIES} ${QT_ADD})
  TARGET_LINK_LIBRARIES(${SY_LIB_BASE} ${OYRANOS_LIBRARIES} ${QT_LIBRARIES} ${QT_ADD})
ENDIF()

IF(Qt5Widgets_FOUND)
  TARGET_LINK_LIBRARIES(${SY_LIB_BASE}-static ${OYRANOS_LIBRARIES} ${QT_ADD})
  TARGET_LINK_LIBRARIES(${SY_LIB_BASE} ${OYRANOS_LIBRARIES} ${QT_ADD})
ENDIF()


INSTALL (TARGETS ${SY_LIB_BASE} ${SY_LIB_BASE}-static
         FRAMEWORK DESTINATION Library/Frameworks
         LIBRARY DESTINATION lib${LIB_SUFFIX}
         ARCHIVE DESTINATION lib${LIB_SUFFIX}
         PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
        )

ADD_SUBDIRECTORY( app )
SET( SyAPP ${SyAPP} PARENT_SCOPE )

