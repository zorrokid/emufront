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
// You should have received a cyopy of the GNU General Public License
// along with EmuFront.  If not, see <http://www.gnu.org/licenses/>.

#include <QSqlQueryModel>
#include <QDebug>
#include "dbquerymodelmanager.h"

DbQueryModelManager::DbQueryModelManager(QObject *parent)
    : DatabaseManager(parent)
{
}

void DbQueryModelManager::filterById(int id)
{
    QList<QString> filters;
    filters.append(constructFilterById(id));
    filterDataObjects(filters);
}

/* filters is a list of SQL conditions e.g. setup.id=1 */
void DbQueryModelManager::filterDataObjects(QList<QString> filters)
{
   if (!sqlTableModel) sqlTableModel = getDataModel();
    QString query = constructSelect(constructWhereByFilters(filters));
    sqlTableModel->setQuery(query);
}

QString DbQueryModelManager::constructWhereByFilters(QList<QString>filters)
{
    if (filters.count() == 0) return "";
    QString where = " WHERE ";
    int c = 0;
    foreach(QString filter, filters){
        where.append(" %1 ").arg(filter);
        if (++c < filters.count())
            where.append(" AND ");
    }
    qDebug() << "constructWhereByFilters: " << where;
    return where;
}

void DbQueryModelManager::clearFilters()
{
    sqlTableModel->setQuery(constructSelect());
}

