#-------------------------------------------------
#
# Project created by QtCreator 2022-11-09T14:29:54
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pvz
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
    monster.cpp \
    plantpos.cpp \
    plant.cpp \
    select.cpp \
    bullet.cpp \
    startscene.cpp \
    fileselect.cpp \
    menu.cpp

HEADERS += \
        mainwindow.h \
    monster.h \
    plantpos.h \
    plant.h \
    select.h \
    bullet.h \
    utility.h \
    startscene.h \
    fileselect.h \
    menu.h

FORMS += \
        mainwindow.ui \
    startscene.ui \
    fileselect.ui \
    menu.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
