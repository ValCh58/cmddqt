#-------------------------------------------------
#
# Project created by QtCreator 2015-10-02T13:58:10
#
#-------------------------------------------------
CONFIG += c++14
QT += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4):

TARGET = cmbbqt
TEMPLATE = app
LIBS += /usr/lib/x86_64-linux-gnu/libQt5SerialPort.so

SOURCES += main.cpp\
    calibrationmbac.cpp \
    config.cpp \
    xmldocument.cpp \
    ccmbb.cpp \
    datamodules.cpp \
    cmbblistmodules.cpp \
    viewmodules.cpp \
    viewbase.cpp \
    sport.cpp \
    protocol.cpp \
    ./tinyxml/tinystr.cpp \
    ./tinyxml/tinyxml.cpp \
    ./tinyxml/tinyxmlerror.cpp \
    ./tinyxml/tinyxmlparser.cpp \
    modules.cpp \
    myprogressbar.cpp \
    logfile.cpp \
    calibratedialog.cpp \
    settingsanalog.cpp \
    rateconstants.cpp \
    settingmodule.cpp \
    Style.cpp \
    testmodule.cpp \
    test.cpp \
    worker.cpp \
    testtimer.cpp \
    testuart1.cpp \
    testled.cpp \
    testuart0.cpp \
    checkknoawo.cpp \
    testdiscrnbc.cpp \
    testcheckfain1.cpp \
    testacp.cpp \
    testcalibrch.cpp \
    testdiscrnnc.cpp \
    about.cpp \
    testrds.cpp \
    discretrds.cpp \
    discritem.cpp \
    comdtords.cpp \
    daccalibration.cpp \
    mydataport.cpp \
    calibrationcheck.cpp

HEADERS  += \
    calibrationmbac.h \
    config.h \
    xmldocument.h \
    ccmbb.h \
    datamodules.h \
    cmbblistmodules.h \
    viewmodules.h \
    viewbase.h \
    sport.h \
    protocol.h \
    ./tinyxml/tinystr.h \
    ./tinyxml/tinyxml.h \
    modules.h \
    myprogressbar.h \
    logfile.h \
    calibratedialog.h \
    settingsanalog.h \
    rateconstants.h \
    settingmodule.h \
    Style.h \
    testmodule.h \
    test.h \
    worker.h \
    testtimer.h \
    testuart1.h \
    testled.h \
    testuart0.h \
    checkknoawo.h \
    testdiscrnbc.h \
    testcheckfain1.h \
    testacp.h \
    testcalibrch.h \
    testdiscrnnc.h \
    about.h \
    testrds.h \
    discretrds.h \
    discritem.h \
    comdtords.h \
    daccalibration.h \
    mydataport.h \
    consts.h \
    calibrationcheck.h

RESOURCES = images.qrc
RC_FILE = resource.rc
