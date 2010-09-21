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

#ifndef DBFILE_H
#define DBFILE_H

#include "dbquerymodelmanager.h"
#include "../dataobjects/emufrontfile.h"

class DbFile : public DbQueryModelManager
{
public:
    DbFile(QObject*);
    virtual bool updateDataObjectToModel(const EmuFrontObject*);
    bool insertDataObjectToModel(const EmuFrontObject*);
    bool deleteDataObjectFromModel(QModelIndex*);
    int countDataObjectRefs(int) const;
    EmuFrontObject* getFileByChecksum(QString checksum);
    //int insertFile(const EmuFrontFile *);

    enum {
        File_Id = 0,
        File_Name,
        File_FileType,
        File_CheckSum,
        File_FileSize,
        File_UpdateTime };

protected:
    virtual EmuFrontObject* recordToDataObject(const QSqlRecord*);
    virtual QString constructSelectById(int id) const;
    virtual QString constructSelect(QString whereClause = "") const;
    int type;

private:
    virtual QSqlQueryModel* getData();
};

#endif // DBFILE_H
