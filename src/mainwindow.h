// EmuFront
// Copyright Mikko Keinänen 2010
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class PlatformDialog;
class QLabel;
class DatabaseManager;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
        //~MainWindow()

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void configurePlatforms();

private:
	void createActions();
	void createMenus();
	void createStatusBar();
	void readSettings();
	void writeSettings();
	bool okToContinue();
	PlatformDialog *platformDialog;
	QMenu *configMenu;
	QMenu *fileMenu;
        QAction *configPlatformAction;	
	QAction *exitAction;
	QLabel *messageLabel;
        DatabaseManager *dbManager;
};

#endif
