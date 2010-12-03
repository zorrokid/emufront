// EmuFront
// Copyright 2010 Mikko Keinänen
//
// This file is part of EmuFront.
//
//
// EmuFront is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as published by
// the Free Software Foundation and appearing in the file gpl.txt included in the
// packaging of this file.
//
// EmuFront is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with EmuFront.  If not, see <http://www.gnu.org/licenses/>.

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
// TODO: DEPRECATED
class PlatformDialog;
class PlatformMainDialog;
class MediaTypeMainDialog;
// TODO: DEPRECATED
class MediaTypeDialog;
class MediaTypeMainDialog;
class MediaImagePathMainDialog;
// TODO: DEPRECATED
class SetupMainDialog;
class SetupMainView;
class ExecutableMainDialog;
//class TmpFolderEditDialog;
class QLabel;
class DatabaseManager;
class EmuFrontDialog;
class EmuLauncher;
class DatFileUtil;
class QErrorMessage;

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
    void configurePlatforms();
    void configurePlatformss();
    // TODO: DEPRECATED
    void configureMediaTypes();
    void configureMediaTypess();
    void configureMediaImagePaths();
    // TODO: DEPRECATED
    void configureSetups();
    void configureSetupss();
    void configureEmulators();
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
    PlatformDialog *platformDialog;
    PlatformMainDialog *plfDialog;
    // TODO: deprecated
    MediaTypeDialog *mediaTypeDialog;
    MediaTypeMainDialog *mdtDialog;
    MediaImagePathMainDialog *mediaImagePathDialog;
    // TODO: deprecated
    SetupMainDialog *setupMainDialog;
    SetupMainView *setupMainView;
    ExecutableMainDialog *executableMainDialog;
    //TmpFolderEditDialog *tmpFolderDialog;
	QMenu *configMenu;
    QMenu *fileMenu;
    QMenu *helpMenu;
    // TODO: deprecated
    QAction *configPlatformAction;
    QAction *configPlatformsAction;
    // TODO: deprecated
    QAction *configMediaTypeAction;
    QAction *configMediaTypesAction;
    QAction *configMediaImagePathAction;
    // TODO: deprecated
    QAction *configSetupAction;
    QAction *configSetupsAction;
    QAction *configEmulatorAction;
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
};

#endif
