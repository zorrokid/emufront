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


DbPlatform::DbPlatform(QObject *parent) : DbTableModelManager(parent)
{
    sqlTableModel = 0; //getData();
}

EmuFrontObject* DbPlatform::recordToDataObject(const QSqlRecord *record) const
{
    int id = record->value(Platform_Id).toInt();
    QString name = record->value(Platform_Name).toString();
    QString fileName = record->value(Platform_Filename).toString();
    //qDebug() << "Got platform Name " << name << " id " << id;
    return new Platform(id, name, fileName);
}

bool DbPlatform::updateDataObjectToModel(const EmuFrontObject *ob)
{
    const Platform *plf = dynamic_cast<const Platform*>(ob);
    bool ret = false;
    QSqlTableModel *tmodel = dynamic_cast<QSqlTableModel*>(sqlTableModel);
    tmodel->setFilter(QString("id = %1").arg(plf->getId()));
    tmodel->select();
    if (tmodel->rowCount() == 1)
    {
        QSqlRecord record = tmodel->record(0);
        record.setValue("name", plf->getName());
        record.setValue("filename", plf->getFilename());
        tmodel->setRecord(0, record);
        ret = tmodel->submitAll();
    }
    resetModel();
    return ret;
}

bool DbPlatform::insertDataObjectToModel(const EmuFrontObject *ob)
{
    const Platform *plf = dynamic_cast<const Platform*>(ob);
    int row = 0;
    QSqlTableModel *tmodel = dynamic_cast<QSqlTableModel*>(sqlTableModel);
    tmodel->insertRows(row, 1);
    // the null value for index will be set implicitily
    // when we don't assign any value to cell 0 in the sql table model
    //sqlTableModel->setData(sqlTableModel->index(row, 0), NULL);
    tmodel->setData(sqlTableModel->index(row, 1), plf->getName());
    tmodel->setData(sqlTableModel->index(row, 2), plf->getFilename());
    return tmodel->submitAll();
}

int DbPlatform::countDataObjectRefs(int id) const
{
    return countRows("imagecontainer", "platformid", id);
}

// WARNING: this will delete also all the databindings to selected platform
bool DbPlatform::deleteDataObjectFromModel(QModelIndex *index)
{
    QSqlDatabase::database().transaction();
    QSqlTableModel *tmodel = dynamic_cast<QSqlTableModel*>(sqlTableModel);
    QSqlRecord record = tmodel->record(index->row());
    int id = record.value(Platform_Id).toInt();
    qDebug() << "Deleting platform " << id;
    int count = countDataObjectRefs(id);
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
    tmodel->removeRow(index->row());
    tmodel->submitAll();
    return QSqlDatabase::database().commit();
}

QSqlQueryModel* DbPlatform::getData()
{
    QSqlTableModel *model = new QSqlTableModel(this);
    model->setTable(DB_TABLE_NAME_PLATFORM);
    model->setSort(Platform_Name, Qt::AscendingOrder);
    model->setHeaderData(Platform_Name, Qt::Horizontal, tr("Name"));
    model->select();
    return model;
}

/*void DbPlatform::filterById(int id)
{
    QSqlTableModel *tmodel = dynamic_cast<QSqlTableModel*>(sqlTableModel);
    tmodel->setFilter(QString("id = %1").arg(id));
    tmodel->setQuery(constructSelectById(id));
    tmodel->select();
}*/
