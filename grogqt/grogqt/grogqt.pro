#-------------------------------------------------
#
# Project created by QtCreator 2019-02-10T15:39:29
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = grogqt
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    ../../grog/grog.cpp \
    ../../grog/SceneNode.cpp \
    ../../grog/Mesh.cpp \
    ../../grog/Engine.cpp \
    ../../grog/Car.cpp \
    ../../grog/Road.cpp \
    Display.cpp \
    ../../grog/Matrix.cpp

HEADERS += \
        mainwindow.h \
    ../../grog/grog.h \
    ../../grog/common.h \
    ../../grog/SceneNode.h \
    ../../grog/Mesh.h \
    ../../grog/Engine.h \
    ../../grog/Matrix.h \
    ../../grog/trigo.h \
    ../../grog/asset_car.h \
    ../../grog/asset_road.h \
    ../../grog/Car.h \
    ../../grog/Road.h \
    ../../grog/asset_tree.h \
    Gamebuino-Meta-stuff.h \
    Display.h

FORMS += \
        mainwindow.ui


INCLUDEPATH += ../../grog

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../../grog/projection.md
