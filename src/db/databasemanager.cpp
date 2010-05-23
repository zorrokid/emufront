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

#include "databasemanager.h"
#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QFile>
#include <QDir>
#include <QVariant>

const QString DatabaseManager::DB_FILENAME = QString("my.db.sqlite");
const QString DatabaseManager::DATABASE = QString("QSQLITE");

DatabaseManager::DatabaseManager(QObject *parent)
	: QObject(parent)
{}

DatabaseManager::~DatabaseManager()
{
    // no need to explicitily destroy sqlTableModel
    // because it is parented QObject and will
    // be destroyed when parent is destroyed
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

void DatabaseManager::resetModel() const
{
    if (!sqlTableModel) return;
    sqlTableModel->setFilter("");
    sqlTableModel->select();
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
