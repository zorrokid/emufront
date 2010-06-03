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
#include "../dataobjects/filepathobject.h"
#include "dbfilepath.h"
#include "dbplatform.h"
#include "dbmediatype.h"

DbFilePath::DbFilePath(QObject *parent) : DatabaseManager(parent)
{
    dbPlatform = new DbPlatform(this);
    dbMediaType = new DbMediaType(this);
    sqlTableModel = getData();
}

QSqlTableModel* DbFilePath::getDataModel()
{
    return sqlTableModel;
}

EmuFrontObject* DbFilePath::recordToDataObject(const QSqlRecord *rec) const
{
    int id = rec->value(FilePath_Id).toInt();
    QString fpath = rec->value(FilePath_Name).toString();
    int plfId = rec->value(FilePath_PlatformId).toInt();
    int mtId = rec->value(FilePath_MediaTypeId).toInt();
    int fileType = rec->value(FilePath_FileTypeId).toInt();
    Platform *plf = dynamic_cast<Platform*>(dbPlatform->getDataObject(plfId));
    MediaType *mt = dynamic_cast<MediaType*>(dbMediaType->getDataObject(mtId));
       // TODO
    //int lastScanned = 0;
    return new FilePathObject(id, fpath, fpath, fileType, plf, mt);
}

bool DbFilePath::updateDataObjectToModel(const EmuFrontObject *ob)
{
    const FilePathObject *fpo = dynamic_cast<const FilePathObject*>(ob);
    bool ret = false;
    sqlTableModel->setFilter(QString("id = %1").arg(fpo->getId()));
    sqlTableModel->select();
    if (sqlTableModel->rowCount() == 1)
    {
        QSqlRecord rec = sqlTableModel->record(0);
        rec.setValue("name", fpo->getName());
        rec.setValue("filetypeid", fpo->getFiletype());

        Platform *pl = fpo->getPlatform();
        MediaType *mt = fpo->getMediaType();
        if (pl) rec.setValue("platformid", pl->getId());
        if (mt) rec.setValue("mediatypeid", mt->getId());

        // TODO
        //rec.setValue("lastscanned", 0);
    }
    return ret;
}

bool DbFilePath::insertDataObjectToModel(const EmuFrontObject *ob)
{
    const FilePathObject *fpo = dynamic_cast<const FilePathObject*>(ob);
    int row = 0;
    sqlTableModel->insertRows(row, 1);

    Platform *pl = fpo->getPlatform();
    MediaType *mt = fpo->getMediaType();

    //sqlTableModel->setData(sqlTableModel->index(row, FilePath_Id), NULL);
    sqlTableModel->setData(sqlTableModel->index(row, FilePath_Name), fpo->getName());
    sqlTableModel->setData(sqlTableModel->index(row, FilePath_FileTypeId), fpo->getFiletype());
    // not all the file path types have platform and/or media type
    if (pl) sqlTableModel->setData(sqlTableModel->index(row, FilePath_PlatformId), pl->getId());
    if (mt) sqlTableModel->setData(sqlTableModel->index(row, FilePath_MediaTypeId), mt->getId());
    // TODO:
    sqlTableModel->setData(sqlTableModel->index(row, FilePath_LastScanned), 0);
    return sqlTableModel->submitAll();
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

QSqlTableModel* DbFilePath::getData()
{
   QSqlRelationalTableModel *model = new QSqlRelationalTableModel(this);
   model->setTable(DB_TABLE_NAME_FILEPATH);
   model->setRelation(FilePath_PlatformId,
       QSqlRelation(DB_TABLE_NAME_PLATFORM, "id", "name"));
   model->setRelation(FilePath_MediaTypeId,
       QSqlRelation(DB_TABLE_NAME_MEDIATYPE, "id", "name"));
   model->setSort(FilePath_Name, Qt::AscendingOrder);
   model->setHeaderData(FilePath_MediaTypeId, Qt::Horizontal, tr("Media type"));
   model->setHeaderData(FilePath_PlatformId, Qt::Horizontal, tr("Platform"));
   model->select();
   return model;
}
