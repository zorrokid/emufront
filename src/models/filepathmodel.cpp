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

#include "filepathmodel.h"
#include "emufrontfile.h"
#include "emufrontexception.h"
#include "filepathobject.h"
#include "setup.h"
#include "setupmodel.h"
#include <QtSql>

FilePathModel::FilePathModel(QObject *parent) :
    EmuFrontQueryModel(parent)
{
    refresh();
}

void FilePathModel::refresh()
{
    setQuery(constructSelect());
    setHeaderData(FilePath_Id, Qt::Horizontal, tr("Id"));
    setHeaderData(FilePath_Name, Qt::Horizontal, tr("Name"));
    setHeaderData(FilePath_LastScanned, Qt::Horizontal, tr("Last scanned"));
    setHeaderData(FilePath_SetupId, Qt::Horizontal, tr("Set up id"));
    setHeaderData(FilePath_SetupName, Qt::Horizontal, tr("Set up"));
}

QString FilePathModel::constructSelect(QString where) const
{
    return QString("SELECT "
                   "filepath.id AS FilePathId, "
                   "filepath.name AS Name, "
                   "datetime(filepath.lastscanned, 'unixepoch') AS LastScanned, "
                   "setup.id AS SetupId, "
                   "platform.name || ' ' || mediatype.name AS SetupName, "
                   "filepath.filetypeid "
                   "FROM filepath "
                   "INNER JOIN setup ON filepath.setupid=setup.id  "
                   "INNER JOIN platform ON setup.platformid=platform.id "
                   "INNER JOIN mediatype ON setup.mediatypeid=mediatype.id "
                   "%1 "
                   "ORDER BY SetupName").arg(where);
}

Qt::ItemFlags FilePathModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlQueryModel::flags(index);
    int col = index.column();
    if (col == FilePath_SetupId ||
        col == FilePath_Name) {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}

bool FilePathModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int col = index.column();
    if (col != FilePath_SetupId &&
        col != FilePath_Name) {
        return false;
    }

    QModelIndex primaryKeyIndex = QSqlQueryModel::index(index.row(), FilePath_Id);

    int id = data(primaryKeyIndex).toInt();
    clear();

    bool ok;
    switch(index.column()) {
    case FilePath_SetupId:
        ok = setSetup(id, value.toInt());
        break;
    case FilePath_Name:
        ok = setFilePath(id, value.toString());
        break;
    default:
        ok = false;
        qDebug() << "File path model, this shouldn't be happening!";
    }
    refresh();
    return ok;
}

bool FilePathModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (parent.isValid())
        return false; // This is a flat model
    if (rowCount() < row)
        row = rowCount() + 1;
    int supId = -1;
    QSqlQuery q;
    q.exec(QString("SELECT setup.id, "
           // The following is to get the correct order:
           "platform.name || ' ' || mediatype.name AS SetupName "
           "FROM setup "
           "INNER JOIN platform ON setup.platformid=platform.id "
           "INNER JOIN mediatype ON setup.mediatypeid=mediatype.id "
           "ORDER BY SetupName "
           "LIMIT 1"));
    if (q.first()) {
        supId = q.value(0).toInt();
        qDebug() << "Got id " << supId << " for default setup.";
    }
    else {
        throw EmuFrontException(tr("No setups yet available for file path configuration!"));
    }
    q.prepare(QString("INSERT INTO filepath "
        "(id, name, filetypeid, setupid, lastscanned) "
        "VALUES (NULL, '', :filetype, :setupid, :lastscanned )"));
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for(int i = 0; i < count; ++i) {
        q.bindValue(":filetype", EmuFrontFile::FileType_MediaImageContainer);
        q.bindValue(":setupid", supId);
        q.bindValue(":lastscanned", 0);
        if (!q.exec()) {
            throw EmuFrontException(tr("Failed creating new filepath row: %1").
                                    arg(q.lastError().text()));
        }
    }
    endInsertRows();
    refresh();
    return true;
}

bool FilePathModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (parent.isValid()) {
        return false; // This is a flat model
    }
    if (rowCount() < row + count - 1)
        return false;

    QSqlQuery q;
    q.prepare(QString("DELETE FROM filepath WHERE id=:id"));
    QModelIndex primaryIndex;
    int id = -1;
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for(int i = 0; i < count; ++i) {
        primaryIndex = QSqlQueryModel::index(row + i, FilePath_Id);
        id = data(primaryIndex).toInt();
        qDebug() << "Removing data item with id " << id;
        q.bindValue(":id", id);
        q.exec();
    }
    endRemoveRows();
    refresh();
    return true;
}

bool FilePathModel::setSetup(int id, int setupId)
{
    QSqlQuery q;
    q.prepare(QString("UPDATE filepath SET setupid = :setupid WHERE id = :id"));
    q.bindValue(":setupid", setupId);
    q.bindValue(":id", id);
    return q.exec();
}

bool FilePathModel::setFilePath(int id, QString filePath)
{
    QSqlQuery q;
    q.prepare(QString("UPDATE filepath SET name = :name WHERE id = :id"));
    q.bindValue(":name", filePath);
    q.bindValue(":id", id);
    return q.exec();
}

bool FilePathModel::setScanned(int id)
{
    QSqlQuery q;
    q.prepare(QString("UPDATE filepath SET lastscanned = :timestamp WHERE id = :id"));
    q.bindValue(":timestamp", getCurrentTimeStamp());
    q.bindValue(":id", id);
    return q.exec();
}

FilePathObject* FilePathModel::getFilePathObject(const QModelIndex &index)
{
    if (!index.isValid()) return 0;
    EmuFrontObject *efo = getDataObject(index);
    return dynamic_cast<FilePathObject *>(efo);
}

EmuFrontObject* FilePathModel::recordToDataObject(const QSqlRecord* rec)
{
    int id = rec->value(FilePath_Id).toInt();
    QString fpath = rec->value(FilePath_Name).toString();
    int setupId = rec->value(FilePath_SetupId).toInt();
    int fileType = rec->value(FilePath_FileTypeId).toInt();
    SetupModel supModel;
    EmuFrontObject *efo = supModel.getDataObject(setupId);
    Setup *sup = dynamic_cast<Setup*>(efo);
    return new FilePathObject(id, fpath, fileType, sup);
}

QString FilePathModel::constructFilterById(int id) const
{
    return QString("filepath.id = %1").arg(id);
}
