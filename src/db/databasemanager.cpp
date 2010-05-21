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

//QSqlDatabase DatabaseManager::db = QSqlDatabase::addDatabase("QSQLITE");

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
    /*if (db.isOpen())
    {*/
		QSqlQuery query;

		ret = query.exec("create table platform "
				"(id integer primary key, "
				"name varchar(30), "
				"filename varchar(125))");
		/*ret = query.exec("create table media "
						"(id integer primary key, "
						"name varchar(30), "
						"filename varchar(125))");*/
    //}
	return ret;
}

/*int DatabaseManager::insertPlatform(QString name, QString filename)
{
	int newId = -1;
    bool ret = false;*/
    /*if (db.isOpen())
    {*/
		//http://www.sqlite.org/autoinc.html
		// NULL = is the keyword for the autoincrement to generate next value
        /*QSqlQuery query;
		query.prepare("insert into platform (id, name, filename) "
						"values (NULL, :name, :filename)");
		query.bindValue(":name", name);
		query.bindValue(":filename", filename);
        ret = query.exec();*/

		/*ret = query.exec(QString("insert into person values(NULL,'%1','%2',%3)")
				.arg(firstname).arg(lastname).arg(age));*/
		// Get database given autoincrement value
        /*if (ret)
		{
			// http://www.sqlite.org/c3ref/last_insert_rowid.html  
			QVariant var = query.lastInsertId();
			if (var.isValid()) newId = var.toInt();
		}
    //}
	return newId;
}*/

/*QString DatabaseManager::getPlatform(int id)
{
	QString name;
	QSqlQuery query(QString("select firstname, lastname from person where id = %1").arg(id));

    if (query.next())
	{
		name.append(query.value(0).toString());
		name.append(query.value(1).toString());
	}
	return name;
}

QSqlTableModel* DatabaseManager::getPlatforms()
{
    QSqlTableModel *model = new QSqlTableModel(this);
    model->setTable(DB_TABLE_NAME_PLATFORM);
    model->setSort(Platform_Name, Qt::AscendingOrder);
    model->setHeaderData(Platform_Name, Qt::Horizontal, tr("Name"));
    model->select();
    return model;
}*/

