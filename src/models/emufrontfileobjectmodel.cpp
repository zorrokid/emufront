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

#include <QtSql>
#include "emufrontfileobjectmodel.h"
#include "emufrontfile.h"
#include "emufrontfileobject.h"

EmuFrontFileObjectModel::EmuFrontFileObjectModel(QObject *parent) :
    EmuFrontQueryModel(parent)
{ }

Qt::ItemFlags EmuFrontFileObjectModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlQueryModel::flags(index);
    if (index.column() == EmuFrontFileObject_Name) {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}

bool EmuFrontFileObjectModel::setData(const QModelIndex &index, const QVariant &value, int /*role*/)
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

void EmuFrontFileObjectModel::refresh()
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

QString EmuFrontFileObjectModel::constructSelect(QString where) const
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

bool EmuFrontFileObjectModel::setName(int id, const QString &name)
{
    QSqlQuery query;
    query.prepare(QString("update %1 set name = :name where id = :id").arg(tableName));
    query.bindValue(":name", name);
    query.bindValue(":id", id);
    return query.exec();
}

bool EmuFrontFileObjectModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (parent.isValid())
        return false; // This is a flat model
    if (rowCount() < row)
        row = rowCount() + 1;
    QSqlQuery q;
    q.prepare(QString("INSERT INTO %1 (id, name, fileid) "
        " VALUES (NULL, '', NULL) ").arg(tableName));
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        q.exec();
    }
    endInsertRows();
    refresh();
    return true;
}

bool EmuFrontFileObjectModel::removeRows(int row, int count, const QModelIndex &parent)
{
     if (parent.isValid()) {
        return false; // This is a flat model
    }
    if (rowCount() < row + count - 1)
        return false;

    QSqlQuery q;
    q.prepare(QString("DELETE FROM %1 WHERE id=:id").arg(tableName));
    QModelIndex primaryIndex;
    int id = -1;
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for(int i = 0; i < count; ++i) {
        primaryIndex = QSqlQueryModel::index(row + i, EmuFrontFileObject_Id);
        id = data(primaryIndex).toInt();
        qDebug() << "Removing data item with id " << id;
        q.bindValue(":id", id);
        q.exec();
    }
    endRemoveRows();
    refresh();
    return true;
}

// Implemented for EmuFrontQueryModel:
EmuFrontObject* EmuFrontFileObjectModel::recordToDataObject(const QSqlRecord* record)
{
    int id = record->value(EmuFrontFileObject_Id).toInt();
    QString name = record->value(EmuFrontFileObject_Name).toString();
    int fileId = record->value(EmuFrontFileObject_FileId).toInt();
    EmuFrontFile *f = 0;
    /*if (fileId > 0)
    {
        // TODO: need fileModel
        EmuFrontObject *o = fileModel.getDataObject(fileId);
        f = dynamic_cast<EmuFrontFile*>(o);
    }*/
    EmuFrontObject *efo = createEmuFrontFileObject(id, name, f);
    return efo;
}

QString EmuFrontFileObjectModel::constructFilterById(int id) const
{
    return QString("maintbl.id = %1").arg(id);
}
