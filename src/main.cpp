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
