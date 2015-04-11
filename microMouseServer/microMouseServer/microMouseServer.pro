#-------------------------------------------------
#
# Project created by QtCreator 2015-04-07T17:58:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = microMouseServer
TEMPLATE = app


SOURCES += main.cpp\
        micromouseserver.cpp

HEADERS  += micromouseserver.h \
    mazeConst.h \
    mazeBase.h

FORMS    += micromouseserver.ui
