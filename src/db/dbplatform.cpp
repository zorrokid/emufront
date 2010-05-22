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

#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QDebug>
#include "dbplatform.h"

const QString DbPlatform::DB_TABLE_NAME_PLATFORM = QString("platform");

DbPlatform::DbPlatform(QObject *parent) : DatabaseManager(parent)
{
    sqlTableModel = getData();
}

QSqlTableModel* DbPlatform::getDataModel()
{
    return sqlTableModel;
}

Platform* DbPlatform::getPlatformFromModel(QModelIndex *index)
{
    QSqlRecord record = sqlTableModel->record(index->row());
    int id = record.value(Platform_Id).toInt();
    QString name = record.value(Platform_Name).toString();
    QString fileName = record.value(Platform_Filename).toString();
    //qDebug() << "Got platform Name " << name << " id " << id;
    return new Platform(id, name, fileName);
}

bool DbPlatform::updatePlatformToModel(const Platform *ob)
{
    bool ret = false;
    sqlTableModel->setFilter(QString("id = %1").arg(ob->getId()));
    sqlTableModel->select();
    if (sqlTableModel->rowCount() == 1)
    {
        QSqlRecord record = sqlTableModel->record(0);
        record.setValue("name", ob->getName());
        record.setValue("filename", ob->getFilename());
        sqlTableModel->setRecord(0, record);
        ret = sqlTableModel->submitAll();
    }
    resetModel();
    return ret;
}

bool DbPlatform::insertPlatformToModel(const Platform *ob)
{
    int row = 0;
    sqlTableModel->insertRows(row, 1);
    // the null value for index will be set implicitily
    // when we don't assign any value to cell 0 in the sql table model
    //sqlTableModel->setData(sqlTableModel->index(row, 0), NULL);
    sqlTableModel->setData(sqlTableModel->index(row, 1), ob->getName());
    sqlTableModel->setData(sqlTableModel->index(row, 2), ob->getFilename());
    return sqlTableModel->submitAll();
}

int DbPlatform::countPlatformBindings(int id) const
{
    int numEntries = 0;
    QSqlQuery query(QString("SELECT COUNT(*) FROM imagecontainer WHERE platformid = %1").arg(id));
    if (query.next())
        numEntries = query.value(0).toInt();
    return numEntries;
}

// WARNING: this will delete also all the databindings to selected platform
bool DbPlatform::deletePlatformFromModel(QModelIndex *index)
{
    QSqlDatabase::database().transaction();
    QSqlRecord record = sqlTableModel->record(index->row());
    int id = record.value(Platform_Id).toInt();
    qDebug() << "Deleting platform " << id;
    int count = countPlatformBindings(id);
    if (count > 0)
    {
        QSqlQuery query;
        if (!query.exec(QString("DELETE FROM imagecontainer WHERE platformid = %1").arg(id)))
        {
            qDebug() << "Deleting data bindings failed!";
            QSqlDatabase::database().rollback();
            return false;
        }
    }
    sqlTableModel->removeRow(index->row());
    sqlTableModel->submitAll();
    return QSqlDatabase::database().commit();
}

QSqlTableModel* DbPlatform::getData()
{
    QSqlTableModel *model = new QSqlTableModel(this);
    model->setTable(DB_TABLE_NAME_PLATFORM);
    model->setSort(Platform_Name, Qt::AscendingOrder);
    model->setHeaderData(Platform_Name, Qt::Horizontal, tr("Name"));
    model->select();
    return model;
}
