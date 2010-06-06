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

#ifndef DBMEDIATYPE_H
#define DBMEDIATYPE_H

#include "dbtablemodelmanager.h"
#include "../dataobjects/mediatype.h"

class DbMediaType : public DbTableModelManager
{
public:
    DbMediaType(QObject *);
    virtual bool updateDataObjectToModel(const EmuFrontObject*);
    virtual bool insertDataObjectToModel(const EmuFrontObject*);
    virtual bool deleteDataObjectFromModel(QModelIndex*);
    virtual int countDataObjectRefs(int) const;
    enum {
        MediaType_Id = 0,
        MediaType_Name = 1,
        MediaType_Filename = 2 };

protected:
    virtual EmuFrontObject* recordToDataObject(const QSqlRecord* ) const;

private:
      virtual QSqlQueryModel* getData();

};

#endif // DBMEDIATYPE_H
