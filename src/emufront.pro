# #####################################################################
# Automatically generated by qmake (2.01a) Fri May 14 22:39:13 2010
# #####################################################################
TEMPLATE = app
TARGET = 
DEPENDPATH += . \
    db \
    dialogs
INCLUDEPATH += . \
    db \
    dialogs
QT += sql

# Input
HEADERS += mainwindow.h \
    db/databasemanager.h \
    dialogs/dbobjectdialog.h \
    dialogs/namedialog.h \
    dialogs/platformdialog.h \
    dialogs/platformnamedialog.h \
    dialogs/emufrontdialog.h \
    dataobjects/emufrontobject.h \
    dataobjects/platform.h \
    db/dbplatform.h
SOURCES += main.cpp \
    mainwindow.cpp \
    db/databasemanager.cpp \
    dialogs/dbobjectdialog.cpp \
    dialogs/namedialog.cpp \
    dialogs/platformdialog.cpp \
    dialogs/platformnamedialog.cpp \
    dialogs/emufrontdialog.cpp \
    dataobjects/emufrontobject.cpp \
    dataobjects/platform.cpp \
    db/dbplatform.cpp
