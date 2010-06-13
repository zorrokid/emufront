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

#include <QApplication>
#include <QTextStream>
#include <QDebug>
#include <iostream>
#include "mainwindow.h"
#include "db/databasemanager.h"
#include "db/dbcreator.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
    QTextStream cout(stdout, QIODevice::WriteOnly);
    QStringList arglst = app.arguments();

    bool reset = arglst.contains("reset", Qt::CaseInsensitive);


    if (reset) qDebug() << "Database Reset requested";

    if (DatabaseManager::openDB())
        cout << " Database opened succesfully!" << endl;
	else cout << " Database connection failed!" << endl;

    if (reset || !DbCreator::dbExists())
	{
        try
        {
            cout << " Database is missing!" << endl;
            DbCreator dbCreator;
            dbCreator.createDB();
            cout << " Database created succesfully!" << endl;
        }
        catch (QString str) {
            cout << str << endl;
			exit(1);
		}
    }
    else cout << " Database exists!" << endl;

    MainWindow *mw = new MainWindow;
	mw->show();
	return app.exec();
}
