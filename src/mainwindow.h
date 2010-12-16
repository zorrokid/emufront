/*
** EmuFront
** Copyright 2010 Mikko Keinänen
**
** This file is part of EmuFront.
**
**
** EmuFront is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License version 2 as published by
** the Free Software Foundation and appearing in the file gpl.txt included in the
** packaging of this file.
**
** EmuFront is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with EmuFront.  If not, see <http://www.gnu.org/licenses/>.
*/#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
// TODO: DEPRECATED
//class PlatformDialog;
class PlatformEditView;
class MediaTypeEditView;
// TODO: DEPRECATED
//class MediaTypeDialog;
class MediaTypeEditView;
// TODO: DEPRECATED
//class MediaImagePathMainDialog;
class FilePathEditView;
// TODO: DEPRECATED
//class SetupMainDialog;
class SetupEditView;
// TODO: DEPRECATED
//class ExecutableMainDialog;
class ExternalExecutableEditView;
//class TmpFolderEditDialog;
class QLabel;
class DatabaseManager;
class EmuFrontDialog;
class EmuLauncher;
class DatFileUtil;
class QErrorMessage;
class SetupModel;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
    MainWindow(bool reset = false);
    //~MainWindow()

protected:
	void closeEvent(QCloseEvent *event);

private slots:
    // TODO: DEPRECATED
    //void configurePlatforms();
    void configurePlatformss();
    // TODO: DEPRECATED
    //void configureMediaTypes();
    void configureMediaTypess();
    // TODO: DEPRECATED
    //void configureMediaImagePaths();
    void configureMediaImagePathss();
    // TODO: DEPRECATED
    //void configureSetups();
    void configureSetupss();
    // TODO: DEPRECATED
    //void configureEmulators();
    void configureEmulatorss();
    void configureTmpDir();
    void resetDb();
    void updateData();
    void manageDatFiles();
    void about();

private:
    static QString aboutStr;
    static QString aboutTitle;
    void createActions();
	void createMenus();
	void createStatusBar();
	void readSettings();
	void writeSettings();
	bool okToContinue();
    void connectSignals();
    void activateDialog(EmuFrontDialog*) const;
    bool testDB(bool reset);
    void createDB() const;

    // TODO: deprecated
    //PlatformDialog *platformDialog;
    PlatformEditView *plfDialog;
    // TODO: deprecated
    //MediaTypeDialog *mediaTypeDialog;
    MediaTypeEditView *mdtDialog;
    // TODO: DEPRECATED
    //MediaImagePathMainDialog *mediaImagePathDialog;
    FilePathEditView *mediaImagePathView;
    // TODO: deprecated
    //SetupMainDialog *setupMainDialog;
    SetupEditView *setupMainView;
    // TODO: deprecated
    //ExecutableMainDialog *executableMainDialog;
    ExternalExecutableEditView *emulatorEditView;
    //TmpFolderEditDialog *tmpFolderDialog;
	QMenu *configMenu;
    QMenu *fileMenu;
    QMenu *helpMenu;
    // TODO: deprecated
    //QAction *configPlatformAction;
    QAction *configPlatformsAction;
    // TODO: deprecated
    //QAction *configMediaTypeAction;
    QAction *configMediaTypesAction;
    // TODO: DEPRECATED
    //QAction *configMediaImagePathAction;
    QAction *configMediaImagePathsAction;
    // TODO: deprecated
    //QAction *configSetupAction;
    QAction *configSetupsAction;
    // TODO: deprecated
    //QAction *configEmulatorAction;
    QAction *configEmulatorsAction;
    QAction *exitAction;
    QAction *resetDbAction;
    QAction *aboutAction;
    QAction *configTmpDirAction;
    QAction *manageDatFilesAction;
	QLabel *messageLabel;
    DatabaseManager *dbManager;
    EmuLauncher *launcher;
    QString tmpDirFilePath;
    QErrorMessage *errorMessage;
    SetupModel *supModel;
};

#endif
