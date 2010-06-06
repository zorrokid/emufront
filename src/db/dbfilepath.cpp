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
// Foobar is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

#include <QSqlRelationalTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include "../dataobjects/filepathobject.h"
#include "dbfilepath.h"
#include "dbsetup.h"

DbFilePath::DbFilePath(QObject *parent) : DbQueryModelManager(parent)
{
    dbSetup = new DbSetup(this);
    sqlTableModel = getData();
}

EmuFrontObject* DbFilePath::recordToDataObject(const QSqlRecord *rec) const
{
    int id = rec->value(FilePath_Id).toInt();
    QString fpath = rec->value(FilePath_Name).toString();
    int setupId = rec->value(FilePath_SetupId).toInt();
    int fileType = rec->value(FilePath_FileTypeId).toInt();
    Setup *sup = dynamic_cast<Setup*>(dbSetup->getDataObject(setupId));
       // TODO
    //int lastScanned = 0;
    return new FilePathObject(id, fpath, fpath, fileType, sup);
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
    query.bindValue(":filetypeid", fpo->getFiletype());
    query.bindValue(":lastscanned", 0); // TODO
    query.bindValue(":id", fpo->getId());
    ret = query.exec();
    if (ret) resetModel();
    /*sqlTableModel->setFilter(QString("id = %1").arg(fpo->getId()));
    sqlTableModel->select();
    if (sqlTableModel->rowCount() == 1)
    {
        QSqlRecord rec = sqlTableModel->record(0);
        rec.setValue("name", fpo->getName());
        rec.setValue("filetypeid", fpo->getFilet.bype());

        Setup *sup = fpo->getSetup();
        if (sup) rec.setValue("setupid", sup->getId());

        // TODO
        //rec.setValue("lastscanned", 0);
    }*/
    return ret;
}

bool DbFilePath::insertDataObjectToModel(const EmuFrontObject *ob)
{
    const FilePathObject *fpo = dynamic_cast<const FilePathObject*>(ob);
    QSqlQuery query;
    query.prepare("INSERT INTO filepath (id, name, filetypeid, setupid, lastscanned) "
        "VALUES (NULL, :name, :filetypeid, :setupid, :lastscanned) ");
    query.bindValue(":name", fpo->getName());
    query.bindValue(":filetypeid", fpo->getFiletype());
    if (fpo->getSetup())
        query.bindValue(":setupid", fpo->getSetup()->getId());
    query.bindValue(":lastscanned", 0); // TODO
    return query.exec();
    /*int row = 0;

    sqlTableModel->insertRows(row, 1);


    Setup *sup = fpo->getSetup();
    //Platform *pl = fpo->getPlatform();
    //MediaType *mt = fpo->getMediaType();

    //sqlTableModel->setData(sqlTableModel->index(row, FilePath_Id), NULL);
    sqlTableModel->setData(sqlTableModel->index(row, FilePath_Name), fpo->getName());
    sqlTableModel->setData(sqlTableModel->index(row, FilePath_FileTypeId), fpo->getFiletype());
    // not all the file path types have platform and/or media type
    //if (pl) sqlTableModel->setData(sqlTableModel->index(row, FilePath_PlatformId), pl->getId());
    //if (mt) sqlTableModel->setData(sqlTableModel->index(row, FilePath_MediaTypeId), mt->getId());
    if (sup) sqlTableModel->setData(sqlTableModel->index(row, FilePath_SetupId), sup->getId());
    // TODO:
    sqlTableModel->setData(sqlTableModel->index(row, FilePath_LastScanned), 0);
    return sqlTableModel->submitAll();*/
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

QString DbFilePath::constructSelectById(int id) const
{
    return constructSelect(QString("filepath.id = %1").arg(id));
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

            //"platform.name, mediatype.name
   /*QSqlRelationalTableModel *model = new QSqlRelationalTableModel(this);
   model->setTable(DB_TABLE_NAME_FILEPATH);*/
   /*model->setRelation(FilePath_PlatformId,
       QSqlRelation(DB_TABLE_NAME_PLATFORM, "id", "name"));
   model->setRelation(FilePath_MediaTypeId,
       QSqlRelation(DB_TABLE_NAME_MEDIATYPE, "id", "name"e));*/
    /*model->setRelation(FilePath_SetupId,
        QSqlRelation(DB_TABLE_NAME_SETUP, "id", ""));
           model->setSort(FilePath_Name, Qt::AscendingOrder);*/

   //model->setHeaderData(FilePath_MediaTypeId, Qt::Horizontal, tr("Media type"));
   //model->setHeaderData(FilePath_PlatformId, Qt::Horizontal, tr("Platform"));
   /*model->select();
   return model;*/
}
