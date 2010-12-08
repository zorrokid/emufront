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
#include "emufrontexception.h"
#include <QtSql>

const QString SetupModel::FILE_TYPE_EXTENSION_SEPARATOR = QString("|");

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
    int col = index.column();
    if (col == Setup_PlatformId ||
        col == Setup_MediaTypeId ||
        col == Setup_FileTypeExtensions) {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}

bool SetupModel::setData(const QModelIndex &index, const QVariant &value, int /*role*/)
{
    int col = index.column();
    if(col != Setup_PlatformId &&
        col != Setup_MediaTypeId &&
        col != Setup_FileTypeExtensions)
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

    case Setup_FileTypeExtensions:
        ok = setSupportedExtensions(id, value.toString());
        break;

    default:
        qDebug() << "Setup model, this shouldn't be happening!";
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

bool SetupModel::setSupportedExtensions(int id, QString exts)
{
    QSqlQuery query;
    query.prepare(QString("update setup set filetypeextensions = :exts where id = :id"));
    query.bindValue(":exts", exts);
    query.bindValue(":id", id);
    return query.exec();
}

bool SetupModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (parent.isValid())
        return false; // This is a flat model
    if (rowCount() < row)
        row = rowCount() + 1;
    // we need a default value for platformid and mediatypeid and if none is yet
    // available an error message must be shown!
    int plfId = -1;
    int mdtId = -1;
    QSqlQuery q;
    q.exec(QString("SELECT id FROM platform ORDER BY name LIMIT 1"));
    if (q.first()){
        qDebug() << "Got id " << plfId << " for default platform.";
        plfId = q.value(0).toInt();
    }
    else {
        throw EmuFrontException(tr("No platforms yet available for setup configuration!"));
    }
    q.exec(QString("SELECT id FROM mediatype ORDER BY name LIMIT 1"));
    if (q.first()) {
        qDebug() << "Got id " << mdtId << " for default media type.";
        mdtId = q.value(0).toInt();
    }
    else {
        throw EmuFrontException(tr("No media types yet available for setup configuration!"));
    }
    q.prepare(QString("INSERT INTO setup (id, platformid, mediatypeid, filetypeextensions) "
        " VALUES (NULL, :plfid, :mdtid, '') "));
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        q.bindValue(":plfid", plfId);
        q.bindValue(":mdtid", mdtId);
        if (!q.exec()) {
            throw EmuFrontException(tr("Failed creating new setup: %1").
                arg(q.lastError().text()));
        }
    }
    endInsertRows();
    refresh();
    return true;
}

bool SetupModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (parent.isValid()) {
        return false; // This is a flat model
    }
    if (rowCount() < row + count - 1)
        return false;

    QSqlQuery q;
    q.prepare(QString("DELETE FROM setup WHERE id=:id"));
    QModelIndex primaryIndex;
    int id = -1;
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for(int i = 0; i < count; ++i) {
        primaryIndex = QSqlQueryModel::index(row + i, Setup_Id);
        id = data(primaryIndex).toInt();
        qDebug() << "Removing data item with id " << id;
        q.bindValue(":id", id);
        q.exec();
    }
    endRemoveRows();
    refresh();
    return true;
}

