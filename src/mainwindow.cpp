// EmuFront
// Copyright 2010 Mikko Keinänen
//
// This file is part of EmuFront.
//
//
// EmuFront is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Foobar is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

#include <QtGui>
#include "mainwindow.h"
#include "dialogs/platformdialog.h"
#include "dialogs/mediatypedialog.h"
#include "dialogs/mediaimagepathmaindialog.h"
#include "db/databasemanager.h"

MainWindow::MainWindow()
{
    setWindowTitle("EmuFront");
    createActions();
    createMenus();
    createStatusBar();
    readSettings();
    platformDialog = 0;
    mediaTypeDialog = 0;
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

    exitAction = new QAction(tr("&Exit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit EmuFront"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::configurePlatforms()
{
   if (!platformDialog)
   {
       platformDialog = new PlatformDialog(this);
   } 
   activateDialog(platformDialog);
}

void MainWindow::configureMediaTypes()
{
    if (!mediaTypeDialog)
    {
        mediaTypeDialog = new MediaTypeDialog(this);
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
