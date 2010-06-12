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
        query.exec("DROP TABLE IF EXISTS mediaimagecontainer_mediaimage");
        query.exec("DROP TABLE IF EXISTS mediaimage");
        query.exec("DROP TABLE IF EXISTS mediaimagecontainer");
        query.exec("DROP TABLE IF EXISTS filepath");
        query.exec("DROP TABLE IF EXISTS setup");
        query.exec("DROP TABLE IF EXISTS mediatype");
        query.exec("DROP TABLE IF EXISTS platform");

        qDebug() << "Creating TABLE platform";

        ret = query.exec("CREATE TABLE IF NOT EXISTS platform "
                         "(id INTEGER PRIMARY KEY, "
                         "name TEXT, "
                         "filename TEXT)");

        if (!ret) throw QString("platform.");

        qDebug() << "Creating TABLE mediatype ";

        ret = query.exec("CREATE TABLE IF NOT EXISTS mediatype "
                         "(id INTEGER PRIMARY KEY, "
                         "name TEXT, "
                         "filename TEXT)");

        if (!ret) throw QString("mediatype.");

        qDebug() << "Creating TABLE setup";

        ret = query.exec("CREATE TABLE IF NOT EXISTS setup "
                        "(id INTEGER PRIMARY KEY, "
                        "platformid INTEGER REFERENCES platform(id) ON DELETE CASCADE, "
                        "mediatypeid INTEGER REFERENCES mediatype(id) ON DELETE CASCADE, "
                        "filetypeextensions TEXT)");

        /*qDebug() << "Creating TABLE filetype";
            ret = query.exec("CREATE TABLE filetype IF NOT EXISTS"
                             "(id INTEGER PRIMARY KEY, "
                             "name TEXT)");
            if (!ret) throw QString("filetype.");
            query.exec("insert into filetype (id, name) values (1, 'media image container')");
            query.exec("insert into filetype (id, name) values (2, 'screenshot')");
            query.exec("insert into filetype (id, name) values (3, 'platform icon')");
            query.exec("insert into filetype (id, name) values (4, 'media type icon')");*/

        qDebug() << "Creating TABLE filepath";

        ret = query.exec("CREATE TABLE IF NOT EXISTS filepath "
                         "(id INTEGER PRIMARY KEY, "
                         "name TEXT, "
                         "filetypeid INTEGER, "
                         "setupid INTEGER, "
                         "lastscanned NUMERIC, "
                         "FOREIGN KEY (setupid) REFERENCES setup(id))");

        if (!ret) throw QString("filepath");

        qDebug() << "Creating TABLE mediaimagecontainer";

        ret = query.exec("CREATE TABLE IF NOT EXISTS mediaimagecontainer "
                        "(id INTEGER PRIMARY KEY, "
                        "name TEXT, "
                        "filename TEXT, "
                        "sha1 TEXT, "
                        "md5 TEXT, "
                        "filepathid INTEGER, "
                        "platformid INTEGER, "
                        "mediatypeid INTEGER, "
                        "updatetime NUMERIC, "
                        "FOREIGN KEY (filepathid) REFERENCES filepath(id), "
                        "FOREIGN KEY (platformid) REFERENCES platform(id), "
                        "FOREIGN KEY (mediatypeid) REFERENCES mediatype(id))");

        if (!ret) throw QString("mediaimagecontainer");

        qDebug() << "Creating TABLE mediaimage";

        ret = query.exec("CREATE TABLE IF NOT EXISTS mediaimage "
                        "(id INTEGER PRIMARY KEY, "
                        "filename TEXT, "
                        "sha1 TEXT, "
                        "md5 TEXT, "
                        "filesize INTEGER, "
                        "updatetime NUMERIC)");

        qDebug() << "Creating TABLE mediaimagecontainer_mediaimage";

        ret = query.exec("CREATE TABLE IF NOT EXISTS mediaimagecontainer_mediaimage "
                        "(mediaimagecontainerid INTEGER, "
                        "mediaimageid INTEGER, "
                        "FOREIGN KEY (mediaimagecontainerid) REFERENCES mediaimagecontainer(id), "
                        "FOREIGN KEY (mediaimageid) REFERENCES mediaimage(id))");

        if (!ret) throw QString("mediaimagecontainer_mediaimage");

        ret = query.exec(
            "CREATE TRIGGER IF NOT EXISTS trg_onplatformdelete "
            "AFTER DELETE ON platform "
            "BEGIN "
            "   DELETE FROM setup WHERE setup.platformid = old.id;"
            "END;"
            );

        ret = query.exec(
            "CREATE TRIGGER IF NOT EXISTS trg_onmediatypedelete "
            "AFTER DELETE ON mediatype"
            "BEGIN "
            "   DELETE FROM setup WHERE setup.mediatypeid = old.id;"
            "END;"
            );

        ret = query.exec(
            "CREATE TRIGGER IF NOT EXISTS trg_onsetupdelete "
            "AFTER DELETE ON setup"
            "BEGIN "
            "   DELETE FROM filepath WHERE filepath.setupid = old.id;"
            "END;"
            );
    }
    catch (QString tbl)
    {
        throw QString("Couldn't CREATE database '%1'!").arg(tbl);
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

bool DbCreator::tableExists(QString TABLE)
{
    QSqlQuery query;
    query.exec(QString("SELECT name FROM sqlite_master WHERE name='%1'").arg(TABLE));
    return query.next();
}

bool DbCreator::deleteDB()
{
    // return QFile::remove(getDbPath());
    return false;
}
