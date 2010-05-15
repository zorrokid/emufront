#include <QApplication>
#include <QTextStream>
#include <iostream>
#include "mainwindow.h"
#include "db/databasemanager.h"
//#include "dialogs/platformnamedialog.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QTextStream cout(stdout, QIODevice::WriteOnly);

    if (DatabaseManager::openDB())
        cout << " Database opened succesflly!" << endl;
	else cout << " Database connection failed!" << endl;

    if (DatabaseManager::dbExists())
		cout << " Database exists!" << endl;
	else 
	{
		cout << " Database is missing!" << endl;
        if (DatabaseManager::createDB())
			cout << " Database created succesfully!" << endl;
		else {
			cout << "Failed creating database!" << endl;
			exit(1);
		}
	 }
     /*

	if (dbm.insertPerson("Testi","Tapaus",1) > 0)
		cout << "Database insert successfull!" << endl;
	else cout << "Database insert failed!" << endl;


	cout << "Trying to select from database: " << dbm.getName(1) << endl;
	*/

	/*PlatformNameDialog *nameDialog = new PlatformNameDialog;
	nameDialog->show();*/
	MainWindow *mw = new MainWindow;
	mw->show();
	return app.exec();
}
