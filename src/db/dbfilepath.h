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

#include "databasemanager.h"

class DbPlatform;
class DbMediaType;

class DbFilePath : public DatabaseManager
{
public:
    DbFilePath(QObject *);    
    virtual QSqlTableModel* getDataModel();
    virtual bool updateDataObjectToModel(const EmuFrontObject*);
    bool insertDataObjectToModel(const EmuFrontObject*);
    bool deleteDataObjectFromModel(QModelIndex*);
    int countDataObjectRefs(int) const;

protected:
    virtual EmuFrontObject* recordToDataObject(const QSqlRecord* ) const;

private:
    virtual QSqlTableModel* getData();
    DbPlatform *dbPlatform;
    DbMediaType *dbMediaType;
    // TODO: add last scanned column
    enum { FilePath_Id = 0,
           FilePath_Name = 1,
           FilePath_FileTypeId = 3,
           FilePath_PlatformId = 4,
           FilePath_MediaTypeId = 5,
           FilePath_LastScanned = 6 };
};

#endif // DBFILEPATH_H
