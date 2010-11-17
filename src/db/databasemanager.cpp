// EmuFront
// Copyright 2010 Mikko Keinänen
//
// This file is part of EmuFront.
//
//
// EmuFront is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as published by
// the Free Software Foundation and appearing in the file gpl.txt included in the
// packaging of this file.
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
#include <QDateTime>

const QString DatabaseManager::DB_FILENAME = QString("emufront.db.sqlite");
const QString DatabaseManager::DATABASE = QString("QSQLITE");
const QString DatabaseManager::DB_TABLE_NAME_MEDIATYPE = QString("mediatype");
const QString DatabaseManager::DB_TABLE_NAME_PLATFORM = QString("platform");
const QString DatabaseManager::DB_TABLE_NAME_FILE= QString("file");
const QString DatabaseManager::DB_TABLE_NAME_FILEPATH = QString("filepath");
const QString DatabaseManager::DB_TABLE_NAME_SETUP = QString("setup");
const QString DatabaseManager::DB_TABLE_MEDIAIMAGECONTAINER = QString("mediaimagecontainer");
const QString DatabaseManager::DB_TABLE_MEDIAIMAGECONTAINER_MEDIAIMAGE = QString("mediaimagecontainer_mediaimage");
const QString DatabaseManager::DB_TABLE_EXECUTABLE = QString("executable");

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

/*
 You may wanna set the possible filters (filterDataObjects) before calling getDataModel.
 After filtering do not set update to true. Data model is already updated.
*/
QSqlQueryModel* DatabaseManager::getDataModel(bool update)
{
    if (!sqlTableModel) {
        sqlTableModel = getData();
    }
    else if (update)
        clearFilters();
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


/* Throws EmuFrontException if filtered data was not found. */
EmuFrontObject* DatabaseManager::getDataObject(int id)
{
    filterById(id);
    return getFilteredDataObject();
}

/* Throws EmuFrontException if filtered data was not found. */
EmuFrontObject* DatabaseManager::getDataObject(QString filter)
{
    QList<QString> filters;
    filters.append(filter);
    filterDataObjects(filters);
    return getFilteredDataObject();
}

/* Throws EmuFrontException if filtered data was not found. */
EmuFrontObject* DatabaseManager::getFilteredDataObject()
{
    EmuFrontObject *plf = 0;
    // TODO: if record has more than one the first instance is returned
    // ... check if this is ok in all cases!
    if (sqlTableModel->rowCount() >= 1)
    {
        QSqlRecord record = sqlTableModel->record(0);
        if (record.isEmpty()) {
            throw EmuFrontException(tr("No filtered data available"));
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

int DatabaseManager::getCurrentTimeStamp() {
    return QDateTime::currentDateTime().toTime_t();
}

int DatabaseManager::countDataObjectRefs(int id) const
{
    int ret = 0;
    QSqlQuery q;
    q.prepare(getCountRefsSelect(id));
    q.exec();
    QSqlRecord rec;
    if (q.next()) {
        rec = q.record();
        ret = rec.value(0).toInt();
    }
    qDebug() << "Found " << ret << " references.";
    return ret;
}
