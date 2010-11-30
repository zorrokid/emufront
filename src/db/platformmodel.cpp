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

#include <QtSql>
#include "platformmodel.h"

PlatformModel::PlatformModel(QObject *parent) :
    EmuFrontQueryModel(parent)
{
    tableName = "platform";
    refresh();
}

Qt::ItemFlags PlatformModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlQueryModel::flags(index);
    if (index.column() == EmuFrontFileObject_Name) {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}

bool PlatformModel::setData(const QModelIndex &index, const QVariant &value, int /*role*/)
{
    if(index.column() != EmuFrontFileObject_Name)
        return false;

    QModelIndex primaryKeyIndex
        = QSqlQueryModel::index(index.row(), EmuFrontFileObject_Id);

    int id = data(primaryKeyIndex).toInt();
    clear();

    bool ok;
    if (index.column() == EmuFrontFileObject_Name) {
        ok = setName(id, value.toString());
    }

    refresh();
    return ok;
}

void PlatformModel::refresh()
 {
     setQuery(constructSelect());
     setHeaderData(EmuFrontFileObject_Id, Qt::Horizontal, tr("ID"));
     setHeaderData(EmuFrontFileObject_Name, Qt::Horizontal, tr("Name"));
     setHeaderData(EmuFrontFileObject_FileId, Qt::Horizontal, tr("FileID"));
     setHeaderData(EmuFrontFileObject_FileName, Qt::Horizontal, tr("File Name"));
     setHeaderData(EmuFrontFileObject_FileCheckSum, Qt::Horizontal, tr("File Checksum"));
     setHeaderData(EmuFrontFileObject_FileSize, Qt::Horizontal, tr("File Size"));
     setHeaderData(EmuFrontFileObject_FileType, Qt::Horizontal, tr("File Type"));
     setHeaderData(EmuFrontFileObject_FileUpdateTime, Qt::Horizontal, tr("File Updated"));
 }

QString PlatformModel::constructSelect(QString where) const
{
    return QString("SELECT maintbl.id AS FileObjectId, "
            "maintbl.name AS Name, "
            "file.id AS FileId, "
            "file.name AS FileName, "
            "file.type AS FileType, "
            "file.checksum AS FileChecksum, "
            "file.size AS FileSize, "
            "file.updatetime AS FileUpdateTime "
            "FROM %1 AS maintbl "
            "LEFT OUTER JOIN file ON maintbl.fileid=file.id "
            "%2 "
            "ORDER BY Name").arg(tableName).arg(where);
}

bool PlatformModel::setName(int id, const QString &name)
{
    QSqlQuery query;
    query.prepare("update platform set name = :name where id = :id");
    query.bindValue(":name", name);
    query.bindValue(":id", id);
    return query.exec();
}

