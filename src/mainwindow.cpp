#include <QtGui>
#include "mainwindow.h"
#include "dialogs/platformdialog.h"
#include "db/databasemanager.h"

MainWindow::MainWindow()
{
    setWindowTitle("EmuFront");
    createActions();
    createMenus();
    createStatusBar();
    readSettings();
    platformDialog = 0;
    //dbManager = new DatabaseManager;
}

void MainWindow::createActions()
{
    configPlatformAction = new QAction(tr("&Platforms"), this);
    configPlatformAction->setStatusTip(tr("Configure platforms"));
    connect(configPlatformAction, SIGNAL(triggered()),
	    this, SLOT(configurePlatforms()));

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
   platformDialog->show();
   platformDialog->raise();
   platformDialog->activateWindow();
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAction);

    configMenu = menuBar()->addMenu(tr("&Config"));
    configMenu->addAction(configPlatformAction);
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
