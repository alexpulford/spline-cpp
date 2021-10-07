
ADS_OUT_ROOT = $${OUT_PWD}/..

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

adsBuildStatic {
    DEFINES += ADS_STATIC
}

SOURCES += \
    basisedit.cpp \
    basisview.cpp \
    curvatureview.cpp \
    geometry.cpp \
    knotedit.cpp \
    main.cpp \
    mainwindow.cpp \
    polynomial.cpp \
    spline.cpp \
    splineparam.cpp \
    splineview.cpp \
    view.cpp

HEADERS += \
    basisedit.h \
    basisview.h \
    controller.h \
    curvatureview.h \
    geometry.h \
    knotedit.h \
    linalg.h \
    mainwindow.h \
    models.h \
    polynomial.h \
    spline.h \
    splineparam.h \
    splineview.h \
    view.h

FORMS += \
    basisedit.ui \
    knotedit.ui \
    mainwindow.ui \
    splineparam.ui

LIBS += -L$${ADS_OUT_ROOT}/lib
include(../docking.pri)
INCLUDEPATH += ../ads
DEPENDPATH += ../ads

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    2D_line.frag \
    2D_line.geom \
    2D_line.vert \
    3D_line.frag \
    3D_line.geom \
    3D_line.vert \
    color_shader.frag \
    color_shader.vert

RESOURCES += \
    embed.qrc
