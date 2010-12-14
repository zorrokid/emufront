/*
** EmuFront
** Copyright 2010 Mikko Keinänen
**
** This file is part of EmuFront.
**
**
** EmuFront is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License version 2 as published by
** the Free Software Foundation and appearing in the file gpl.txt included in the
** packaging of this file.
**
** EmuFront is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with EmuFront.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "emufrontquerymodel.h"
#include "emufrontobject.h"
#include "platform.h"
#include "mediatype.h"
#include <QtSql>

EmuFrontQueryModel::EmuFrontQueryModel(QObject *parent) :
    QSqlQueryModel(parent)
{
}

EmuFrontObject* EmuFrontQueryModel::getObject(int row) const
{
    // TODO
    return 0;
}

/*Platform* EmuFrontQueryModel::getPlatform(int id) const
{
}*/

EmuFrontObject* EmuFrontQueryModel::getDataObject(int id)
{
    filterById(id);
    return getFilteredDataObject();
}

EmuFrontObject* EmuFrontQueryModel::getDataObject(QString filter)
{
    QList<QString> filters;
    filters.append(filter);
    filterDataObjects(filters);
    return getFilteredDataObject();
}

EmuFrontObject* EmuFrontQueryModel::getFilteredDataObject()
{
    EmuFrontObject *plf = 0;
    // TODO: if record has more than one the first instance is returned
    // ... check if this is ok in all cases!
    if (rowCount() >= 1)
    {
        QSqlRecord rec = record(0);
        if (rec.isEmpty()) {
            return 0;
        }
        else plf = recordToDataObject(&rec);
    }
     return plf;
}

EmuFrontObject* EmuFrontQueryModel::getDataObject(const QModelIndex &index)
{
    QSqlRecord rec = record(index.row());
    return recordToDataObject(&rec);
}

int EmuFrontQueryModel::getCurrentTimeStamp() {
    return QDateTime::currentDateTime().toTime_t();
}

void EmuFrontQueryModel::filterById(int id)
{
    QList<QString> filters;
    filters.append(constructFilterById(id));
    filterDataObjects(filters);
}

/* filters is a list of SQL conditions e.g. setup.id=1 */
void EmuFrontQueryModel::filterDataObjects(QList<QString> filters)
{
    QString where = constructWhereByFilters(filters);
    QString query = constructSelect(where);
    setQuery(query);
}

QString EmuFrontQueryModel::constructWhereByFilters(QList<QString>filters)
{
    if (filters.count() == 0) return "";
    QString where = " WHERE ";
    int c = 0;
    foreach(QString filter, filters){
        where.append(QString(" %1 ").arg(filter));
        if (++c < filters.count())
            where.append(" AND ");
    }
    return where;
}

void EmuFrontQueryModel::clearFilters()
{
    setQuery(constructSelect());
}

