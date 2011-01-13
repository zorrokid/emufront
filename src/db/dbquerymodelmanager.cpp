/*
** EmuFront
** Copyright 2010-2011 Mikko Keinänen
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
** You should have received a cyopy of the GNU General Public License
** along with EmuFront.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
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
    //if (!sqlTableModel) sqlTableModel = getDataModel();
    if (!sqlTableModel)
        getDataModel();
    QString where = constructWhereByFilters(filters);
    //qDebug() << "Constructing SQL with " << where;
    QString query = constructSelect(where);
    sqlTableModel->setQuery(query);
}

QString DbQueryModelManager::constructWhereByFilters(QList<QString>filters)
{
    if (filters.count() == 0) return "";
    QString where = " WHERE ";
    int c = 0;
    foreach(QString filter, filters){
        where.append(QString(" %1 ").arg(filter));
        if (++c < filters.count())
            where.append(" AND ");
    }
    //qDebug() << "constructWhereByFilters: " << where;
    return where;
}

void DbQueryModelManager::clearFilters()
{
    sqlTableModel->setQuery(constructSelect());
}

bool DbQueryModelManager::deleteDataObject(int id) const
{
    QString sql = getDeleteObjectSql();
    //qDebug() << sql;
    QSqlQuery q;
    q.prepare(sql);
    q.bindValue(":id", id);
    bool ret =  q.exec();
    if (!ret) {
           qDebug() << q.lastError().text();
    }
    return ret;
}

QString DbQueryModelManager::getDeleteObjectSql() const
{
    return QString("DELETE FROM %1 WHERE id=:id").arg(tableName);
}

