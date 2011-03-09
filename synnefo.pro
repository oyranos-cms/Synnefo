HEADERS           = synnefo.h
SOURCES           = synnefo.cpp \
                    main.cpp
FORMS             = syfoMain.ui
# RESOURCES         = synnefo.qrc

 # install
 target.path = $$[QT_INSTALL_EXAMPLES]/bin/synnefo
 sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS synnefo.pro
 sources.path = $$[QT_INSTALL_EXAMPLES]./
 INSTALLS += target sources