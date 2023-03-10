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

#ifndef DBQUERYMODELMANAGER_H
#define DBQUERYMODELMANAGER_H

#include "databasemanager.h"

class DbQueryModelManager : public DatabaseManager
{
public:
    DbQueryModelManager(QObject *parent);
protected:
    void filterById(int id);
    void filterDataObjects(QList<QString> filter);
    void clearFilters();
    QString constructWhereByFilters(QList<QString> filters);
    virtual QString constructSelectById(int id) const = 0;
    virtual QString constructFilterById(int id) const = 0;
    virtual QString constructSelect(QString whereClause = "") const = 0;
    bool deleteDataObject(int id) const;
    QString tableName;
private:
    virtual QString getDeleteObjectSql() const;
};

#endif // DBQUERYMODELMANAGER_H
