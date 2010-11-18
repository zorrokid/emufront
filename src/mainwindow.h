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
class PlatformDialog;
class MediaTypeDialog;
class MediaImagePathMainDialog;
class SetupMainDialog;
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
	void configurePlatforms();
    void configureMediaTypes();
    void configureMediaImagePaths();
    void configureSetups();
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
	PlatformDialog *platformDialog;
    MediaTypeDialog *mediaTypeDialog;
    MediaImagePathMainDialog *mediaImagePathDialog;
    SetupMainDialog *setupMainDialog;
    ExecutableMainDialog *executableMainDialog;
    //TmpFolderEditDialog *tmpFolderDialog;
	QMenu *configMenu;
    QMenu *fileMenu;
    QMenu *helpMenu;
    QAction *configPlatformAction;
    QAction *configMediaTypeAction;
    QAction *configMediaImagePathAction;
    QAction *configSetupAction;
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
