#-------------------------------------------------
#
# Project created by QtCreator 2018-03-26T15:42:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PlayPainter
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


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    hpaintarea.cpp \
    htextcursor.cpp \
    hdocumentmodel.cpp \
    hrendercontroller.cpp \
    htextedit.cpp \
    hdocumentfind.cpp \
    htexteditfindview.cpp

HEADERS += \
        mainwindow.h \
    hpaintarea.h \
    htextcursor.h \
    hdocumentmodel.h \
    hrendercontroller.h \
    概要设计.h \
    htextedit.h \
    hdocumentfind.h \
    htexteditfindview.h

FORMS += \
    mainwindow.ui
