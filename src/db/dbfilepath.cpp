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
#include "dbfilepath.h"
#include "../dataobjects/filepathobject.h"

DbFilePath::DbFilePath(QObject *parent) : DatabaseManager(parent)
{
    sqlTableModel = getData();
}

QSqlTableModel* DbFilePath::getDataModel()
{
    return sqlTableModel;
}

EmuFrontObject* DbFilePath::getDataObjectFromModel(QModelIndex *index)
{
    return new FilePathObject;
}

bool DbFilePath::updateDataObjectToModel(const EmuFrontObject *ob)
{
    return false;
}

bool DbFilePath::insertDataObjectToModel(const EmuFrontObject *ob)
{
    return false;
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
