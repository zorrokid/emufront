/*
** EmuFront
** Copyright 2010-2011 Mikko Keinänen
**
** This file is part of EmuFront.
**
**
** EmuFront is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License version 2 as published by
** the Free Software Foundation and appearing in the file gpl.txt included in the
** packaging of this file.
**
** EmuFront is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with EmuFront.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "dbconfig.h"
#include <QSqlRelationalTableModel>
#include <QSqlQuery>
#include <QSqlRecord>

DbConfig::DbConfig(QObject *parent) :
    QObject(parent)
{
}

QString DbConfig::getTmpDir()
{
    QString r = "";
    QSqlQuery q("SELECT tmpdirpath FROM config");
    if (q.first())
        r = q.value(DbConfig::DbConfig_TmpDirPath).toString();
    return r;
}

bool DbConfig::setTmpDir(QString path)
{
    QSqlQuery q("SELECT COUNT(*) FROM config");
    int rows = 0;
    if (q.first())
        rows = q.value(0).toInt();
    if (rows == 0)
        q.prepare("INSERT INTO config (tmpdirpath) "
        "VALUES (:path)");
    else
        q.prepare("UPDATE CONFIG SET tmpdirpath=:path");
    q.bindValue(":path", path);
    return q.exec();
}
