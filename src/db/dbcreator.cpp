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
const QString DbCreator::TABLES[] = {"platform", "mediatype", "filetype"};

DbCreator::DbCreator(QObject *parent) : QObject(parent)
{
}


bool DbCreator::createDB()
{
    bool ret = false;
    QSqlQuery query;

    try
    {
        /*if (!tableExists("platform"))
        {*/
            qDebug() << "Creating table platform";
            ret = query.exec("create table if not exists platform "
                             "(id integer primary key, "
                             "name varchar(30), "
                             "filename varchar(125))");
            if (!ret) throw QString("platform.");
        /*}
        if (!tableExists("mediatype"))
        {*/
            qDebug() << "Creating table mediatype ";
            ret = query.exec("create table if not exists mediatype "
                             "(id integer primary key, "
                             "name varchar(30), "
                             "filename varchar(125))");
            if (!ret) throw QString("mediatype.");
        /*}
        if (!tableExists("filetype"))
        {*/
            /*qDebug() << "Creating table filetype";
            ret = query.exec("create table filetype if not exists"
                             "(id integer primary key, "
                             "name varchar(30))");
            if (!ret) throw QString("filetype.");
            query.exec("insert into filetype (id, name) values (1, 'media image container')");
            query.exec("insert into filetype (id, name) values (2, 'screenshot')");
            query.exec("insert into filetype (id, name) values (3, 'platform icon')");
            query.exec("insert into filetype (id, name) values (4, 'media type icon')");*/
        /*}
        if (!tableExists("filepath"))
        {*/
            qDebug() << "Creating table filepath";
            query.exec("create table filepath "
                       "(id integer primary key, "
                       "name text, "
                       "filetypeid integer, "
                       "platformid integer, "
                       "mediatypeid integer, "
                       "lastscanned numeric)");

            if (!ret) throw QString("filepath");
        //}
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
