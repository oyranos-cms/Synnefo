#
# Target installation folders
#

SET (TARGET_INCLUDE_FOLDER
                "synnefo"
                CACHE STRING
                "Optional folder below system include folder to install include files."
    )

IF(NOT CMAKE_MODULES_INSTALL_DIR)
  SET (CMAKE_MODULES_INSTALL_DIR
		"${CMAKE_INSTALL_LIBDIR}/cmake/${PACKAGE_NAME}"
		CACHE STRING
		"The folder where to install cmake files."
    )
ENDIF(NOT CMAKE_MODULES_INSTALL_DIR)

SET (TARGET_PLUGIN_FOLDER "synnefo"
                CACHE STRING
                "Optional folder below system library folder where to install synnefo plugins. LIB_SUFFIX is honored."
    )

SET (TARGET_PKGCONFIG_FOLDER
                "pkgconfig"
                CACHE STRING
                "The folder below system library folder where to install pkgconfig files. LIB_SUFFIX is honored."
    )

SET (TARGET_DOCUMENTATION_FOLDER
                "share/doc/synnefo-api"
                CACHE STRING
                "The folder below system prefix where to install api documentation files."
    )


#
# Misc.
#

SET (LIB_SUFFIX ""
                CACHE STRING
                "Optional suffix to use on lib folders (e.g. 64 for lib64)"
    )
#cmake settings for use by find_package
CONFIGURE_FILE (
                "${CMAKE_CURRENT_SOURCE_DIR}/SynnefoConfig.cmake.in"
                "${CMAKE_CURRENT_BINARY_DIR}/SynnefoConfig.cmake"
                @ONLY
               )

INSTALL ( FILES
                "${CMAKE_CURRENT_BINARY_DIR}/SynnefoConfig.cmake"
                DESTINATION ${CMAKE_MODULES_INSTALL_DIR}
                COMPONENT dev
        )

