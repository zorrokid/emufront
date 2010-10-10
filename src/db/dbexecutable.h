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

#ifndef DBEXECUTABLE_H
#define DBEXECUTABLE_H

#include "dbquerymodelmanager.h"

class DbSetup;

class DbExecutable : public DbQueryModelManager
{
public:
    DbExecutable(QObject*);
     virtual bool updateDataObjectToModel(const EmuFrontObject*);
    int insertDataObjectToModel(const EmuFrontObject*);
    bool deleteDataObjectFromModel(QModelIndex*);
    enum {
        Executable_Id = 0,
        Executable_Name,
        Executable_Executable,
        Executable_Options,
        Executable_TypeId,
        Executable_SetupId,
        Executable_SetupName
    };
protected:
    virtual EmuFrontObject* recordToDataObject(const QSqlRecord*);
    virtual QString constructSelectById(int id) const;
    virtual QString constructFilterById(int id) const;
    virtual QString constructSelect(QString whereClause = "") const;
    virtual bool deleteDataObject(int id) const;
private:
    virtual QSqlQueryModel* getData();
    virtual QString getCountRefsSelect(int) const;
    DbSetup *dbSetup;
};

#endif // DBEXECUTABLE_H
