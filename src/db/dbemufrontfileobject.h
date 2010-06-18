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

#ifndef DBEMUFRONTFILEOBJECT_H
#define DBEMUFRONTFILEOBJECT_H

#include "dbtablemodelmanager.h"
#include "dbfile.h"
#include "../dataobjects/emufrontfileobject.h"

class DbEmuFrontFileObject : public DbTableModelManager
{
public:
    DbEmuFrontFileObject(QObject *parent);
    virtual bool updateDataObjectToModel(const EmuFrontObject*);
    bool insertDataObjectToModel(const EmuFrontObject*);
    bool deleteDataObjectFromModel(QModelIndex*);
    int countDataObjectRefs(int) const;
    enum {
        EmuFrontFileObject_Id= 0,
        EmuFrontFileObject_Name,
        EmuFrontFileObject_FileId };

protected:
    virtual EmuFrontObject* recordToDataObject(const QSqlRecord* );
    QString tableName;
    virtual EmuFrontObject* createEmuFrontFileObject(int id, QString name, EmuFrontFile *f) = 0;

private:
    virtual QSqlQueryModel* getData();
    DbFile *dbFile;
};

#endif // DBEMUFRONTFILEOBJECT_H
