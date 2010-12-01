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
// TODO: deprecated
#include "dialogs/platformdialog.h"
#include "dialogs/platformmaindialog.h"
#include "dialogs/mediatypedialog.h"
// TODO: deprecated
#include "dialogs/mediatypemaindialog.h"
#include "dialogs/mediaimagepathmaindialog.h"
#include "dialogs/setupmaindialog.h"
#include "dialogs/executablemaindialog.h"
#include "utils/datfileutil.h"
#include "db/databasemanager.h"
#include "db/dbcreator.h"
#include "db/dbconfig.h"

QString MainWindow::aboutStr = trUtf8(
        "<h2>EmuFront</h2>"
        "<p>&copy; 2010 Mikko Keinänen</p>"
        "<p>mikko.keinanen@gmail.com</p>"
        "<p>EmuFront is free software: you can redistribute it and/or modify "
        "it under the terms of the GNU General Public License version 2 as published by "
        "the Free Software Foundation.</p>"
);

QString MainWindow::aboutTitle = tr("About EmuFront");

MainWindow::MainWindow(bool reset)
{
    if (!testDB(reset)) close();
    errorMessage = new QErrorMessage(this);
    setWindowTitle("EmuFront");
    tmpDirFilePath = DbConfig::getTmpDir();
    if (tmpDirFilePath.isEmpty())
        tmpDirFilePath = QDir::homePath();
    qDebug() << "Temporary dir is " << tmpDirFilePath;
    launcher = new EmuLauncher(errorMessage, this, tmpDirFilePath);
    setCentralWidget(launcher);
    createActions();
    createMenus();
    createStatusBar();
    readSettings();
    // TODO: deprecated
    platformDialog = 0;
    plfDialog = 0;
    // TODO: deprecated
    mediaTypeDialog = 0;
    mdtDialog = 0;
    mediaImagePathDialog = 0;
    setupMainDialog = 0;
    executableMainDialog = 0;
}

void MainWindow::connectSignals()
{
}

void MainWindow::createActions()
{
    // TODO: deprecated
    configPlatformAction = new QAction(tr("&Platforms"), this);
    configPlatformAction->setStatusTip(tr("Configure platforms"));
    connect(configPlatformAction, SIGNAL(triggered()),
        this, SLOT(configurePlatforms()));

    configPlatformsAction = new QAction(tr("&Set Platforms"), this);
    configPlatformsAction->setStatusTip(tr("Add, edit and delete platforms"));
    connect(configPlatformsAction, SIGNAL(triggered()),
        this, SLOT(configurePlatformss()));

    // TODO: deprecated
    configMediaTypeAction = new QAction(tr("&Media Types"), this);
    configMediaTypeAction->setStatusTip(tr("Configure media types"));
    connect(configMediaTypeAction, SIGNAL(triggered()), this, SLOT(configureMediaTypes()));

    configMediaTypesAction = new QAction(tr("&Set Media Types"), this);
    configMediaTypeAction->setStatusTip(tr("Add, edit and delete media types"));
    connect(configMediaTypesAction, SIGNAL(triggered()), this, SLOT(configureMediaTypess()));

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

    manageDatFilesAction = new QAction(tr("&Manage dats"), this);
    manageDatFilesAction->setStatusTip(tr("Read dat files to database."));
    connect(manageDatFilesAction, SIGNAL(triggered()), this, SLOT(manageDatFiles()));

    exitAction = new QAction(tr("&Exit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit EmuFront"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    resetDbAction = new QAction( tr("Reset database"), this);
    resetDbAction->setStatusTip(tr("Deletes all the current data and create a new database."));
    connect(resetDbAction, SIGNAL(triggered()), this, SLOT(resetDb()));

    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setStatusTip(tr("About EmuFront"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

// TODO: deprecated
void MainWindow::configurePlatforms()
{
   if (!platformDialog)
   {
       platformDialog = new PlatformDialog(this);
       connect(platformDialog, SIGNAL(finished(int)), this, SLOT(updateData()));
   }
   activateDialog(platformDialog);
}

void MainWindow::configurePlatformss()
{
    if (!plfDialog) {
        plfDialog = new PlatformMainDialog(this);
        connect(plfDialog, SIGNAL(finished(int)), this, SLOT(updateData()));
    }
    activateDialog(plfDialog);
}

// TODO: deprecated
void MainWindow::configureMediaTypes()
{
    if (!mediaTypeDialog)
    {
        mediaTypeDialog = new MediaTypeDialog(this);
        connect(mediaTypeDialog, SIGNAL(finished(int)), this, SLOT(updateData()));
   }
   activateDialog(mediaTypeDialog);
}

void MainWindow::configureMediaTypess()
{
    if (!mdtDialog)
    {
        mdtDialog = new MediaTypeMainDialog(this);
        connect(mdtDialog, SIGNAL(finished(int)), this, SLOT(updateData()));
   }
   activateDialog(mdtDialog);
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

void MainWindow::resetDb()
{
    if (QMessageBox::question(this, "Reset database?",
        "Are you REALLY SURE you want to do this? "
        "All the current data WILL BE LOST!",
        QMessageBox::No,
        QMessageBox::Yes) == QMessageBox::No) {
        return;
    }
    try {
        createDB();
    }
    catch (EmuFrontException e) {
        errorMessage->showMessage(e.what());
    }
}

void MainWindow::manageDatFiles()
{
    DatFileUtil dfu;
    dfu.open();
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
    fileMenu->addAction(resetDbAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    configMenu = menuBar()->addMenu(tr("&Config"));
    configMenu->addAction(configTmpDirAction);
    configMenu->addSeparator();
    // TODO: deprecated
    configMenu->addAction(configPlatformAction);
    configMenu->addAction(configPlatformsAction);
    // TODO: deprecated
    configMenu->addAction(configMediaTypeAction);
    configMenu->addAction(configMediaTypesAction);
    configMenu->addAction(configSetupAction);
    configMenu->addAction(configMediaImagePathAction);
    configMenu->addAction(configEmulatorAction);
    configMenu->addSeparator();
    configMenu->addAction(manageDatFilesAction);

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
    QMessageBox::about(this, aboutTitle, aboutStr);
}

bool MainWindow::testDB(bool reset)
{
    try {
        if (DatabaseManager::openDB()) {
            qDebug() << " Database opened succesfully!";
        }
        else {
            throw EmuFrontException("Database connection failed!");
        }

        int dbVer = DbCreator::dbExists();
        if (dbVer == 0) reset = true;
        if (!reset && dbVer != DbCreator::DB_VERSION) {
            QString msg("Database is not compatible "
                        "with current version of EmuFront!"
                        "Do you want to continue to recreate the database?"
                        "ALL THE CURRENT DATA WILL BE LOST!!!");
            if (QMessageBox::question(this, "Database not compatible!", msg,
                                      QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
                reset = true;
            }
            else throw EmuFrontException("The current database is not compatible!"
                                         " Cannot continue.");
        }

        if (reset) {
            createDB();
        }
        return true;
    }
    catch (EmuFrontException e) {
        qDebug() << e.what();
        errorMessage->showMessage(e.what());
        return false;
    }
}

void MainWindow::createDB() const
{
    try
    {
        DbCreator dbCreator;
        dbCreator.createDB();
    }
    catch (QString str) {
        QString msg(tr("Exception while trying to create"
                       " EmuFront database: %s").arg(str));
        errorMessage->showMessage(msg);
    }
}

