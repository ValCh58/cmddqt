TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    tinystr.cpp \
    tinyxml.cpp \
    tinyxmlerror.cpp \
    tinyxmlparser.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    ../../Moxa/Source/cmbb MAC/cmbb/tinyxml/tinystr.h \
    ../../Moxa/Source/cmbb MAC/cmbb/tinyxml/tinyxml.h \
    tinystr.h \
    tinyxml.h

