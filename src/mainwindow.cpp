/*
** EmuFront
** Copyright 2010-2011 Mikko Keinänen
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
*/

#include <QtGui>
#include "mainwindow.h"
#include "emulauncher.h"
#include "platformeditview.h"
#include "mediatypeeditview.h"
#include "filepatheditview.h"
#include "setupeditview.h"
#include "externalexecutableeditview.h"
#include "externalexecutablemodel.h"
#include "datfileutil.h"
#include "databasemanager.h"
#include "dbcreator.h"
#include "dbconfig.h"
#include "setupmodel.h"
#include "platformmodel.h"
#include "mediatypemodel.h"

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
    supModel = new SetupModel(this);
    plfModel = new PlatformModel(this);
    mdtModel = new MediaTypeModel(this);
	emuModel = new ExternalExecutableModel(this);
    launcher = new EmuLauncher(errorMessage, supModel, emuModel, this, tmpDirFilePath);
    setCentralWidget(launcher);
    createActions();
    createMenus();
    createStatusBar();
    readSettings();
    plfDialog = 0;
    mdtDialog = 0;
    mediaImagePathView = 0;
    setupMainView = 0;
    emulatorEditView = 0;
}

void MainWindow::connectSignals()
{
}

void MainWindow::createActions()
{
    configPlatformsAction = new QAction(tr("&Set Platforms"), this);
    configPlatformsAction->setStatusTip(tr("Add, edit and delete platforms"));
    connect(configPlatformsAction, SIGNAL(triggered()),
        this, SLOT(configurePlatforms()));

    configMediaTypesAction = new QAction(tr("&Set Media Types"), this);
    configMediaTypesAction->setStatusTip(tr("Add, edit and delete media types"));
    connect(configMediaTypesAction, SIGNAL(triggered()), this, SLOT(configureMediaTypes()));

    configMediaImagePathsAction = new QAction(tr("Set media &image paths"), this);
    configMediaImagePathsAction->setStatusTip(tr("Add, edit and delete media image file paths."));
    connect(configMediaImagePathsAction, SIGNAL(triggered()),
        this, SLOT(configureMediaImagePaths()));

    configSetupsAction = new QAction(tr("Configure S&etups"), this);
    configSetupsAction->setStatusTip(tr("Add, edit and delete setups"));
    connect(configSetupsAction, SIGNAL(triggered()), this, SLOT(configureSetups()));

    configEmulatorsAction = new QAction(tr("Configure Em&ulators"), this);
    configEmulatorsAction->setStatusTip(tr("Add, edit and delete emulator configurations"));
    connect(configEmulatorsAction, SIGNAL(triggered()), this, SLOT(configureEmulators()));

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

void MainWindow::configurePlatforms()
{
    if (!plfDialog) {
        plfDialog = new PlatformEditView(plfModel, this);
        connect(plfDialog, SIGNAL(finished(int)), this, SLOT(updateData()));
    }
    activateDialog(plfDialog);
}

void MainWindow::configureMediaTypes()
{
    if (!mdtDialog)
    {
        mdtDialog = new MediaTypeEditView(mdtModel, this);
        connect(mdtDialog, SIGNAL(finished(int)), this, SLOT(updateData()));
   }
   activateDialog(mdtDialog);
}

void MainWindow::configureMediaImagePaths()
{
    if (!mediaImagePathView)
    {
        mediaImagePathView = new FilePathEditView(supModel, this);
    }
    activateDialog(mediaImagePathView);
}

void MainWindow::configureSetups()
{
    if (!setupMainView) {
        setupMainView = new SetupEditView(plfModel, mdtModel, supModel, this);
    }
    activateDialog(setupMainView);
}

void MainWindow::configureEmulators()
{
    if (!emulatorEditView) {
        emulatorEditView = new ExternalExecutableEditView(supModel, emuModel, this);
    }
    activateDialog(emulatorEditView);
}

void MainWindow::configureTmpDir()
{
    QString fpath = QFileDialog::getExistingDirectory(this,
        tr("Select a directory"), tmpDirFilePath,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QDir d(fpath);
    if (d.exists() && d.isReadable()) {
        tmpDirFilePath = fpath;
        if (DbConfig::setTmpDir(tmpDirFilePath)) {
            QMessageBox::information(
                this,
                tr("Temp dir updated"),
                tr("The directory for temporary files "
                    "has been updated to '%1'.")
                    .arg(tmpDirFilePath)
            );
            launcher->setTmpDirPath(tmpDirFilePath);
        }
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
    configMenu->addAction(configPlatformsAction);
    configMenu->addAction(configMediaTypesAction);
    configMenu->addAction(configSetupsAction);
    configMenu->addAction(configMediaImagePathsAction);
    configMenu->addAction(configEmulatorsAction);
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

