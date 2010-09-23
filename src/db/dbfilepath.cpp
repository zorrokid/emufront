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
// EmuFront is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with EmuFront.  If not, see <http://www.gnu.org/licenses/>.

#include <QSqlRelationalTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include "../dataobjects/filepathobject.h"
#include "dbfilepath.h"
#include "dbsetup.h"

DbFilePath::DbFilePath(QObject *parent) : DbQueryModelManager(parent)
{
    dbSetup = new DbSetup(this);
}

EmuFrontObject* DbFilePath::recordToDataObject(const QSqlRecord *rec)
{
    int id = rec->value(FilePath_Id).toInt();
    QString fpath = rec->value(FilePath_Name).toString();
    int setupId = rec->value(FilePath_SetupId).toInt();
    //int fileType = rec->value(FilePath_FileTypeId).toInt();
    Setup *sup = dynamic_cast<Setup*>(dbSetup->getDataObject(setupId));
       // TODO
    //int lastScanned = 0;
    return new FilePathObject(id, fpath, /* TODO */ 0, sup);
}

bool DbFilePath::updateDataObjectToModel(const EmuFrontObject *ob)
{
    const FilePathObject *fpo = dynamic_cast<const FilePathObject*>(ob);
    bool ret = false;
    QSqlQuery query;
    query.prepare(QString("UPDATE filepath SET "
        "name=:name, "
        "filetypeid=:filetypeid, "
        "setupid=:setupid, "
        "lastscanned=:lastscanned "
        "WHERE id=:id"));
    query.bindValue(":name", fpo->getName());
    query.bindValue(":filetypeid", fpo->getType());
    query.bindValue(":lastscanned", 0); // TODO
    query.bindValue(":id", fpo->getId());
    ret = query.exec();
    if (ret) resetModel();
    return ret;
}

int DbFilePath::insertDataObjectToModel(const EmuFrontObject *ob)
{
    const FilePathObject *fpo = dynamic_cast<const FilePathObject*>(ob);
    QSqlQuery query;
    query.prepare("INSERT INTO filepath (id, name, filetypeid, setupid, lastscanned) "
        "VALUES (NULL, :name, :filetypeid, :setupid, :lastscanned) ");
    query.bindValue(":name", fpo->getName());
    query.bindValue(":filetypeid", fpo->getType());
    if (fpo->getSetup())
        query.bindValue(":setupid", fpo->getSetup()->getId());
    query.bindValue(":lastscanned", 0); // TODO
    int id = -1;
    if (query.exec())
        id = query.lastInsertId().toInt();
    return id;
}

int DbFilePath::countDataObjectRefs(int id) const
{
    return 0;
}

// WARNING: this will delete also all the databindings to selected media image path
bool DbFilePath::deleteDataObjectFromModel(QModelIndex *index)
{
    return false;
}

QString DbFilePath::constructSelect(QString whereClause) const
{
    QString where = whereClause.isEmpty()
        ? "" : QString("WHERE ").append(whereClause);

    return QString("SELECT filepath.id AS FilePathId, "
            "filepath.name AS Name, "
            "filepath.lastscanned AS LastScanned, "
            "setup.id AS SetupId, "
            "platform.name || ' ' || mediatype.name AS SetupName "
            "FROM filepath "
            "INNER JOIN setup ON filepath.setupid=setup.id  "
            "INNER JOIN platform ON setup.platformid=platform.id "
            "INNER JOIN mediatype ON setup.mediatypeid=mediatype.id "
            "%1 "
            "ORDER BY SetupName").arg(where);
}

QString DbFilePath::constructFilterById(int id) const
{
    return QString("filepath.id = %1").arg(id);
}

QString DbFilePath::constructSelectById(int id) const
{
    return constructSelect(constructFilterById(id));
}

QSqlQueryModel* DbFilePath::getData()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(constructSelect());
    model->setHeaderData(FilePath_Id, Qt::Horizontal, tr("Id"));
    model->setHeaderData(FilePath_Name, Qt::Horizontal, tr("Name"));
    model->setHeaderData(FilePath_LastScanned, Qt::Horizontal, tr("Last scanned"));
    model->setHeaderData(FilePath_SetupId, Qt::Horizontal, tr("Set up id"));
    model->setHeaderData(FilePath_SetupName, Qt::Horizontal, tr("Set up"));
    return model;
}
