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
// EmuFront is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with EmuFront.  If not, see <http://www.gnu.org/licenses/>.

#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include "../exceptions/emufrontexception.h"

class QSqlError;
class QFile;
class QSqlQueryModel;
class QModelIndex;
class EmuFrontObject;

class DatabaseManager : public QObject
{
public:
	DatabaseManager(QObject *parent = 0);
	~DatabaseManager();

    QSqlQueryModel* getDataModel();
    EmuFrontObject* getDataObjectFromModel(QModelIndex*);
    EmuFrontObject* getDataObject(int id);
    virtual bool updateDataObjectToModel(const EmuFrontObject*) = 0;
    virtual bool insertDataObjectToModel(const EmuFrontObject*) = 0;
    virtual bool deleteDataObjectFromModel(QModelIndex*) = 0;
    virtual int countDataObjectRefs(int id) const = 0;
    static bool openDB();
    void resetModel();
    enum {
        Filetype_MediaImageContainer = 0,
        Filetype_Screenshot = 1,
        Filetype_PlatformIcon = 2,
        Filetype_MediaTypeIcon = 3 };

protected:
    QSqlQueryModel* sqlTableModel;
    virtual EmuFrontObject* recordToDataObject(const QSqlRecord* ) = 0;
    virtual void filterById(int id) = 0;
    virtual void clearFilters() = 0;
    int countRows(QString tableName, QString columnName, int id) const;
    static const QString DB_TABLE_NAME_FILE;
    static const QString DB_TABLE_NAME_FILEPATH;
    static const QString DB_TABLE_NAME_MEDIATYPE;
    static const QString DB_TABLE_NAME_PLATFORM;
    static const QString DB_TABLE_NAME_SETUP;
    static const QString DB_TABLE_MEDIAIMAGECONTAINER;
    static const QString DB_TABLE_MEDIAIMAGECONTAINER_MEDIAIMAGE;


private:
	static const QString DB_FILENAME;
    static const QString DATABASE;
    virtual QSqlQueryModel* getData() = 0;
    static QString getDbPath();
};

#endif
