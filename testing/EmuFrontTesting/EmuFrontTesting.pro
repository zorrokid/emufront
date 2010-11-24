#-------------------------------------------------
#
# Project created by QtCreator 2010-11-21T22:52:33
#
#-------------------------------------------------

QT       += core
QT       += testlib
QT       -= gui

#INCLUDEPATH += "../../src/dataobjects"
#include("../../src/emufront.pro")

#INCLUDEPATH += . "../../src/"
TARGET = EmuFrontTesting
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    platformtest.cpp \
    ../../src/dataobjects/platform.cpp \
    ../../src/dataobjects/mediatype.cpp \
    ../../src/dataobjects/emufrontobject.cpp \
    ../../src/dataobjects/emufrontfileobject.cpp \
    ../../src/dataobjects/emufrontfile.cpp \
    mediatypetest.cpp

HEADERS += \
    platformtest.h \
    ../../src/dataobjects/platform.h \
    ../../src/dataobjects/mediatype.h \
    ../../src/dataobjects/emufrontobject.h \
    ../../src/dataobjects/emufrontfileobject.h \
    ../../src/dataobjects/emufrontfile.h \
    mediatypetest.h
