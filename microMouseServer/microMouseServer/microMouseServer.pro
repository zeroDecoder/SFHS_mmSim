#-------------------------------------------------
#
# Project created by QtCreator 2015-04-07T17:58:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = microMouseServer
TEMPLATE = app


SOURCES += mazegui.cpp\
        main.cpp \
        micromouseserver.cpp \
    mazeBase.cpp \
    networkinterface.cpp


HEADERS  += micromouseserver.h \
    mazeConst.h \
    mazeBase.h \
    mazegui.h \
    networkinterface.h

FORMS    += micromouseserver.ui

win32:LIBS += -lWS2_32
