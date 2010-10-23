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

#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRelationalTableModel>
#include <QDebug>
#include "dbemufrontfileobject.h"

DbEmuFrontFileObject::DbEmuFrontFileObject(QObject *parent)
    : DbQueryModelManager(parent)
{
    dbFile = new DbFile(this);
}

EmuFrontObject* DbEmuFrontFileObject::recordToDataObject(const QSqlRecord *record)
{
    int id = record->value(EmuFrontFileObject_Id).toInt();
    QString name = record->value(EmuFrontFileObject_Name).toString();
    int fileId = record->value(EmuFrontFileObject_FileId).toInt();
    EmuFrontFile *f = 0;
    if (fileId > 0)
    {
        EmuFrontObject *o = dbFile->getDataObject(fileId);
        f = dynamic_cast<EmuFrontFile*>(o);
    }
    EmuFrontObject *efo = createEmuFrontFileObject(id, name, f);
    return efo;
}

bool DbEmuFrontFileObject::updateDataObjectToModel(const EmuFrontObject *ob)
{
    const EmuFrontFileObject *plf = dynamic_cast<const EmuFrontFileObject*>(ob);
    bool ret = false;
    QSqlQuery query;
    query.prepare(QString("UPDATE %1 SET "
        "name=:name, "
        "fileid=:fileid "
        "WHERE id=:id").arg(tableName));
    query.bindValue(":name", plf->getName());
    query.bindValue(":fileid",
                    plf->getFile() ? QString(plf->getFile()->getId()) : "NULL");
    query.bindValue(":id", plf->getId());
    ret = query.exec();

    if (ret) resetModel();
    else
            qDebug() << "Failed updating " << tableName
                << " " <<   query.lastError().text()
                << " " << query.executedQuery() ;
    return ret;
}

int DbEmuFrontFileObject::insertDataObjectToModel(const EmuFrontObject *ob)
{
    const EmuFrontFileObject *plf = dynamic_cast<const EmuFrontFileObject *>(ob);
    QSqlQuery query;
    query.prepare(QString("INSERT INTO %1 (id, name, fileid) "
        "VALUES (NULL, :name, :fileid) ").arg(tableName));
    query.bindValue(":name", plf->getName());
    if (plf->getFile())
        query.bindValue(":fileid", plf->getFile()->getId());
    else query.bindValue(":fileid", "NULL");
    int id = -1;
    if (query.exec())
        id = query.lastInsertId().toInt();
    else
        qDebug() << "Failed inserting to " << tableName << " "
            << query.lastError().text() << " " << query.executedQuery() ;
    return id;
}

// WARNING: this will delete also all the databindings to selected platform
bool DbEmuFrontFileObject::deleteDataObjectFromModel(QModelIndex *index)
{
    // TODO
    return false;
}

QString DbEmuFrontFileObject::constructSelect(QString where) const
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

QString DbEmuFrontFileObject::constructSelectById(int id) const
{
    return constructSelect(QString("WHERE %1").arg(constructFilterById(id)));
}

QString DbEmuFrontFileObject::constructFilterById(int id) const
{
    return QString("maintbl.id = %1").arg(id);
}

QSqlQueryModel* DbEmuFrontFileObject::getData()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(constructSelect());
    model->setHeaderData(EmuFrontFileObject_Id, Qt::Horizontal, tr("Id"));
    model->setHeaderData(EmuFrontFileObject_Name, Qt::Horizontal, tr("Name"));
    return model;
}

QString DbEmuFrontFileObject::getDeleteObjectSql() const
{
    return QString("DELETE FROM %1 WHERE id=:id").arg(tableName);
}
