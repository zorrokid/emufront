#-------------------------------------------------
#
# Project created by QtCreator 2010-11-21T22:52:33
#
#-------------------------------------------------

QT       += core
QT       += testlib
QT       += gui
QT       += sql

TARGET = EmuFrontTesting
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    platformtest.cpp \
    ../../src/exceptions/emufrontexception.cpp \
    ../../src/dataobjects/emufrontobject.cpp \
    ../../src/dataobjects/platform.cpp \
    ../../src/db/databasemanager.cpp \
    ../../src/db/dbquerymodelmanager.cpp \
    ../../src/db/dbplatform.cpp \
    ../../src/db/dbfile.cpp \
    ../../src/dataobjects/mediatype.cpp \
    ../../src/dataobjects/emufrontfileobject.cpp \
    ../../src/db/dbemufrontfileobject.cpp \
    ../../src/dataobjects/emufrontfile.cpp \
    ../../src/dialogs/dataobjecteditdialog.cpp \
    ../../src/dialogs/emufrontdialog.cpp \
    ../../src/dialogs/namedialog.cpp \
    ../../src/dialogs/platformnamedialog.cpp \
    mediatypetest.cpp \
    platformnamedialogtest.cpp \
    emufrontobjecttest.cpp \
    dbplatformtest.cpp

HEADERS += \
    platformtest.h \
    ../../src/exceptions/emufrontexception.h \
    ../../src/dataobjects/emufrontobject.h \
    ../../src/dataobjects/platform.h \
    ../../src/db/databasemanager.h \
    ../../src/db/dbquerymodelmanager.h \
    ../../src/db/dbplatform.h \
    ../../src/db/dbfile.h \
    ../../src/dataobjects/mediatype.h \
    ../../src/db/dbemufrontfileobject.h \
    ../../src/dataobjects/emufrontfileobject.h \
    ../../src/dataobjects/emufrontfile.h \
    ../../src/dialogs/dataobjecteditdialog.h \
    ../../src/dialogs/emufrontdialog.h \
   ../../src/dialogs/namedialog.h \
   ../../src/dialogs/platformnamedialog.h \
   mediatypetest.h \
   platformnamedialogtest.h \
    emufrontobjecttest.h \
    dbplatformtest.h
