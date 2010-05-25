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

#ifndef DBMEDIAIMAGEPATH_H
#define DBMEDIAIMAGEPATH_H

#include "databasemanager.h"

class DbMediaImagePath : public DatabaseManager
{
public:
    DbMediaImagePath(QObject *);
    virtual QSqlTableModel* getDataModel();
    virtual EmuFrontObject* getDataObjectFromModel(QModelIndex*);
    virtual bool updateDataObjectToModel(const EmuFrontObject*);
    bool insertDataObjectToModel(const EmuFrontObject*);
    bool deleteDataObjectFromModel(QModelIndex*);
    int countDataObjectRefs(int) const;

private:
    virtual QSqlTableModel* getData();

};

#endif // DBMEDIAIMAGEPATH_H
