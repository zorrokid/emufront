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

#include "databasemanager.h"
#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QFile>
#include <QDir>
#include <QVariant>
#include <iostream>

const QString DatabaseManager::DB_FILENAME = QString("my.db.sqlite");

DatabaseManager::DatabaseManager(QObject *parent)
	: QObject(parent)
{}

DatabaseManager::~DatabaseManager()
{}

bool DatabaseManager::openDB()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DatabaseManager::getDbPath());
    return db.open();
}

/*QSqlError DatabaseManager::lastError()
{
	return db.lastError();
}*/

/*bool DatabaseManager::deleteDB()
{
	db.close();
	return QFile::remove(getDbPath());
}*/

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

/**
 * Check if database already exists.
 * Returns false if doesn't or we don't have a connection.
*/ 
bool DatabaseManager::dbExists()
{
	QSqlQuery query;
    query.exec("SELECT name FROM sqlite_master WHERE name='platform'");
    return query.next();
}

bool DatabaseManager::createDB()
{
	bool ret = false;
    QSqlQuery query;

    ret = query.exec("create table platform "
                     "(id integer primary key, "
                     "name varchar(30), "
                     "filename varchar(125))");
    /*ret = query.exec("create table media "
						"(id integer primary key, "
						"name varchar(30), "
						"filename varchar(125))");*/
	return ret;
}

void DatabaseManager::resetModel()
{
    if (!sqlTableModel) return;
    sqlTableModel->setFilter("");
    sqlTableModel->select();
}
