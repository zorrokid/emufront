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
