/*
** EmuFront
** Copyright 2010 Mikko Keinänen
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

#include "emufrontfile.h"
#include "filemodel.h"
#include <QtSql>

FileModel::FileModel(QObject *parent) :
    EmuFrontQueryModel(parent)
{
}

QString FileModel::constructSelect(QString where) const
{
    return QString(
            "SELECT file.id AS FileId, "
            "file.name AS Name, "
            "file.type AS FileType, "
            "file.checksum AS Checksum, "
            "file.size AS FileSize, "
            "file.updatetime AS UpdateTime "
            "FROM file "
            "%1 "
            "ORDER BY Name"
        ).arg(where);
}

QString FileModel::constructFilterById(int id) const
{
    return QString("file.id = %1").arg(id);
}

EmuFrontObject* FileModel::recordToDataObject(const QSqlRecord *record)
{
    int id = record->value(File_Id).toInt();
    QString name = record->value(File_Name).toString();
    QString checksum = record->value(File_CheckSum).toString();
    int size = record->value(File_FileSize).toInt();
    int type = record->value(File_FileType).toInt();
    return new EmuFrontFile(id, name, checksum, size, type);
}
