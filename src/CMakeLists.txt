 
include_directories(${CMAKE_CURRENT_BINARY_DIR})

INCLUDE(   ${QT_USE_FILE}   )

set(synnefo_SRCS
    main.cpp
)
 
set(synnefo_PART_SRCS
    synnefo.cpp   
)

SET(synnefo_MOC_HDRS
    synnefo.h
)

set(synnefo_UIS
    ui/synnefo-main.ui
)

#QT4_AUTOMOC(${synnefo_PART_SRCS})

QT4_WRAP_UI (synnefo_UIS_H ${synnefo_UIS})

QT4_WRAP_CPP(synnefo_MOC_SRCS ${synnefo_MOC_HDRS})


#set(synnefo_RCCS
#      main_resources.qrc
#      )

#QT4_ADD_RESOURCES(synnefo_RCC_SRCS ${synnefo_RCCS})

add_executable(synnefo ${synnefo_SRCS} ${synnefo_PART_SRCS} ${synnefo_UIS} ${synnefo_MOC_HDRS})
target_link_libraries(synnefo ${OYRANOS_LIBRARIES} ${QT_LIBRARIES} )

#install(TARGETS kmdevices DESTINATION ${PLUGIN_INSTALL_DIR})
#install( FILES kmdevices.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} )
