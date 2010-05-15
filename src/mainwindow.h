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

	// todo ks. s. 46 jatkoa...
};

#endif
