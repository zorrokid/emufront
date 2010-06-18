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

#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlRelationalTableModel>
#include <QDebug>
#include "dbemufrontfileobject.h"

DbEmuFrontFileObject::DbEmuFrontFileObject(QObject *parent)
    : DbTableModelManager(parent)
{
    dbFile = new DbFile(this);
}

EmuFrontObject* DbEmuFrontFileObject::recordToDataObject(const QSqlRecord *record)
{
    int id = record->value(EmuFrontFileObject_Id).toInt();
    QString name = record->value(EmuFrontFileObject_Name).toString();
    int fileId = record->value(EmuFrontFileObject_FileId).toInt();
    EmuFrontFile *f = 0;
    if (fileId > 0)
    {
        EmuFrontObject *o = dbFile->getDataObject(fileId);
        f = dynamic_cast<EmuFrontFile*>(o);
    }
    EmuFrontObject *efo = createEmuFrontFileObject(id, name, f);
    return efo;
}

bool DbEmuFrontFileObject::updateDataObjectToModel(const EmuFrontObject *ob)
{
    const EmuFrontFileObject *plf = dynamic_cast<const EmuFrontFileObject*>(ob);
    bool ret = false;
    QSqlTableModel *tmodel = dynamic_cast<QSqlTableModel*>(sqlTableModel);
    tmodel->setFilter(QString("id = %1").arg(plf->getId()));
    tmodel->select();
    if (tmodel->rowCount() == 1)
    {
        QSqlRecord record = tmodel->record(0);
        record.setValue("name", plf->getName());
        if (plf->getFile())
            record.setValue("fileid", plf->getFile()->getId());
        else record.setNull("fileid");
        tmodel->setRecord(0, record);
        ret = tmodel->submitAll();
    }
    resetModel();
    return ret;
}

bool DbEmuFrontFileObject::insertDataObjectToModel(const EmuFrontObject *ob)
{
    const EmuFrontFileObject *plf = dynamic_cast<const EmuFrontFileObject *>(ob);
    int row = 0;
    if (!sqlTableModel) sqlTableModel = getDataModel();
    QSqlTableModel *tmodel = dynamic_cast<QSqlTableModel*>(sqlTableModel);
    tmodel->insertRows(row, 1);
    // the null value for index will be set implicitily
    // when we don't assign any value to cell 0 in the sql table model
    //sqlTableModel->setData(sqlTableModel->index(row, 0), NULL);
    tmodel->setData(sqlTableModel->index(row, EmuFrontFileObject_Name), plf->getName());
    if (plf->getFile())
        tmodel->setData(sqlTableModel->index(row, EmuFrontFileObject_FileId), plf->getFile()->getId());
    return tmodel->submitAll();
}

int DbEmuFrontFileObject::countDataObjectRefs(int id) const
{
    return 0; // TODO
    // return countRows("imagecontainer", "platformid", id);
}

// WARNING: this will delete also all the databindings to selected platform
bool DbEmuFrontFileObject::deleteDataObjectFromModel(QModelIndex *index)
{
    //QSqlDatabase::database().transaction();
    QSqlTableModel *tmodel = dynamic_cast<QSqlTableModel*>(sqlTableModel);
    /*QSqlRecord record = tmodel->record(index->row());
    int id = record.value(EmuFrontFileObject_Id).toInt();
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
    }*/
    tmodel->removeRow(index->row());
    tmodel->submitAll();
    return QSqlDatabase::database().commit();
}

QSqlQueryModel* DbEmuFrontFileObject::getData()
{
    QSqlRelationalTableModel *model = new QSqlRelationalTableModel(this);
    model->setTable(tableName);
    // TODO: table realtion model seems not to be suitable for this
    // since not always does data object have a file relation:
    //model->setRelation(EmuFrontFileObject_FileId, QSqlRelation("file", "id", "name"));
    model->setSort(EmuFrontFileObject_Name, Qt::AscendingOrder);
    model->setHeaderData(EmuFrontFileObject_Name, Qt::Horizontal, tr("Name"));
    model->setHeaderData(EmuFrontFileObject_FileId, Qt::Horizontal, tr("Icon"));
    model->select();
    return model;
}
