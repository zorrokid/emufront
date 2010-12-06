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

#include "setupmodel.h"
#include <QtSql>

SetupModel::SetupModel(QObject *parent) :
    EmuFrontQueryModel(parent)
{
    refresh();
}

void SetupModel::refresh()
{
    setQuery(constructSelect());
    setHeaderData(Setup_Id, Qt::Horizontal, tr("Id"));
    setHeaderData(Setup_PlatformId, Qt::Horizontal, tr("Platform id"));
    setHeaderData(Setup_MediaTypeId, Qt::Horizontal, tr("Media type id"));
    setHeaderData(Setup_FileTypeExtensions, Qt::Horizontal, tr("File types"));
    setHeaderData(Setup_Name, Qt::Horizontal, tr("Name"));
}

QString SetupModel::constructSelect(QString where) const
{
    return QString(
        "SELECT setup.id AS SetupId, "
        "setup.platformid AS PlatformId, "
        "setup.mediatypeid AS MediaTypeId, "
        "setup.filetypeextensions AS SupportedFileTypeExtensions, "
        "platform.name || ' ' || mediatype.name AS SetupName "
        "FROM setup "
        "INNER JOIN platform ON setup.platformid=platform.id "
        "INNER JOIN mediatype ON setup.mediatypeid=mediatype.id %1 "
        "ORDER BY SetupName"
        ).arg(where);
}

Qt::ItemFlags SetupModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlQueryModel::flags(index);
    if (index.column() == Setup_PlatformId || index.column() == Setup_MediaTypeId) {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}

bool SetupModel::setData(const QModelIndex &index, const QVariant &value, int /*role*/)
{
    if(index.column() != Setup_PlatformId && index.column() != Setup_MediaTypeId)
        return false;

    QModelIndex primaryKeyIndex
        = QSqlQueryModel::index(index.row(), Setup_Id);

    int id = data(primaryKeyIndex).toInt();
    clear();

    bool ok;
    switch(index.column()) {

    case Setup_PlatformId:
        ok = setPlatform(id, value.toInt());
        break;

    case Setup_MediaTypeId:
        ok = setMediaType(id, value.toInt());
        break;
    };
    refresh();
    return ok;
}

bool SetupModel::setPlatform(int id, int platformId)
{
    QSqlQuery query;
    query.prepare(QString("update setup set platformid = :platformid where id = :id"));
    query.bindValue(":platformid", platformId);
    query.bindValue(":id", id);
    return query.exec();
}

bool SetupModel::setMediaType(int id, int mediaTypeId)
{
    QSqlQuery query;
    query.prepare(QString("update setup set mediatypeid = :mediatypeid where id = :id"));
    query.bindValue(":mediatypeid", mediaTypeId);
    query.bindValue(":id", id);
    return query.exec();
}
