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

#ifndef DBFILEPATH_H
#define DBFILEPATH_H

#include "dbquerymodelmanager.h"

class DbSetup;

class DbFilePath : public DbQueryModelManager
{
public:
    DbFilePath(QObject *);    
    virtual bool updateDataObjectToModel(const EmuFrontObject*);
    bool insertDataObjectToModel(const EmuFrontObject*);
    bool deleteDataObjectFromModel(QModelIndex*);
    int countDataObjectRefs(int) const;
    enum { FilePath_Id = 0,
           FilePath_Name,
           FilePath_LastScanned,
           FilePath_SetupId,
           FilePath_SetupName };

protected:
    virtual EmuFrontObject* recordToDataObject(const QSqlRecord* ) const;
    virtual QString constructSelectById(int id) const;
    virtual QString constructSelect(QString whereClause = "") const;

private:
    virtual QSqlQueryModel* getData();
    DbSetup *dbSetup;
};

#endif // DBFILEPATH_H
