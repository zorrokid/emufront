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

#include <QtGui>
#include "mainwindow.h"
#include "emulauncher.h"
#include "dialogs/platformdialog.h"
#include "dialogs/mediatypedialog.h"
#include "dialogs/mediaimagepathmaindialog.h"
#include "dialogs/setupmaindialog.h"
#include "dialogs/executablemaindialog.h"
#include "db/databasemanager.h"
#include "db/dbconfig.h"

QString MainWindow::aboutStr = tr(
        "<h2>EmuFront</h2>"
        "<p>&copy; 2010 Mikko Keinänen</p>"
        "<p>mikko.keinanen@gmail.com</p>"
        "<p>EmuFront is free software: you can redistribute it and/or modify "
        "it under the terms of the GNU General Public License version 2 as published by "
        "the Free Software Foundation.</p>"
);

QString MainWindow::aboutTitle = tr("About EmuFront");

MainWindow::MainWindow()
{
    setWindowTitle("EmuFront");
    tmpDirFilePath = DbConfig::getTmpDir();
    if (tmpDirFilePath.isEmpty())
        tmpDirFilePath = QDir::homePath();
    qDebug() << "Temporary dir is " << tmpDirFilePath;
    launcher = new EmuLauncher(this, tmpDirFilePath);
    setCentralWidget(launcher);
    createActions();
    createMenus();
    createStatusBar();
    readSettings();
    platformDialog = 0;
    mediaTypeDialog = 0;
    mediaImagePathDialog = 0;
    setupMainDialog = 0;
    executableMainDialog = 0;
}

void MainWindow::connectSignals()
{
}

void MainWindow::createActions()
{
    configPlatformAction = new QAction(tr("&Platforms"), this);
    configPlatformAction->setStatusTip(tr("Configure platforms"));
    connect(configPlatformAction, SIGNAL(triggered()),
	    this, SLOT(configurePlatforms()));

    configMediaTypeAction = new QAction(tr("&Media Types"), this);
    configMediaTypeAction->setStatusTip(tr("Configure media types"));
    connect(configMediaTypeAction, SIGNAL(triggered()), this, SLOT(configureMediaTypes()));

    configMediaImagePathAction = new QAction(tr("Media &Image Paths"), this);
    configMediaImagePathAction->setStatusTip(tr("Configure media image file paths."));
    connect(configMediaImagePathAction, SIGNAL(triggered()),
        this, SLOT(configureMediaImagePaths()));

    configSetupAction = new QAction(tr("S&etups"), this);
    configSetupAction->setStatusTip(tr("Configure set ups"));
    connect(configSetupAction, SIGNAL(triggered()), this, SLOT(configureSetups()));

    configEmulatorAction = new QAction(tr("Em&ulators"), this);
    configEmulatorAction->setStatusTip(tr("Configure emulators"));
    connect(configEmulatorAction, SIGNAL(triggered()), this, SLOT(configureEmulators()));

    configTmpDirAction = new QAction(tr("&Temp dir"), this);
    configTmpDirAction->setStatusTip(tr("Configure directory for temporary files."));
    connect(configTmpDirAction, SIGNAL(triggered()), this, SLOT(configureTmpDir()));

    exitAction = new QAction(tr("&Exit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit EmuFront"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setStatusTip(tr("About EmuFront"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::configurePlatforms()
{
   if (!platformDialog)
   {
       platformDialog = new PlatformDialog(this);
       connect(platformDialog, SIGNAL(finished(int)), this, SLOT(updateData()));
   }
   activateDialog(platformDialog);
}

void MainWindow::configureMediaTypes()
{
    if (!mediaTypeDialog)
    {
        mediaTypeDialog = new MediaTypeDialog(this);
        connect(mediaTypeDialog, SIGNAL(finished(int)), this, SLOT(updateData()));
   }
   activateDialog(mediaTypeDialog);
}

void MainWindow::configureMediaImagePaths()
{
    if (!mediaImagePathDialog)
    {
        mediaImagePathDialog = new MediaImagePathMainDialog(this);
    }
    activateDialog(mediaImagePathDialog);
}

void MainWindow::configureSetups()
{
    if (!setupMainDialog)
    {
        qDebug() << "MainWindow: Creating a setup main dialog.";
        setupMainDialog = new SetupMainDialog(this);
    }
    activateDialog(setupMainDialog);
    setupMainDialog->refreshDataModel();
}

void MainWindow::configureEmulators()
{
    if (!executableMainDialog) {
        executableMainDialog = new ExecutableMainDialog(this);
        connect(executableMainDialog, SIGNAL(finished(int)), this, SLOT(updateData()));
    }
    activateDialog(executableMainDialog);
    executableMainDialog->refreshDataModel();
}

void MainWindow::configureTmpDir()
{
    /*if (!tmpFolderDialog) {
        tmpFolderDialog = new TmpFolderEditDialog(this, tmpDirFilePath);
    }
    activateDialog(tmpFolderDialog);*/

    QString fpath = QFileDialog::getExistingDirectory(this,
        tr("Select a directory"), tmpDirFilePath,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QDir d(fpath);
    if (d.exists() && d.isReadable()) {
        tmpDirFilePath = fpath;
        DbConfig::setTmpDir(tmpDirFilePath);
        launcher->setTmpDirPath(tmpDirFilePath);
    }
}

void MainWindow::activateDialog(EmuFrontDialog* dia) const
{
    dia->show();
    dia->raise();
    dia->activateWindow();
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAction);

    configMenu = menuBar()->addMenu(tr("&Config"));
    configMenu->addAction(configPlatformAction);
    configMenu->addAction(configMediaTypeAction);
    configMenu->addAction(configMediaImagePathAction);
    configMenu->addAction(configSetupAction);
    configMenu->addAction(configEmulatorAction);
    configMenu->addAction(configTmpDirAction);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
}

void MainWindow::createStatusBar()
{
    messageLabel = new QLabel;
    statusBar()->addWidget(messageLabel);
}

void MainWindow::readSettings()
{
}

void MainWindow::writeSettings()
{
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (okToContinue())
	event->accept();
    else event->ignore();
}

bool MainWindow::okToContinue()
{
    return true;
}

void MainWindow::updateData()
{
    qDebug() << "MainWindow::updateData()";
    launcher->updateData();
}

void MainWindow::about()
{
    QMessageBox::about(this, aboutTitle, aboutStr );
}
