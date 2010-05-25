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

#include <QSqlTableModel>
#include "dbmediaimagepath.h"
#include "../dataobjects/filepathobject.h"

DbMediaImagePath::DbMediaImagePath(QObject *parent) : DatabaseManager(parent)
{
}

QSqlTableModel* DbMediaImagePath::getDataModel()
{
    return sqlTableModel;
}

EmuFrontObject* DbMediaImagePath::getDataObjectFromModel(QModelIndex *index)
{
    return new FilePathObject;
}

bool DbMediaImagePath::updateDataObjectToModel(const EmuFrontObject *ob)
{
    return false;
}

bool DbMediaImagePath::insertDataObjectToModel(const EmuFrontObject *ob)
{
    return false;
}

int DbMediaImagePath::countDataObjectRefs(int id) const
{
    return 0;
}

// WARNING: this will delete also all the databindings to selected media image path
bool DbMediaImagePath::deleteDataObjectFromModel(QModelIndex *index)
{
    return false;
}

QSqlTableModel* DbMediaImagePath::getData()
{
   QSqlTableModel *model = new QSqlTableModel(this);
   return model;
}
