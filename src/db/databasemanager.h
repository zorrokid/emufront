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

#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>

class QSqlError;
class QFile;
class QSqlTableModel;

class DatabaseManager : public QObject
{
public:
	DatabaseManager(QObject *parent = 0);
	~DatabaseManager();

    virtual QSqlTableModel* getDataModel() = 0;
    static bool openDB();
    static bool deleteDB();
    static bool dbExists();
    static QSqlError lastError();
    static bool createDB();
    void resetModel();

protected:
    QSqlTableModel *sqlTableModel;

private:
	static const QString DB_FILENAME;
    virtual QSqlTableModel* getData() = 0;
    static QString getDbPath();
};

#endif
