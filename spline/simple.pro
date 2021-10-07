ADS_OUT_ROOT = $${OUT_PWD}/..

QT += core gui widgets

TARGET = SimpleExample
DESTDIR = $${ADS_OUT_ROOT}/lib
TEMPLATE = app
CONFIG += c++14
CONFIG += debug_and_release
adsBuildStatic {
    DEFINES += ADS_STATIC
}

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        MainWindow.cpp

HEADERS += \
        MainWindow.h

FORMS += \
        MainWindow.ui
        

LIBS += -L$${ADS_OUT_ROOT}/lib
include(../docking.pri)
INCLUDEPATH += ../ads
DEPENDPATH += ../ads
