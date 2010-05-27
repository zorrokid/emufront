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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class PlatformDialog;
class MediaTypeDialog;
class MediaImagePathMainDialog;
class QLabel;
class DatabaseManager;
class EmuFrontDialog;

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
    void configureMediaTypes();
    void configureMediaImagePaths();

private:
	void createActions();
	void createMenus();
	void createStatusBar();
	void readSettings();
	void writeSettings();
	bool okToContinue();
    void activateDialog(EmuFrontDialog*) const;
	PlatformDialog *platformDialog;
    MediaTypeDialog *mediaTypeDialog;
    MediaImagePathMainDialog *mediaImagePathDialog;
	QMenu *configMenu;
	QMenu *fileMenu;
    QAction *configPlatformAction;
    QAction *configMediaTypeAction;
    QAction *configMediaImagePathAction;
    QAction *exitAction;
	QLabel *messageLabel;
    DatabaseManager *dbManager;
};

#endif
