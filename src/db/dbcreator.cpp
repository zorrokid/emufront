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

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <exception>
#include "dbcreator.h"

using namespace std;

const int DbCreator::TABLES_COUNT = 3;
const QString DbCreator::TABLES[] = {"platform", "mediatype", "filepath", "mediaimagecontainer", "mediaimage", "mediaimagecontainer_mediaimage"};

DbCreator::DbCreator(QObject *parent) : QObject(parent)
{
}


bool DbCreator::createDB()
{
    bool ret = false;
    QSqlQuery query;

    try
    {
        query.exec("drop table if exists mediaimagecontainer_mediaimage");
        query.exec("drop table if exists mediaimage");
        query.exec("drop table if exists mediaimagecontainer");
        query.exec("drop table if exists filepath");
        query.exec("drop table if exists setup");
        query.exec("drop table if exists mediatype");
        query.exec("drop table if exists platform");

        qDebug() << "Creating table platform";

        ret = query.exec("create table if not exists platform "
                         "(id integer primary key, "
                         "name varchar(30), "
                         "filename varchar(125))");

        if (!ret) throw QString("platform.");

        qDebug() << "Creating table mediatype ";

        ret = query.exec("create table if not exists mediatype "
                         "(id integer primary key, "
                         "name varchar(30), "
                         "filename varchar(125))");

        if (!ret) throw QString("mediatype.");

        qDebug() << "Creating table setup";

        ret = query.exec("create table if not exists setup "
                        "(id integer primary key, "
                        "platformid integer, "
                        "mediatypeid integer, "
                        "filetypeextensions text, "
                        "foreign key (platformid) references platform(id), "
                        "foreign key (mediatypeid) references mediatype(id))");

        /*qDebug() << "Creating table filetype";
            ret = query.exec("create table filetype if not exists"
                             "(id integer primary key, "
                             "name varchar(30))");
            if (!ret) throw QString("filetype.");
            query.exec("insert into filetype (id, name) values (1, 'media image container')");
            query.exec("insert into filetype (id, name) values (2, 'screenshot')");
            query.exec("insert into filetype (id, name) values (3, 'platform icon')");
            query.exec("insert into filetype (id, name) values (4, 'media type icon')");*/

        qDebug() << "Creating table filepath";

        ret = query.exec("create table if not exists filepath "
                         "(id integer primary key, "
                         "name text, "
                         "filetypeid integer, "
                         "setupid integer, "
                         "lastscanned numeric, "
                         "foreign key (setupid) references setup(id))");

        if (!ret) throw QString("filepath");

        qDebug() << "Creating table mediaimagecontainer";

        ret = query.exec("create table if not exists mediaimagecontainer "
                        "(id integer primary key, "
                        "name text, "
                        "filename text, "
                        "sha1 text, "
                        "md5 text, "
                        "filepathid integer, "
                        "platformid integer, "
                        "mediatypeid integer, "
                        "updatetime numeric, "
                        "foreign key (filepathid) references filepath(id), "
                        "foreign key (platformid) references platform(id), "
                        "foreign key (mediatypeid) references mediatype(id))");

        if (!ret) throw QString("mediaimagecontainer");

        qDebug() << "Creating table mediaimage";

        ret = query.exec("create table if not exists mediaimage "
                        "(id integer primary key, "
                        "filename text, "
                        "sha1 text, "
                        "md5 text, "
                        "filesize integer, "
                        "updatetime numeric)");

        qDebug() << "Creating table mediaimagecontainer_mediaimage";

        ret = query.exec("create table if not exists mediaimagecontainer_mediaimage "
                        "(mediaimagecontainerid integer, "
                        "mediaimageid integer, "
                        "foreign key (mediaimagecontainerid) references mediaimagecontainer(id), "
                        "foreign key (mediaimageid) references mediaimage(id))");

        if (!ret) throw QString("mediaimagecontainer_mediaimage");
    }
    catch (QString tbl)
    {
        throw QString("Couldn't create database '%1'!").arg(tbl);
    }
    return ret;
}

/**
 * Check if database already exists.
 * Returns false if doesn't or we don't have a connection.
*/
bool DbCreator::dbExists()
{
    for (int i = 0; i < TABLES_COUNT; ++i)
    {
        if (!tableExists(TABLES[i]))
        {
            qDebug() << "Table " << TABLES[i] << " missing.";
            return false;
        }
       qDebug() << "Table " << TABLES[i] << " exists.";
    }
    return true;
}

bool DbCreator::tableExists(QString table)
{
    QSqlQuery query;
    query.exec(QString("SELECT name FROM sqlite_master WHERE name='%1'").arg(table));
    return query.next();
}

bool DbCreator::deleteDB()
{
    // return QFile::remove(getDbPath());
    return false;
}
