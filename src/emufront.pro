# #####################################################################
# Automatically generated by qmake (2.01a) Fri May 14 22:39:13 2010
# #####################################################################
TEMPLATE = app
TARGET = 
DEPENDPATH += . \
    exceptions \
    dataobjects \
    db \
    dialogs \
    models \
    utils \
    widgets \
    delegates \
    views
INCLUDEPATH += . \
    db \
    exceptions \
    dataobjects \
    dialogs \
    models \
    utils \
    widgets \
    delegates \
    views
QT += sql
QT += xml


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
    db/dbplatform.h \
    db/dbcreator.h \
    dialogs/mediatypedialog.h \
    dataobjects/emufrontfileobject.h \
    dataobjects/mediatype.h \
    db/dbmediatype.h \
    dialogs/mediatypenamedialog.h \
    dataobjects/filepathobject.h \
    dialogs/mediaimagepathdialog.h \
    dialogs/mediaimagepathmaindialog.h \
    dialogs/dataobjecteditdialog.h \
    db/dbfilepath.h \
    utils/ziputil.h \
    utils/fileutil.h \
    dataobjects/setup.h \
    db/dbsetup.h \
    dialogs/setupmaindialog.h \
    dialogs/setupeditdialog.h \
    widgets/stringlistwidget.h \
    exceptions/emufrontexception.h \
    dataobjects/mediaimage.h \
    dataobjects/mediaimagecontainer.h \
    dataobjects/emufrontfile.h \
    db/dbfile.h \
    db/dbemufrontfileobject.h \
    db/dbmediaimagecontainer.h \
    db/dbmediaimage.h \
    emulauncher.h \
    widgets/efcombobox.h \
    widgets/effileobjectcombobox.h \
    widgets/setupcombobox.h \
    dataobjects/executable.h \
    db/dbexecutable.h \
    dialogs/executablemaindialog.h \
    dialogs/executableeditdialog.h \
    widgets/executablecombobox.h \
    utils/processhelper.h \
    utils/unziphelper.h \
    utils/emuhelper.h \
    dialogs/listdialog.h \
    dialogs/emufrontinputdialog.h \
    dialogs/emufrontfileobjectdialog.h \
    dialogs/browsefilepathdialog.h \
    db/dbconfig.h \
    utils/datfileutil.h \
    widgets/fileextensionwidget.h \
    models/setupmodel.h \
    models/platformmodel.h \
    models/mediatypemodel.h \
    models/emufrontquerymodel.h \
    models/emufrontfileobjectmodel.h \
    views/setupeditview.h \
    views/platformeditview.h \
    views/mediatypeeditview.h \
    views/emufrontfileobjecteditview.h \
    views/emufronteditview.h \
    delegates/comboboxdelegate.h \
    delegates/stringlistdelegate.h \
    models/filepathmodel.h \
    views/filepatheditview.h
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
    db/dbplatform.cpp \
    db/dbcreator.cpp \
    dialogs/mediatypedialog.cpp \
    dataobjects/emufrontfileobject.cpp \
    dataobjects/mediatype.cpp \
    db/dbmediatype.cpp \
    dialogs/mediatypenamedialog.cpp \
    dataobjects/filepathobject.cpp \
    dialogs/mediaimagepathdialog.cpp \
    dialogs/mediaimagepathmaindialog.cpp \
    dialogs/dataobjecteditdialog.cpp \
    db/dbfilepath.cpp \
    utils/ziputil.cpp \
    utils/fileutil.cpp \
    dataobjects/setup.cpp \
    db/dbsetup.cpp \
    db/dbtablemodelmanager.cpp \
    db/dbquerymodelmanager.cpp \
    dialogs/setupmaindialog.cpp \
    dialogs/setupeditdialog.cpp \
    widgets/stringlistwidget.cpp \
    exceptions/emufrontexception.cpp \
    dataobjects/mediaimage.cpp \
    dataobjects/mediaimagecontainer.cpp \
    dataobjects/emufrontfile.cpp \
    db/dbfile.cpp \
    db/dbemufrontfileobject.cpp \
    db/dbmediaimagecontainer.cpp \
    db/dbmediaimage.cpp \
    emulauncher.cpp \
    widgets/efcombobox.cpp \
    widgets/effileobjectcombobox.cpp \
    widgets/setupcombobox.cpp \
    dataobjects/executable.cpp \
    db/dbexecutable.cpp \
    dialogs/executablemaindialog.cpp \
    dialogs/executableeditdialog.cpp \
    widgets/executablecombobox.cpp \
    utils/processhelper.cpp \
    utils/unziphelper.cpp \
    utils/emuhelper.cpp \
    dialogs/listdialog.cpp \
    dialogs/emufrontinputdialog.cpp \
    dialogs/emufrontfileobjectdialog.cpp \
    dialogs/browsefilepathdialog.cpp \
    db/dbconfig.cpp \
    utils/datfileutil.cpp \
    widgets/fileextensionwidget.cpp \
    models/setupmodel.cpp \
    models/platformmodel.cpp \
    models/mediatypemodel.cpp \
    models/emufrontquerymodel.cpp \
    models/emufrontfileobjectmodel.cpp \
    views/setupeditview.cpp \
    views/platformeditview.cpp \
    views/mediatypeeditview.cpp \
    views/emufrontfileobjecteditview.cpp \
    views/emufronteditview.cpp \
    delegates/comboboxdelegate.cpp \
    delegates/stringlistdelegate.cpp \
    models/filepathmodel.cpp \
    views/filepatheditview.cpp
OTHER_FILES +=  

CONFIG += mobility

MOBILITY = 

unix {
   INSTALLS += target desktop icon48 icon64
   target.path = /usr/bin
   desktop.path = /usr/share/applications/hildon
   desktop.files += maemofiles/emufront.desktop
   icon48.path = /usr/share/icons/hicolor/48x48/apps
   icon48.files += maemofiles/48x48/emufront.png
   icon64.path = /usr/share/icons/hicolor/64x64/apps
   icon64.files += maemofiles/64x64/emufront.png
}
