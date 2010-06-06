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

#ifndef DBSETUP_H
#define DBSETUP_H

#include "dbquerymodelmanager.h"
#include "../dataobjects/setup.h"

class DbPlatform;
class DbMediaType;

class DbSetup : public DbQueryModelManager
{
public:
    DbSetup(QObject *);
    virtual bool updateDataObjectToModel(const EmuFrontObject*);
    bool insertDataObjectToModel(const EmuFrontObject*);
    bool deleteDataObjectFromModel(QModelIndex*);
    int countDataObjectRefs(int) const;
    enum { Setup_Id = 0,
           Setup_PlatformId,
           Setup_MediaTypeId,
           Setup_FileTypeExtensions,
           Setup_Name };
    static const QString FILE_TYPE_EXTENSION_SEPARATOR;

protected:
    virtual EmuFrontObject* recordToDataObject(const QSqlRecord* ) const;
    virtual QString constructSelectById(int id) const;
    virtual QString constructSelect(QString whereClause = "") const;

private:
    virtual QSqlQueryModel* getData();
    DbPlatform *dbPlatform;
    DbMediaType *dbMediaType;
};
#endif // DBSETUP_H
