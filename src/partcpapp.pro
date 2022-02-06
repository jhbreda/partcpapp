QT += core gui webenginecore widgets webenginewidgets

CONFIG += c++11

SOURCES += partcp.cpp \
           CustomWebEnginePage.cpp \
           CustomWebengineView.cpp \
           MainWindow.cpp

HEADERS += MainWindow.h \
    CustomWebEnginePage.h \
    CustomWebengineView.h

TRANSLATIONS += partcpapp_de_DE.ts \
                partcpapp_en_GB.ts \
                partcpapp_nl_NL.ts \
                partcpapp_fr_FR.ts \
                partcpapp_sv_SE.ts

RESOURCES += styles/styles.qrc

CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
