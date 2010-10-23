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

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <exception>
#include "dbcreator.h"

using namespace std;

const int DbCreator::TABLES_COUNT = 3;
const QString DbCreator::TABLES[] = {"platform", "mediatype", "filepath", "mediaimagecontainer_filepath", "mediaimage", "mediaimagecontainer_mediaimage"};

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
        query.exec("DROP TABLE IF EXISTS mediaimagecontainer_filepath");
        query.exec("DROP TABLE IF EXISTS filepath");
        query.exec("DROP TABLE IF EXISTS setup");
        query.exec("DROP TABLE IF EXISTS mediatype");
        query.exec("DROP TABLE IF EXISTS platform");
        query.exec("DROP TABLE IF EXISTS file");
        query.exec("DROP TABLE IF EXISTS executable");

        qDebug() << "Creating TABLE file";

        ret = query.exec("CREATE TABLE IF NOT EXISTS file"
                        "(id INTEGER PRIMARY KEY, "
                        "name TEXT, "
                        "type INTEGER, "
                        "checksum TEXT, "
                        "size INTEGER, "
                        "updatetime NUMERIC)");

        if (!ret) throw QString("tbl file");

        qDebug() << "Creating TABLE platform";

        ret = query.exec("CREATE TABLE IF NOT EXISTS platform "
                         "(id INTEGER PRIMARY KEY, "
                         "name TEXT, "
                         "fileid INTEGER REFERENCES file(id))");

        if (!ret) throw QString("tbl platform");

        qDebug() << "Creating TABLE mediatype ";

        ret = query.exec("CREATE TABLE IF NOT EXISTS mediatype "
                         "(id INTEGER PRIMARY KEY, "
                         "name TEXT, "
                         "fileid INTEGER REFERENCES file(id))");

        if (!ret) throw QString("tbl mediatype");

        qDebug() << "Creating TABLE setup";

        ret = query.exec("CREATE TABLE IF NOT EXISTS setup "
                        "(id INTEGER PRIMARY KEY, "
                        "platformid INTEGER REFERENCES platform(id) ON DELETE CASCADE, "
                        "mediatypeid INTEGER REFERENCES mediatype(id) ON DELETE CASCADE, "
                        "filetypeextensions TEXT)");

        if (!ret) throw QString("tbl setup");

        qDebug() << "Creating table executable";

        ret = query.exec("CREATE TABLE IF NOT EXISTS executable "
                        "(id INTEGER PRIMARY KEY, "
                        "name TEXT, "
                        "executable TEXT, "
                        "options TEXT, "
                        "type INTEGER, "
                        "setupid INTEGER REFERENCES setup(id))");

        if (!ret) throw QString("tbl executable");

        qDebug() << "Creating TABLE filepath";

        ret = query.exec("CREATE TABLE IF NOT EXISTS filepath "
                         "(id INTEGER PRIMARY KEY, "
                         "name TEXT, "
                         "filetypeid INTEGER, "
                         "setupid INTEGER, "
                         "lastscanned NUMERIC, "
                         "FOREIGN KEY (setupid) REFERENCES setup(id))");

        if (!ret) throw QString("tbl filepath");

        qDebug() << "Creating TABLE mediaimagecontainer_filepath";

        ret = query.exec("CREATE TABLE IF NOT EXISTS mediaimagecontainer_filepath "
                        "(fileid INTEGER REFERENCES file(id), "
                        "filepathid INTEGER REFERENCES filepath(id), "
                        "updatetime NUMERIC)");

        if (!ret) throw QString("tbl mediaimagecontainer_filepath");


        qDebug() << "Creating TABLE mediaimagecontainer_mediaimage";

        ret = query.exec("CREATE TABLE IF NOT EXISTS mediaimagecontainer_mediaimage "
                        "(mediaimagecontainerid INTEGER REFERENCES file(id), "
                        "mediaimageid INTEGER REFERENCES file(id))");

        if (!ret) throw QString("tbl mediaimagecontainer_mediaimage");

        query.exec(
            "CREATE TRIGGER IF NOT EXISTS trg_onplatformdelete "
            "AFTER DELETE ON platform "
            "BEGIN "
            "   DELETE FROM setup WHERE setup.platformid = old.id;"
            "END;"
            );

        if (!ret) throw QString("trg_onplatformdelete");

        query.exec(
            "CREATE TRIGGER IF NOT EXISTS trg_onmediatypedelete "
            "AFTER DELETE ON mediatype "
            "BEGIN "
            "   DELETE FROM setup WHERE setup.mediatypeid = old.id;"
            "END;"
            );

        if (!ret) throw QString("trg_onmediatypedelete");

        query.exec(
            "CREATE TRIGGER IF NOT EXISTS trg_onsetupdelete "
            "AFTER DELETE ON setup "
            "BEGIN "
            "   DELETE FROM filepath WHERE filepath.setupid = old.id; "
            "   DELETE FROM executable WHERE executable.setupid = old.id; "
            "END;"
            );

        if (!ret) throw QString("trg_onsetupdelete");

        query.exec(
            "CREATE TRIGGER IF NOT EXISTS trg_onfilepathdelete "
            "AFTER DELETE ON filepath "
            "BEGIN "
            "   DELETE FROM mediaimagecontainer_filepath WHERE mediaimagecontainer_filepath.filepathid=old.id; "
            "END;"
        );

        if (!ret) throw QString("trg_onfilepathdelete");

        query.exec(
            "CREATE TRIGGER IF NOT EXISTS trg_onmediaimagecontainerdelete "
            "AFTER DELETE ON mediaimagecontainer_filepath "
            "BEGIN "
            "   DELETE FROM mediaimagecontainer_mediaimage WHERE mediaimagecontainer_mediaimage.mediaimagecontainerid=old.fileid;"
            "END;"
        );

        if (!ret) throw QString("trg_onmediaimagecontainerdelete");

        query.exec(
            "CREATE TRIGGER IF NOT EXISTS trg_onmediaimagecontainer_mediaimagedelete "
            "AFTER DELETE ON mediaimagecontainer_mediaimage "
            "BEGIN "
            "    DELETE FROM file WHERE file.id=old.mediaimageid; "
            "    DELETE FROM file WHERE file.id=old.mediaimagecontainerid; "
            "END;"
        );
        if (!ret) throw QString("trg_onmediaimagecontainer_mediaimagedelete");

    }
    catch (QString tbl)
    {
        QString err = query.lastError().text();
        throw QString("Couldn't CREATE '%1'!").arg(tbl).append(err);
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
