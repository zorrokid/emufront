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

#include "databasemanager.h"
#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QFile>
#include <QDir>
#include <QVariant>
#include <QDebug>

const QString DatabaseManager::DB_FILENAME = QString("my.db.sqlite");
const QString DatabaseManager::DATABASE = QString("QSQLITE");
const QString DatabaseManager::DB_TABLE_NAME_MEDIATYPE = QString("mediatype");
const QString DatabaseManager::DB_TABLE_NAME_PLATFORM = QString("platform");
const QString DatabaseManager::DB_TABLE_NAME_FILE= QString("file");
const QString DatabaseManager::DB_TABLE_NAME_FILEPATH = QString("filepath");
const QString DatabaseManager::DB_TABLE_NAME_SETUP = QString("setup");
const QString DatabaseManager::DB_TABLE_MEDIAIMAGECONTAINER = QString("mediaimagecontainer");
const QString DatabaseManager::DB_TABLE_MEDIAIMAGECONTAINER_MEDIAIMAGE = QString("mediaimagecontainer_mediaimage");

DatabaseManager::DatabaseManager(QObject *parent)
	: QObject(parent)
{
    sqlTableModel = 0;
}

DatabaseManager::~DatabaseManager()
{
    // no need to explicitily destroy sqlTableModel
    // because it is parented QObject and will
    // be destroyed when parent is destroyed
}

QSqlQueryModel* DatabaseManager::getDataModel()
{
    if (!sqlTableModel) sqlTableModel = getData();
    return sqlTableModel;
}

bool DatabaseManager::openDB()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DatabaseManager::getDbPath());
    return db.open();
}

QString DatabaseManager::getDbPath()
{
	QString path;
#ifdef Q_OS_LINUX
	path.append(QDir::home().path());
	path.append(QDir::separator()).append(DB_FILENAME);
#else
	path.append(DB_FILENAME);	
#endif
	return path;
}

void DatabaseManager::resetModel()
{
    if (!sqlTableModel) return;
    clearFilters();
}

// sql must return a count(*) value
int DatabaseManager::countRows(QString tableName, QString columnName, int id) const
{
    QString sql = QString("SELECT COUNT(*) FROM %1 WHERE %2 = %3")
        .arg(tableName).arg(columnName).arg(id);
    int numEntries = 0;
    QSqlQuery query(sql);
    if (query.next())
        numEntries = query.value(0).toInt();
    return numEntries;
}

EmuFrontObject* DatabaseManager::getDataObject(int id)
{
    filterById(id);
    EmuFrontObject *plf = 0;
    if (sqlTableModel->rowCount() == 1)
    {
        QSqlRecord record = sqlTableModel->record(0);
        if (record.isEmpty())
        {
            throw new EmuFrontException(tr("No data available for id %1").arg(id));
        }
        else plf = recordToDataObject(&record);
    }
    return plf;
}

EmuFrontObject* DatabaseManager::getDataObjectFromModel(QModelIndex *index)
{
    if (!sqlTableModel) sqlTableModel = getDataModel();
    QSqlRecord record = sqlTableModel->record(index->row());
    return recordToDataObject(&record);
}
