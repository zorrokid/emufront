// EmuFront
// Copyright 2010 Mikko Keinänen
//
// This file is part of EmuFront.
//
//
// EmuFront is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as published by
// the Free Software Foundation and appearing in the file gpl.txt included in the
// packaging of this file.
//
// EmuFront is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with EmuFront.  If not, see <http://www.gnu.org/licenses/>.

#ifndef DBEMUFRONTFILEOBJECT_H
#define DBEMUFRONTFILEOBJECT_H

#include "dbquerymodelmanager.h"
#include "dbfile.h"
#include "../dataobjects/emufrontfileobject.h"

class DbEmuFrontFileObject : public DbQueryModelManager
{
public:
DbEmuFrontFileObject(QObject *parent);
virtual bool updateDataObjectToModel(const EmuFrontObject*);
int insertDataObjectToModel(const EmuFrontObject*);
bool deleteDataObjectFromModel(QModelIndex*);
enum {
   EmuFrontFileObject_Id= 0,
   EmuFrontFileObject_Name,
   EmuFrontFileObject_FileId,
   EmuFrontFileObject_FileName,
   EmuFrontFileObject_FileType,
   EmuFrontFileObject_FileCheckSum,
   EmuFrontFileObject_FileSize,
   EmuFrontFileObject_FileUpdateTime
};

protected:
   virtual QString constructSelect(QString whereClause = "") const;
   virtual QString constructSelectById(int id) const;
   virtual QString constructFilterById(int id) const;
   virtual EmuFrontObject* recordToDataObject(const QSqlRecord* );
   QString tableName;
   virtual EmuFrontObject* createEmuFrontFileObject(int id, QString name, EmuFrontFile *f) = 0;
   virtual bool deleteDataObject(int id) const;

private:
    virtual QSqlQueryModel* getData();
    DbFile *dbFile;
};

#endif // DBEMUFRONTFILEOBJECT_H
